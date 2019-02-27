#ifndef DOOR_H
#define DOOR_H

#include "../object_class.h"

class Door : public ObjectClass {
private:
	enum STATE {
		open,
		closed
	};

	STATE door_state_;
	glm::vec2 position_;
public:
	Door();
	Door(glm::vec2 position);
	~Door();

	void SetDoorState();
};



#endif
