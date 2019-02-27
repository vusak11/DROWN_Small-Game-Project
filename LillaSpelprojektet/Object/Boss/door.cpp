#include "door.h"

Door::Door() {
	door_state_ = closed;
}

Door::Door(glm::vec2 position) : ObjectClass(glm::vec3(position.x, position.y, 0.0f), OBJECT_ID_DOOR){
	position_ = position;
}

Door::~Door() {

}

void Door::SetDoorState() {
	door_state_ = open;
}