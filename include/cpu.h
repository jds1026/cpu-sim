#ifndef CPU_H
#define CPU_H

#include "process_queue.h"
#define NUM_PROCESSORS 4

class CPU {
public:
	CPU();
	~CPU() {};
	bool is_idle();
	void increment_idle();
	void increment_busy();
	void assign_process(Process p, unsigned int time);
	int  get_burst();
	void set_idle();
	int  get_idle() const;
	void set_busy();
	int  get_busy() const;
	void update();
	int  get_process_tt() const;
	int  get_num_processes() const;
private:
	unsigned int process_size;
	unsigned int process_turnaround;
	unsigned int num_of_processes;
	unsigned int max_job_len;
	unsigned int total_busy_time;
	unsigned int total_idle_time;
	bool 	     idle;
};

#endif

