#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
	Timer();
	Timer(double time);
	Timer(const Timer& rhs);

	void set(double time);
	void pause();
	void reset();
	void offset(double time);

	bool update();

	const bool get_pause();
	const double get_time();
	const double get_clock();
private:
	double _time;
	double _prev_time;
	double _clock;
	bool _is_paused;
};

#endif