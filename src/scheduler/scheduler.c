/*
 * This file is part of sim. Copyright 2016 Juan R. Garcia Blanco
 * 
 * sim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * sim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with sim.  If not, see <http://www.gnu.org/licenses/>.
 */

sim_scheduler_err_t
sim_scheduler_initialize (sim_scheduler_t *sch)
{
}

sim_scheduler_err_t
sim_scheduler_finalize (sim_scheduler_t *sch)
{
}

sim_scheduler_err_t
sim_scheduler_set_tick_duration (sim_scheduler_t *sch,
                                 uint64_t         tick)
{
}

sim_scheduler_err_t
sim_scheduler_get_tick_duration (sim_scheduler_t *sch,
                                 uint64_t        *tick)
{
}

sim_scheduler_err_t
sim_scheduler_add_event (sim_scheduler_t *sch,
                         uint64_t         simtime,
                         uint64_t        *evid)
{
}
