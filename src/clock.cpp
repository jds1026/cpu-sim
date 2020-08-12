#include <iostream>
#include "clock.h"

Clock::Clock(unsigned int max_time) {
	total = max_time;
	time = 0;
}

bool Clock::time_remaining() {
	return time != total;
}

void Clock::increment_clock() {
	time++;
}

void Clock::set_clock(unsigned int time) {
	this->time = time;
}

void Clock::reset_clock(unsigned int max_time) {
	total = max_time;
	time = 0;
}

unsigned int Clock::get_time() const {
	return time;
}

