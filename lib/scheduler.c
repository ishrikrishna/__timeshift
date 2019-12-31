/**
 * This scheduler will be used only for kernel level modules.
 * It is assummed that only fixed amount of process requiring scheduler will run in kernel.
 * Therefore, Once the tasks execution completes, it is re-appended in the queue for running at next logical event.
 * 
 * This is a Round-Robin Scheduler for Kernel tasks/processes.
 * 32 slots of time (1 sec/1024) is assigned to each task.
 * Total number of tasks supported are 32
 */

#include <scheduler.h>
#include <rtc.h>
#include <kbd.h>

int rtc_event_total_in_sec = 1024;
int sched_ticks_ms = 0; // Shall increment upto 1024 every sec
int sched_ticks = 0; // Shall increment at every sec
int sched_ticks_reset_interval = 60; // 60 secs
int sched_lock = 0;
int sched_task_time_slot = 32;

// Circular Queue Module for Scheduler
const int sched_queue_sz = 32;
int sched_queue[32];
int sched_queue_front = -1;
int sched_queue_back = -1;

int sched_queue_is_full(){
	return ((
			(sched_queue_front == sched_queue_back + 1) || 
			(
			 sched_queue_front == 0 && 
			 sched_queue_back == (sched_queue_sz-1)
			)
		) ? 1 : 0);
}
int sched_queue_is_empty(){
	return (sched_queue_front == -1 ? 1 : 0);
}
int sched_queue_add(int proc_addr){
	if(sched_queue_is_full()){
		return 0;
	}
	if(sched_queue_is_empty()){
		sched_queue_front = 0;
	}

	sched_queue_back = (sched_queue_back+1)%sched_queue_sz;
	sched_queue[sched_queue_back] = proc_addr;
	return 1;
}
int sched_queue_pop(){
	if(sched_queue_is_empty()){
		return 0;
	}

	int out = sched_queue[sched_queue_front];
	if(sched_queue_front == sched_queue_back){
		sched_queue_front = sched_queue_back = -1;
	}else{
		sched_queue_front = (sched_queue_front+1)%sched_queue_sz;
	}
	return out;
}
// Scheduler Circular Queue module ends here

void scheduler(){
	if(!sched_is_locked()){
		sched_acquire_lock();

		if(sched_ticks >= sched_ticks_reset_interval){
                	sched_ticks = 0;
        	}

		if(!sched_queue_is_empty()){
			int proc = sched_queue_pop();
			if(proc > 0){
				sched_queue_add(proc);
				((void (*)())proc)();
			}
		}
		
		sched_release_lock();
	}
}


/**
 * Runs 1024 times every second on RTC interrupts.
 */
void update_sched_ticks(){
        int stms = sched_ticks_ms;
	if(stms > rtc_event_total_in_sec){
                sched_ticks_ms = 0;
                ++sched_ticks;
        }
	if(stms > sched_task_time_slot){
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
