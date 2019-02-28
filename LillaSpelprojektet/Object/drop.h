#ifndef DROP_H
#define DROP_H

#include "object_class.h"

class Drop : public ObjectClass {
private:
	enum STATE {
		open,
		closed
	};

	STATE door_state_;
	glm::vec2 position_;
public:
	Drop();
	Drop(glm::vec2 position);
	~Drop();

	void SetDoorState();
};



#endif