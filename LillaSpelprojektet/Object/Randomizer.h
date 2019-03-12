#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "../global_settings.h"
#include "Drop/Drop.h"
#include "../Map/metadata.h"

#include <cstdlib>
#include <ctime>
#include <string>

class Randomizer {
private:
	//Structs:
	struct DropRates {
		float hp_restore = 0.0f;
		float hp_up = 0.0f;
		float atk_up = 0.0f;
		float dash = 0.0f;
		float double_jump = 0.0f;
		float sword = 0.0f;
		float axe = 0.0f;
		float key = 0.0f;

		float sum_of_rates = 0.0f;
	};

	//Variables:
	DropRates* zone_rates_arr_;
	MetaData* meta_data_ptr_;

	//Functions:
	void LoadRates(ZoneID in_id);

public:
	Randomizer(MetaData* in_metadata_ptr);
	~Randomizer();

	Drop* RandomNewDropPtr(glm::vec3 in_pos, float in_drop_rate);
};


#endif // !RANDOMIZER_H
