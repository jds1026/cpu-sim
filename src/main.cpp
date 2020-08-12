#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "process_queue.h"
#include "cpu.h"
#include "clock.h"
#include "simulation_statistics.h"

// CPUs
CPU cpu[NUM_PROCESSORS];

//Clock
Clock sys_clock{MAX_TIME};

// Process Queues
Process_Queue job_queue{};
Process_Queue ready_queue{}; 

// Simulation Statistics
Simulation_Statistics sim_stats{};

/* Simulation Functions */

void clock_cycle();
void generate_processes();
bool incoming_process();
void jobq_to_readyq();
void initialize_sim_stats(Simulation_Statistics *s);
bool available_cpu();
void obtain_cpu(CPU **ref_cpu);	
void dispatcher(CPU **cpu);
void update_states();
int  add_process_wait_t(Process p);
void display(double extime);

/************************/

int main() {
	clock_t begin, end;
	double 	execution_time;
	begin = clock();
	srand(time(NULL));

	// initialize all sim stats to zero
	initialize_sim_stats(&sim_stats);

	// main execution loop
	while (sys_clock.time_remaining()) {
		clock_cycle();
	}

	end = clock();
	execution_time = (double)(end - begin) / CLOCKS_PER_SEC;

	display(execution_time);

	return 0;
}

void clock_cycle() {
	generate_processes();
	jobq_to_readyq();
	while (available_cpu() && !ready_queue.is_empty()) {
		CPU* cpu;
		dispatcher(&cpu);
	}
	update_states();

	sys_clock.increment_clock();
}

void generate_processes() {
	static unsigned int id_num = 0;
	while (incoming_process()) {
		// generate ID, burst time, and
		// arrival time for new process
		unsigned int btime = (rand() % 10) + 1;
		Process p{id_num++, btime, sys_clock.get_time()};

		// add to job queue
		job_queue.enqueue(p);

		/*************/
		/* for stats */
		/*************/

		// add to queue len for job
		sim_stats.jqueue_len++;
		// set max queue len for job queue and at which time it was at max
		if (sim_stats.jqueue_len > sim_stats.max_jqueue_len) {
			sim_stats.max_jqueue_len = sim_stats.jqueue_len;
			sim_stats.time_at_max_jqlen = sys_clock.get_time();
		}
		// total processes
		sim_stats.total_proc_in_jqueue++;
		// max_process_len
		if (p.burst > sim_stats.max_process_len) {
			sim_stats.max_process_len = p.burst;
		}
		// min_process_len
		if (p.burst < sim_stats.min_process_len) {
			sim_stats.min_process_len = p.burst;
		}
		// total_process_len
		sim_stats.total_process_len += p.burst;
	}
}

/* 50% chance that a new process is created */
bool incoming_process() {
	double ret = (double)rand() / RAND_MAX;

	if (ret > .5) {
		return true;
	} else {
		return false;
	}
}

void jobq_to_readyq() {
	while (incoming_process()) {
		if (job_queue.is_empty()) {
			break;
		}

		ready_queue.enqueue(job_queue.front_node());
		job_queue.dequeue();
		// remove length number from job queue
		sim_stats.jqueue_len--;
		// add to ready queue total process
		sim_stats.total_proc_in_rqueue++;
		// add to ready queue len
		sim_stats.rqueue_len++;
		// set max queue len for ready queue and at which time it was at max
		if (sim_stats.rqueue_len > sim_stats.max_rqueue_len) {
			sim_stats.max_rqueue_len = sim_stats.rqueue_len;
			sim_stats.time_at_max_rqlen = sys_clock.get_time();
		}
	}
}

bool available_cpu() {
	for (CPU& c : cpu) {
		if (c.is_idle()) {
			return true;
		}
	}
	return false;
}

void obtain_cpu(CPU **ref_cpu) {
	for (CPU& c : cpu) {
		if (c.is_idle()) {
			(*ref_cpu) = &c;
		}
	}
}

void dispatcher(CPU **cpu) {
	// grab CPU from CPU array
	obtain_cpu(cpu);
	// give control of CPU to process
	(*cpu)->assign_process(ready_queue.front_node(), sys_clock.get_time());
	// add process wait time to total
	sim_stats.total_process_wait_time += add_process_wait_t(ready_queue.front_node());
	// remove from ready queue
	ready_queue.dequeue();
	// remove one from ready queue len
	sim_stats.rqueue_len--;
	// make cpu unavailable
	(*cpu)->set_busy();
}

void update_states() {
	for (CPU& c : cpu) {
		if (!c.is_idle()) {
			c.update();
			if (c.is_idle()) {
				sim_stats.total_proc_complete++;
				sim_stats.total_turnaround_time += c.get_process_tt();
			}
		} else {
			// add to CPU's total idle time
			c.increment_idle();
		}
	}

	// calculate process wait times
	job_queue.calculate_queue_wait(&sim_stats.total_jwait_time);
	ready_queue.calculate_queue_wait(&sim_stats.total_rwait_time);
}

int  add_process_wait_t(Process p) {
	return (sys_clock.get_time() - p.arr_time);
}

void initialize_sim_stats(Simulation_Statistics *s) {
	s->total_turnaround_time = 0;
	s->av_turnaround_time = 0;

	s->total_process_wait_time = 0;
	s->av_process_wait_time = 0;

	s->max_process_len = 0; 
	s->min_process_len = 1; 
	s->total_process_len = 0;
	s->av_process_len = 0; 

	s->jqueue_len = 0;
	s->rqueue_len = 0;

	s->max_jqueue_len = 0;
	s->max_rqueue_len = 0;

	s->time_at_max_jqlen = 0;
	s->time_at_max_rqlen = 0;

	s->total_jwait_time = 0;
	s->total_rwait_time = 0;

	s->av_jwait_time = 0;
	s->av_rwait_time = 0;

	s->total_proc_in_jqueue = 0;
	s->total_proc_in_rqueue = 0;

	s->total_proc_complete = 0;
	s->proc_completion_rate = 0;
}

void display(double extime) {

	// get averages
	sim_stats.proc_completion_rate = 100 * (double)sim_stats.total_proc_complete / (double)sim_stats.total_proc_in_jqueue;
	sim_stats.av_turnaround_time = sim_stats.total_turnaround_time / sim_stats.total_proc_complete;
	sim_stats.av_process_wait_time = sim_stats.total_process_wait_time / sim_stats.total_proc_in_rqueue;
	sim_stats.av_process_len = sim_stats.total_process_len / sim_stats.total_proc_in_jqueue;
	sim_stats.av_jwait_time = sim_stats.total_jwait_time / sim_stats.total_proc_in_jqueue;
	sim_stats.av_rwait_time = sim_stats.total_rwait_time / sim_stats.total_proc_in_jqueue;


	// display results
	std::cout << "************************** CPU SIMULATION **************************" << std::endl << std::endl;
	printf("%34s%f seconds\n\n", "EXECUTION TIME : ", extime);
	printf("%29d%s\n\n", MAX_TIME, " CLOCK CYCLES");
	printf("%39s%6.2f%%\n\n", "COMPLETION RATE : ", sim_stats.proc_completion_rate);
	// printf("%52s\n", "------------------------------------");
	printf("%21s", "PROCESSES");
	printf("%35s\n", "JOB QUEUE");
	printf("---------------------------------");
	printf("%35s\n","---------------------------------");
	printf("%26s%7d", "Total Complete : ", sim_stats.total_proc_complete);
	printf("%30s%5d\n", "Total Processes : ", sim_stats.total_proc_in_jqueue);
	printf("%26s%7d", "Total Burst : ", sim_stats.total_process_len);
	printf("%30s%5d\n", "Max Queue Size : ", sim_stats.max_jqueue_len);
	printf("%26s%7d", "Max Burst : ", sim_stats.max_process_len);
	printf("%30s%5d\n", "Cycle At Max Size : ", sim_stats.time_at_max_jqlen);
	printf("%26s%7d", "Min Burst : ", sim_stats.min_process_len);
	printf("%30s%5d\n", "Total Wait Time In Queue : ", sim_stats.total_jwait_time);
	printf("%26s%7d", "Avg Burst : ", sim_stats.av_process_len);
	printf("%30s%5d\n", "Avg Queue Wait Time : ", sim_stats.av_jwait_time);
	printf("%26s%7d\n", "Avg Turnaround Time : ", sim_stats.av_turnaround_time);
	printf("%26s%7d", "Avg Wait Time : ", sim_stats.av_process_wait_time);
	printf("%24s\n", "READY QUEUE");
	printf("%68s\n", "---------------------------------");
	printf("%63s%5d\n", "Total Processes : ", sim_stats.total_proc_in_rqueue);
	printf("%63s%5d\n", "Max Queue Size : ", sim_stats.max_rqueue_len);
	printf("%63s%5d\n", "Cycle At Max Size : ", sim_stats.time_at_max_rqlen);
	printf("%63s%5d\n", "Total Wait Time In Queue : ", sim_stats.total_rwait_time);
	printf("%63s%5d\n\n", "Avg Queue Wait Time : ", sim_stats.av_rwait_time);
	printf("%10s", "CPU 1");
	printf("%17s", "CPU 2");
	printf("%19s", "CPU 3");
	printf("%17s\n", "CPU 4");
	printf("---------------  ---------------    ---------------  ---------------\n");
	printf("%s%3d", "Processes : ", cpu[0].get_num_processes());
	printf("%14s%3d", "Processes : ", cpu[1].get_num_processes());
	printf("%16s%3d", "Processes : ", cpu[2].get_num_processes());
	printf("%14s%3d\n", "Processes : ", cpu[3].get_num_processes());
	printf("%s%3d", "Busy Time : ", cpu[0].get_busy());
	printf("%14s%3d", "Busy Time : ", cpu[1].get_busy());
	printf("%16s%3d", "Busy Time : ", cpu[2].get_busy());
	printf("%14s%3d\n", "Busy Time : ", cpu[3].get_busy());
	printf("%s%3d", "Idle Time : ", cpu[0].get_idle());
	printf("%14s%3d", "Idle Time : ", cpu[1].get_idle());
	printf("%16s%3d", "Idle Time : ", cpu[2].get_idle());
	printf("%14s%3d\n\n", "Idle Time : ", cpu[3].get_idle());
	std::cout << "********************************************************************" << std::endl << std::endl;
}


