#include "drop.h"

Drop::Drop() {
	door_state_ = closed;
}

Drop::Drop(glm::vec2 position) : ObjectClass(glm::vec3(position.x, position.y, 0.0f), OBJECT_ID_DROP) {
	position_ = position;
}

Drop::~Drop() {

}

void Drop::SetDoorState() {
	door_state_ = open;
}