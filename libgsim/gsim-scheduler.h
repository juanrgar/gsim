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

#ifndef __GSIM_SCHEDULER_H__
#define __GSIM_SCHEDULER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GSIM_TYPE_SCHEDULER gsim_scheduler_get_type()

#define GSIM_SCHEDULER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                 GSIM_TYPE_SCHEDULER, GsimScheduler))

#define GSIM_SCHEDULER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), \
                              GSIM_TYPE_SCHEDULER, GsimSchedulerClass))

#define GSIM_IS_SCHEDULER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 GSIM_TYPE_SCHEDULER))

#define GSIM_IS_SCHEDULER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                              GSIM_TYPE_SCHEDULER))

#define GSIM_SCHEDULER_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                GSIM_TYPE_SCHEDULER, GsimSchedulerClass))

typedef struct _GsimScheduler GsimScheduler;
typedef struct _GsimSchedulerClass GsimSchedulerClass;
typedef struct _GsimSchedulerPrivate GsimSchedulerPrivate;

typedef gpointer (*GsimSchedulerFunc) (guint64 id,
                                       gpointer data);

typedef enum {
    GSIM_SCHEDULER_STATE_HOLD,
    GSIM_SCHEDULER_STATE_RUN,
} GsimSchedulerState;

struct _GsimScheduler
{
    GObject parent;

    GsimSchedulerPrivate *priv;
};

struct _GsimSchedulerClass
{
    GObjectClass parent_class;
};

GType              gsim_scheduler_get_type      (void) G_GNUC_CONST;

GsimScheduler *    gsim_scheduler_new           (void);

void               gsim_scheduler_set_tolerance (GsimScheduler    *scheduler,
                                                 guint64           tolerance);
guint64            gsim_scheduler_get_tolerance (GsimScheduler    *scheduler);

void               gsim_scheduler_set_cycle     (GsimScheduler    *scheduler,
                                                 guint64           cycle);
guint64            gsim_scheduler_get_cycle     (GsimScheduler    *scheduler);

guint64            gsim_scheduler_get_sim_time  (GsimScheduler    *scheduler);

GsimSchedulerState gsim_scheduler_get_state     (GsimScheduler    *scheduler);

gboolean           gsim_scheduler_run           (GsimScheduler    *scheduler);
gboolean           gsim_scheduler_pause         (GsimScheduler    *scheduler);

guint64            gsim_scheduler_add_event     (GsimScheduler    *scheduler,
                                                 GsimSchedulerFunc func,
                                                 const gchar      *name,
                                                 guint64           delay,
                                                 guint64           cycle,
                                                 gint64            repeat_count);

G_END_DECLS

#endif /* __GSIM_SCHEDULER_H__ */
