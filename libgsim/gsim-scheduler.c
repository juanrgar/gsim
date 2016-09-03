/*
 * This file is part of GSIM. Copyright 2016 Juan R. Garcia Blanco
 * 
 * GSIM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * GSIM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GSIM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gsim-scheduler.h"

G_DEFINE_TYPE (GsimScheduler, gsim_scheduler, G_TYPE_OBJECT);

#define SCHEDULER_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GSIM_TYPE_SCHEDULER, GsimSchedulerPrivate))

typedef struct _GsimSchedulerPrivate
{
    guint64 sim_time;
    GsimSchedulerState state;

    guint64 tolerance;
    guint64 cycle;

    GThread *sched_thread;
    GMutex sched_mutex;
    GCond sched_cond;

    gboolean do_exit : 1;
} GsimSchedulerPrivate;


enum {
    PROP_0,

    PROP_TOLERANCE,
    PROP_CYCLE,
    PROP_STATE
};

static void
gsim_scheduler_get_property (GObject    *object,
                             guint       property_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
    GsimScheduler *scheduler = GSIM_SCHEDULER (object);
    GsimSchedulerPrivate *priv = SCHEDULER_PRIVATE (scheduler);

    switch (property_id)
    {
        case PROP_TOLERANCE:
            g_value_set_uint64 (value, priv->tolerance);
            break;
        case PROP_CYCLE:
            g_value_set_uint64 (value, priv->cycle);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
gsim_scheduler_set_property (GObject      *object,
                             guint         property_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
    GsimScheduler *scheduler = GSIM_SCHEDULER (object);

    switch (property_id)
    {
        case PROP_TOLERANCE:
            gsim_scheduler_set_tolerance (scheduler,
                                          g_value_get_uint64 (value));
            break;
        case PROP_CYCLE:
            gsim_scheduler_set_cycle (scheduler,
                                     g_value_get_uint64 (value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
gsim_scheduler_dispose (GObject *object)
{
    G_OBJECT_CLASS (gsim_scheduler_parent_class)->dispose (object);
}

static void
gsim_scheduler_finalize (GObject *object)
{
    GsimSchedulerPrivate *priv = SCHEDULER_PRIVATE (GSIM_SCHEDULER (object));

    if (priv->sched_thread != NULL)
    {
        g_mutex_lock (&priv->sched_mutex);
        priv->do_exit = TRUE;
        g_cond_signal (&priv->sched_cond);
        g_mutex_unlock (&priv->sched_mutex);
        gpointer ret = g_thread_join (priv->sched_thread);
    }

    g_mutex_clear (&priv->sched_mutex);
    g_cond_clear (&priv->sched_cond);

    G_OBJECT_CLASS (gsim_scheduler_parent_class)->finalize (object);
}

static void
gsim_scheduler_class_init (GsimSchedulerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GsimSchedulerPrivate));

    object_class->get_property = gsim_scheduler_get_property;
    object_class->set_property = gsim_scheduler_set_property;
    object_class->dispose = gsim_scheduler_dispose;
    object_class->finalize = gsim_scheduler_finalize;

    g_object_class_install_property (object_class,
                                     PROP_TOLERANCE,
                                     g_param_spec_uint64 ("tolerance",
                                                          "Tolerance",
                                                          "",
                                                          0,
                                                          G_MAXUINT64,
                                                          500,
                                                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property (object_class,
                                     PROP_CYCLE,
                                     g_param_spec_uint64 ("cycle",
                                                          "Cycle",
                                                          "",
                                                          0,
                                                          G_MAXUINT64,
                                                          100,
                                                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

static gpointer sched_thread_func (gpointer data);

static void
gsim_scheduler_init (GsimScheduler *self)
{
    GsimSchedulerPrivate *priv = SCHEDULER_PRIVATE (self);

    self->priv = priv;

    priv->state = GSIM_SCHEDULER_STATE_HOLD;
    priv->do_exit = FALSE;

    g_mutex_init (&priv->sched_mutex);
    g_cond_init (&priv->sched_cond);

    priv->sched_thread = g_thread_new ("sched-thread", sched_thread_func, self);
}

GsimScheduler *
gsim_scheduler_new (void)
{
    return g_object_new (GSIM_TYPE_SCHEDULER, NULL);
}

void
gsim_scheduler_set_tolerance (GsimScheduler *scheduler,
                              guint64        tolerance)
{
    GsimSchedulerPrivate *priv = SCHEDULER_PRIVATE (scheduler);

    priv->tolerance = tolerance;
}

guint64
gsim_scheduler_get_tolerance (GsimScheduler *scheduler)
{
    return SCHEDULER_PRIVATE(scheduler)->tolerance;
}

void
gsim_scheduler_set_cycle (GsimScheduler *scheduler,
                          guint64        cycle)
{
    GsimSchedulerPrivate *priv = SCHEDULER_PRIVATE (scheduler);

    priv->cycle = cycle;
}

guint64
gsim_scheduler_get_cycle (GsimScheduler *scheduler)
{
    return SCHEDULER_PRIVATE(scheduler)->cycle;
}

/*
guint64            gsim_scheduler_get_sim_time  (GsimScheduler    *scheduler);

GsimSchedulerState gsim_scheduler_get_state     (GsimScheduler    *scheduler);
*/
gboolean
gsim_scheduler_run (GsimScheduler *scheduler)
{
    gboolean ret = FALSE;
    GsimSchedulerPrivate *priv = SCHEDULER_PRIVATE (scheduler);

    if (priv->state == GSIM_SCHEDULER_STATE_HOLD)
    {
        g_debug ("Transition to RUN");

        priv->state = GSIM_SCHEDULER_STATE_RUN;
        g_mutex_lock (&priv->sched_mutex);
        g_cond_signal (&priv->sched_cond);
        g_mutex_unlock (&priv->sched_mutex);
        ret = TRUE;
    }

    return ret;
}
/*
gboolean           gsim_scheduler_pause         (GsimScheduler    *scheduler);

guint64            gsim_scheduler_add_event     (GsimScheduler    *scheduler,
                                                 GsimSchedulerFunc func,
                                                 const gchar      *name,
                                                 guint64           delay,
                                                 guint64           cycle,
                                                 gint64            repeat_count);
*/

static gpointer
sched_thread_func (gpointer data)
{
    gint ret = 0;
    GsimScheduler* self = GSIM_SCHEDULER (data);
    GsimSchedulerPrivate *priv = SCHEDULER_PRIVATE (self);

    g_debug ("Scheduler thread created");

    while (TRUE)
    {
        g_debug ("Scheduler thread loop iteration");

        g_mutex_lock (&priv->sched_mutex);

        while ((priv->state == GSIM_SCHEDULER_STATE_HOLD) &&
               !priv->do_exit)
        {
            g_cond_wait (&priv->sched_cond, &priv->sched_mutex);
        }

        if (priv->do_exit)
        {
            g_thread_exit ((gpointer)(uintptr_t)ret);
            return NULL;
        }

        g_mutex_unlock (&priv->sched_mutex);

        g_debug ("Scheduler thread released");

        gulong cycle_us = priv->cycle * 1.0e3;

        g_usleep (cycle_us);

        priv->sim_time += priv->cycle;

        g_debug ("Tick (%ld ms) - S: %ld", priv->cycle, priv->sim_time);
    }

    return NULL;
}
