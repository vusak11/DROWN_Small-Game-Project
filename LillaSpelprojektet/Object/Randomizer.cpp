#include "randomizer.h"

//Private--------------------------------------------------
void Randomizer::LoadRates(ZoneID in_id) {

	//Use the ID to get the string needed to access the global value
	std::string zone_str = "";
	switch (in_id) {
	case DEF:
		zone_str = "DEF";
		break;
	case RED:
		zone_str = "RED";
		break;
	case GRE:
		zone_str = "GRE";
		break;
	case BLU:
		zone_str = "BLU";
		break;
	default:
		break;
	}

	//Load in the value from the globals
	this->zone_rates_arr_[in_id].hp_restore		= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_HP_RESTORE");
	this->zone_rates_arr_[in_id].hp_up			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_HP_UP");
	this->zone_rates_arr_[in_id].atk_up			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_ATK_UP");
	this->zone_rates_arr_[in_id].spd_up			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_SPD_UP");
	this->zone_rates_arr_[in_id].dash			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_DASH");
	this->zone_rates_arr_[in_id].double_jump	= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_DOUBLE_JUMP");
	this->zone_rates_arr_[in_id].sword			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_SWORD");
	this->zone_rates_arr_[in_id].axe			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_AXE");
	this->zone_rates_arr_[in_id].key			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_KEY");

	this->zone_rates_arr_[in_id].sum_of_rates =
		this->zone_rates_arr_[in_id].hp_restore
		+ this->zone_rates_arr_[in_id].hp_up
		+ this->zone_rates_arr_[in_id].atk_up
		+ this->zone_rates_arr_[in_id].spd_up
		+ this->zone_rates_arr_[in_id].dash
		+ this->zone_rates_arr_[in_id].double_jump
		+ this->zone_rates_arr_[in_id].sword
		+ this->zone_rates_arr_[in_id].axe
		+ this->zone_rates_arr_[in_id].key;

}


//Public---------------------------------------------------
Randomizer::Randomizer(MetaData* in_metadata_ptr) {
	//Initiate the random seed
	//NOTE:	This should preferably be done nowhere else in the program
	//		As of now the zone generation does it twice before this part
	//		of the code
	//srand(static_cast<unsigned>(time(0)));
	//EDIT: Now done with a call to global settings from main

	//Allocate space for drop rates for each zone
	this->zone_rates_arr_ = new DropRates[NUM_OF_ZONES];

	//Load in the drop rates for each zone from the globals
	this->LoadRates(DEF);
	this->LoadRates(RED);
	this->LoadRates(GRE);
	this->LoadRates(BLU);

	//Save the MetaData pointer
	this->meta_data_ptr_ = in_metadata_ptr;
	
}

Randomizer::~Randomizer() {
	delete this->zone_rates_arr_;
}

float Randomizer::RandomizeFloat(float lower_bound, float upper_bound) {
	return lower_bound
		+ (static_cast<float>(GlobalSettings::GetRandomInt())
			/ static_cast<float>(RAND_MAX 
				/ (upper_bound - lower_bound)
				)
			)
		;
}

Drop* Randomizer::RandomNewDropPtr(glm::vec3 in_pos, float in_drop_rate) {
	//NOTE 1:
	//Note that this pointer returns a pointer to a Drop object
	//The Randomizer DOES NOT delete that object upon being deleted
	//itself but rather leaves that responsibility to the caller
	
	//NOTE 2:
	//This function can return a null pointer. Ensure to handle outside

	//Get a random number in the range 0 - 100
	float verdict = this->RandomizeFloat(0.0f, 100.0f);

	//If the verdict is higher than the drop rate value, return null
	if (verdict > in_drop_rate) { return NULL; }
	
	//If the verdict is lower than the drop rate value we should return
	//a drop. 
	//Since we do not want to generate a new random number so close to
	//the last we scale up the verdict from
	//			[0.0, drop_rate]
	//to
	//			[0.0, sum_of_all_rates]
	//by multiplying it sum_of_all_rates/drop_rate (dr * x = sor	->	x = sor/dr)
	ZoneID zone_id = this->meta_data_ptr_->GetZone(in_pos);
	
	//NTS: What is best?

	//ALT 1:	Not calling for a new rand() within a set time span of the last call
	float sum_of_all_rates = this->zone_rates_arr_[zone_id].sum_of_rates;
	verdict *= (sum_of_all_rates / in_drop_rate);	//NTS: Inaccuracy rises with smaller drop rates
	
	//ALT 2:	Not dilating a low verdict when in_drop_rate is low
	//verdict = static_cast<float>(rand()) / static_cast<float>(RAND_MAX/100);

	//

	//We now go through the zone's drop rates to see what drop is to be spawned.
	//We will check if the "new" verdict is lower than the rate of a specific
	//drop in that zone.
	//	NOTE:	We are checking if the rate is LOWER. This means that
	//			a drop with 0% spawn rate will not be created
	//After we have determined the verdict to not be lower than that drop rate
	//we add the drop rate of the next drop on top of the old one and check again.
	//This will practically let us examine a range without defining a lower bound
	//as we already know the drop to not be in any ranges below what we are checking
	//	NOTE:	Given that we check "lower than" the max of the previous
	//			range becomes the 0 of the current one
	
	
	
	float range_max = 0.0;

	range_max += this->zone_rates_arr_[zone_id].hp_restore;
	if (verdict < range_max) {
		return new HPRestoreDrop(in_pos);
	}

	range_max += this->zone_rates_arr_[zone_id].hp_up;
	if (verdict < range_max) {
		return new HPUpDrop(in_pos);
	}

	range_max += this->zone_rates_arr_[zone_id].atk_up;
	if (verdict < range_max) {
		return new AtkUpDrop(in_pos);
	}

	range_max += this->zone_rates_arr_[zone_id].spd_up;
	if (verdict < range_max) {
		return new SpdUpDrop(in_pos);
	}

	range_max += this->zone_rates_arr_[zone_id].dash;
	if (verdict < range_max) {
		return new DashDrop(in_pos);
	}

	range_max += this->zone_rates_arr_[zone_id].double_jump;
	if (verdict < range_max) {
		return new DoubleJumpDrop(in_pos);
	}

	range_max += this->zone_rates_arr_[zone_id].sword;
	if (verdict < range_max) {
		return new SwordDrop(in_pos);
	}
	
	range_max += this->zone_rates_arr_[zone_id].axe;
	if (verdict < range_max) {
		return new AxeDrop(in_pos);
	}

	range_max += this->zone_rates_arr_[zone_id].key;
	if (verdict < range_max) {
		return new KeyDrop(in_pos);
	}


	
	
	//Return drop pointer, if the drop was not propernly represented this
	//might reurn null
	return NULL;
}