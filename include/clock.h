#ifndef CLOCK_H
#define CLOCK_H

#define MAX_TIME 600

class Clock {
public:
	Clock(unsigned int max_time);
	~Clock() {};
	void 		 increment_clock();
	bool 		 time_remaining();
	void 		 set_clock(unsigned int time);
	unsigned int get_time() const;
	void 		 reset_clock(unsigned int max_time);
private:
	unsigned int total;
	unsigned int time;
};

#endif