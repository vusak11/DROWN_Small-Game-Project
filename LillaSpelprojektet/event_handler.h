#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SFML/Window.hpp>

#include "Camera/camera_handler.h"

class EventHandler
{
private:
	CameraHandler* cam_handler_ptr;	//NTS: Temp solution while the position of the EventHandler is decided
public:
	EventHandler();
	~EventHandler();

	void InputFromDevices();
};

#endif // !EVENTHANDLER_H

