#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SFML/Window.hpp>

#include "Camera/camera_handler.h"

class EventHandler
{
private:
	CameraHandler* camera_handler_ptr_;
	//ObjectHandler* object_handler_ptr_;
public:
	EventHandler(CameraHandler* in_camera_handler_ptr/*, in_object_handler_ptr*/);
	~EventHandler();

	void InputFromDevices(float in_deltatime);
};

#endif // !EVENTHANDLER_H

