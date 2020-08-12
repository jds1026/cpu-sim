#ifndef PROCESS_QUEUE_H
#define PROCESS_QUEUE_H

/* Process structure */
typedef struct process {
	unsigned int ID;
	unsigned int burst;
	unsigned int arr_time;
} Process;

/* Process Node structure, contains a process
   data structure */
typedef struct process_n {
	struct process   job;
	struct process_n *next;
} Process_Node;



/* Process Queue structure for Job Queue and Ready Queue */
class Process_Queue {
private:
	struct process_n *front;
	struct process_n *rear;
	unsigned int 	  queue_size;
	unsigned int 	  total_process_wait;
public:
	Process_Queue();
	~Process_Queue();
	void    enqueue(Process p);
	void 	dequeue();
	Process front_node();
	void 	count();
	bool 	is_empty();
	void 	calculate_queue_wait(unsigned int *stat);
};

#endif
