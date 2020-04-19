#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Clock;
class Log;
class Window;
class ResourceManager;

class Environment {
public:
	Environment();
	~Environment();

	void set_mode(int mode);
	void set_clock(Clock* clock);
	void set_log(Log* log);
	void set_window(Window* window);
	void set_resource_manager(ResourceManager* resource_manager);

	int     get_mode();
	Clock*  get_clock();
	Log*    get_log();
	Window* get_window();
	ResourceManager* get_resource_manager();

	void shut_down();

	static Environment& get();
private:
	int     _mode;
	Clock*  _clock;
	Log*    _log;
	Window* _window;
	ResourceManager* _resource_manager;

	static Environment* _instance;
};

#endif