#ifndef CLOCK_H
#define CLOCK_H

#include <string>

#define UPDATE_INTERVAL 1

#define CLOCK_FILE "Data/system.txt"

class Clock {
public:
	Clock(const int fps = load_cap(CLOCK_FILE));

	// updates _time 
	// returns true once each interval
	bool update(const double interval = UPDATE_INTERVAL);
	void reset();

	// fps limit
	void limit(const bool limit);
	void set_limit(const int limit);

	double get_time();
	double get_fms();

	std::string get_display_time();

	static int load_cap(const char* file_path = CLOCK_FILE);
private:
	int _limit;
	double _frames;
	double _fms, _ms, _time;
	double _ticks, _previous_ticks, _update_ticks;
	bool _is_limit;

	void update_time();
};

#endif
