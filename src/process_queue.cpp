#include "process_queue.h"
#include <iostream>
#include <cstddef>

Process_Queue::Process_Queue() {
	front = rear = NULL;
	queue_size = 0;
	total_process_wait = 0;
}

Process_Queue::~Process_Queue() {
	if (front != NULL) {
		Process_Node *current = front;
		Process_Node *next;
		while (current != NULL) {
			next = current->next;
			delete(current);
			current = next;
		}
	}
}

void Process_Queue::enqueue(Process p) {
	Process_Node *pnode = new Process_Node;
	pnode->job = p;
	pnode->next = NULL;

	if (front == NULL) {
		front = pnode;
		rear = pnode;
	} else {
		rear->next = pnode;
		rear = pnode;
	}
	queue_size++;
}

void Process_Queue::dequeue() {
	if (front == NULL) {
		std::cout << "Queue is empty\n";
		return;
	} else {
		Process_Node *ret = front;
		front = front->next;
		delete(ret);
	}
	queue_size--;
}

Process Process_Queue::front_node() {
	Process ready_job = front->job;
	return ready_job;
}

void Process_Queue::count() {
	Process_Node *temp = front;
	unsigned int count = 0;
	while(temp != NULL) {
		count++;
		temp = temp->next;
	}
	std::cout << "# in queue: " << count << std::endl;
}

bool Process_Queue::is_empty() {
	return queue_size == 0; 
}

void Process_Queue::calculate_queue_wait(unsigned int *stat) {
	Process_Node *temp = front;
	while (temp != NULL) {
		(*stat)++;
		temp = temp->next;
	}
}












