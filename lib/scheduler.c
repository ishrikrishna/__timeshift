#include <scheduler.h>
#include <rtc.h>
#include <kbd.h>

int sched_ticks_ms = 0; // Shall increment upto 1024 every sec
int sched_ticks = 0; // Shall increment at every sec
int sched_ticks_reset_interval = 60; // 60 secs
int sched_lock = 0;

void scheduler(){
	if(!sched_is_locked()){
		sched_acquire_lock();

		if(sched_ticks >= sched_ticks_reset_interval){
                	sched_ticks = 0;
        	}
		if(rtc_is_set()){
			rtc_wait();
			rtc_unset();
		}
		if(kbd_is_set()){
			kbd_wait(1);
			kbd_unset();
		}
		sched_release_lock();
	}
}


/**
 * Runs 1024 times every second on RTC interrupts.
 */
void update_sched_ticks(){
        int stms = sched_ticks_ms;
	if(stms > 1024){
                sched_ticks_ms = 0;
                ++sched_ticks;
        }
	if(stms > 128){
		scheduler();
	}
        ++sched_ticks_ms;
}

void sched_acquire_lock(){
	sched_lock = 1;
}
int sched_is_locked(){
	return sched_lock;
}
void sched_release_lock(){
	sched_lock = 0;
}

int get_sched_ticks(){
	return sched_ticks;
}
int get_sched_ticks_in_ms(){
	return sched_ticks_ms;
}
int get_sched_ticks_reset_interval(){
	return sched_ticks_reset_interval;
}
