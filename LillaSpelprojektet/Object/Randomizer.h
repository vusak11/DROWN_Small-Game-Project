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
		float hp_restore;
		float hp_up;
		float atk_up;
		float dash;
		float double_jump;
		float sword;
		float axe;
		float key;
	};

	//Variables:
	DropRates* zone_rates_arr_;
	const MetaData* meta_data_ptr_;

	//Functions:
	void LoadRates(ZoneID in_id);


	Drop* DropZoneDef(const float& in_verdict);
	Drop* DropZoneRed(const float& in_verdict);
	Drop* DropZoneGre(const float& in_verdict);
	Drop* DropZoneBlu(const float& in_verdict);

public:
	Randomizer(const MetaData* in_metadata_ptr);
	~Randomizer();

	Drop* RandomNewDropPtr(glm::vec3 in_pos, float in_drop_rate);
};


#endif // !RANDOMIZER_H
