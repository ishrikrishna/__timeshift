#ifndef _SCHEDULER_H
#define _SCHEDULER_H

extern void scheduler();
extern int sched_wait(int ms);
extern void update_sched_ticks();

//Scheduler Locks
extern void sched_acquire_lock();
extern int sched_is_locked();
extern void sched_release_lock();

extern int get_sched_ticks();
extern int get_sched_ticks_in_ms();
extern int get_sched_ticks_reset_interval();

#endif
