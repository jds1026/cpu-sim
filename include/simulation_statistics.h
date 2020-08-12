#ifndef SIMULATION_STATISTICS_H
#define SIMULATION_STATISTICS_H

typedef struct simulation_statistics {
	unsigned int total_turnaround_time;
	unsigned int av_turnaround_time;
	unsigned int total_process_wait_time;
	unsigned int av_process_wait_time;
	unsigned int max_process_len;
	unsigned int min_process_len;
	unsigned int total_process_len;
	unsigned int av_process_len;

	unsigned int jqueue_len;
	unsigned int rqueue_len;
	unsigned int max_jqueue_len;
	unsigned int max_rqueue_len;
	unsigned int time_at_max_jqlen;
	unsigned int time_at_max_rqlen;
	unsigned int total_jwait_time;
	unsigned int total_rwait_time;
	unsigned int av_jwait_time;
	unsigned int av_rwait_time;
	unsigned int total_proc_in_jqueue;
	unsigned int total_proc_in_rqueue;
	unsigned int total_proc_complete;
	double 		 proc_completion_rate;
} Simulation_Statistics;

#endif