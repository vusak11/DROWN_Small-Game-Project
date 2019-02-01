#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SFML/Window.hpp>

#include "Camera/camera_handler.h"

class EventHandler
{
private:
	CameraHandler* cam_handler_ptr_;
	//ObjectHandler* obj_handler_ptr_;
public:
	EventHandler(CameraHandler* in_cptr/*, in_optr*/);
	~EventHandler();

	void InputFromDevices();
};

#endif // !EVENTHANDLER_H

