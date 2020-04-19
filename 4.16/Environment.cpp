#include "Environment.h"

#include "Clock.h"
#include "Window.h"

#include <cassert>

Environment* Environment::_instance = nullptr;

Environment::Environment() :
	_mode				( NULL    ),
	_clock				( nullptr ),
	_log				( nullptr ),
	_window				( nullptr ),
	_resource_manager	( nullptr )
{
	assert(!_instance);
	_instance = this;
}

Environment::~Environment() {
	shut_down();
	_instance = nullptr;
}

Environment& Environment::get() {
	assert(_instance);
	return *_instance;
}

void Environment::set_mode(int mode) {
	_mode = mode;
}

void Environment::set_clock(Clock* clock) {
	_clock = clock;
}

void Environment::set_log(Log* log) {
	_log = log;
}

void Environment::set_window(Window* window) {
	_window = window;
}

void Environment::set_resource_manager(ResourceManager* resource_manager) {
	_resource_manager = resource_manager;
}

Clock* Environment::get_clock() {
	return _clock;
}

Log* Environment::get_log() {
	return _log;
}

Window* Environment::get_window() {
	return _window;
}

ResourceManager* Environment::get_resource_manager() {
	return _resource_manager;
}

void Environment::shut_down() {
	if (_window) {
		delete _window;
		_window = nullptr;
	}
	
	if (_log) {
		delete _log;
		_log = nullptr;
	}

	if (_clock) {
		delete _clock;
		_clock = nullptr;
	}

	if (_resource_manager) {
		delete _resource_manager;
		_resource_manager = nullptr;
	}
}