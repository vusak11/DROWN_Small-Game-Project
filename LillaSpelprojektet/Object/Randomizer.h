#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "../global_settings.h"
#include "Drop/Drop.h"

#include <cstdlib>
#include <ctime>

class Randomizer {
private:
	ObjectID first_spawnable_drop_;
	ObjectID last_spawnable_drop_;
	int num_of_drops_;

public:
	Randomizer();
	~Randomizer();

	Drop* RandomNewDropPtr(glm::vec3 in_pos, float in_drop_rate);
};


#endif // !RANDOMIZER_H
