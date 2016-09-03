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

int main (int argc, char *argv[])
{
    g_message ("Test starts");

    GsimScheduler *scheduler = gsim_scheduler_new ();

    g_message ("Object created");

    g_message ("Transition to RUN");

    gsim_scheduler_run (scheduler);

    g_message ("Wait started");

    g_usleep (10 * 1.0e6);

    g_message ("Wait finished");

    g_object_unref (scheduler);

    g_message ("Object unrefed");

    return 0;
}
