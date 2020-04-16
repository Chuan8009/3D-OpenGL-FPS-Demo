#ifndef CLOCK_H
#define CLOCK_H

#include <string>

#define UPDATE_INTERVAL 1

#define CLOCK_FILE "Data/system.txt"

class Clock {
public:
	Clock(int fps = load_cap(CLOCK_FILE) );

	bool update(double interval = UPDATE_INTERVAL);
	void reset();

	void limit(bool limit);
	void set_cap(int cap);

	const double get_time();
	const double get_fms();

	const std::string get_display_time();
private:
	static const int load_cap(const char* file_path = CLOCK_FILE);
	void update_time();
private:
	int _cap;
	double _frames;
	double _fms, _ms, _time;
	double _ticks, _previous_ticks, _update_ticks;
	bool _is_limit;
};

#endif
