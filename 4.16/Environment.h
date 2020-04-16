#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Clock;
class Log;
class Window;

class Environment {
public:
	Environment();
	~Environment();

	void set_mode(int mode);
	void set_clock(Clock* clock);
	void set_log(Log* log);
	void set_window(Window* window);

	int     get_mode();
	Clock*  get_clock();
	Log*    get_log();
	Window* get_window();

	void shut_down();

	static Environment& get();
private:
	int     _mode;
	Clock*  _clock;
	Log*    _log;
	Window* _window;

	static Environment* _instance;
};

#endif