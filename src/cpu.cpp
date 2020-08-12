#include <iostream>
#include "cpu.h"

CPU::CPU() {
	process_size = 0;
	process_turnaround = 0;
	num_of_processes = 0;
	max_job_len = 0;
	total_busy_time = 0;
	total_idle_time = 0;
	idle = true;
}

void CPU::increment_idle() {
	total_idle_time++;
}

void CPU::increment_busy() {
	total_busy_time++;
}

bool CPU::is_idle() {
	return idle;
}

void CPU::assign_process(Process p, unsigned int time) {
	process_size = p.burst;

	// calculate max process for each CPU
	if (p.burst > max_job_len) {
		max_job_len = p.burst;
	}
	// calculate jobs's turnaround time
	process_turnaround = (time + process_size) - p.arr_time;
}

int CPU::get_burst() {
	return process_size;
}

void CPU::set_idle() {
	idle = true;
}

int CPU::get_idle() const {
	return total_idle_time;
}

void CPU::set_busy() {
	idle = false;
}

int CPU::get_busy() const {
	return total_busy_time;
}

void CPU::update() {
	if (process_size > 0){
		process_size--;
		// add to CPU's total busy time
		total_busy_time++;
	}

	if (process_size == 0) {
		set_idle();
		// add to CPU's number of processes
		num_of_processes++;
	}
}

int CPU::get_process_tt() const {
	return process_turnaround;
}

int CPU::get_num_processes() const {
	return num_of_processes;
}