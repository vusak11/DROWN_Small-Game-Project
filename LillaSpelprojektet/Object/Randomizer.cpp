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

	//LOad in the value from the globals
	this->zone_rates_arr_[in_id].hp_restore		= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_HP_RESTORE");
	this->zone_rates_arr_[in_id].hp_up			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_HP_UP");
	this->zone_rates_arr_[in_id].atk_up			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_ATK_UP");
	this->zone_rates_arr_[in_id].dash			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_DASH");
	this->zone_rates_arr_[in_id].double_jump	= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_DOUBLE_JUMP");
	this->zone_rates_arr_[in_id].sword			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_SWORD");
	this->zone_rates_arr_[in_id].axe			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_AXE");
	this->zone_rates_arr_[in_id].key			= GlobalSettings::Access()->ValueOf(zone_str + "_ZONE_DROP_RATE_KEY");

}

Drop* Randomizer::DropZoneDef(const float& in_verdict) {
	
}

Drop* Randomizer::DropZoneRed(const float& in_verdict) {

}

Drop* Randomizer::DropZoneGre(const float& in_verdict) {

}

Drop* Randomizer::DropZoneBlu(const float& in_verdict) {

}


//Public---------------------------------------------------
Randomizer::Randomizer(MetaData* in_metadata_ptr) {
	//Initiate the random seed
	//NOTE:	This should preferably be done nowhere else in the program
	//		As of now the zone generation does it twice before this part
	//		of the code
	srand(static_cast<unsigned>(time(0)));

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

Drop* Randomizer::RandomNewDropPtr(glm::vec3 in_pos, float in_drop_rate) {
	//NOTE 1:
	//Note that this pointer returns a pointer to a Drop object
	//The Randomizer DOES NOT delete that object upon being deleted
	//itself but rather leaves that responsibility to the caller
	
	//NOTE 2:
	//This function can return a null pointer. Ensure to handle outside

	//Get a random number in the range 0 - 100
	float verdict = static_cast<float>(rand()) / static_cast<float>(100);

	//If the verdict is higher than the drop rate value, return null
	if (verdict > in_drop_rate) { return NULL; }
	
	//If the verdict is lower than the drop rate value we should return
	//a drop. 
	//Since we do not want to generate a new random number so close to
	//the last we scale up the verdict from
	//			[0.0, drop_rate]
	//to
	//			[0.0, 100.0]
	//by multiplying it by 100.0/drop_rate (drop_rate * x = 100	->	x = 100/drop_rate)
	verdict *= 100.0f / in_drop_rate;			//NTS: If this doesn't work
												//just generate a new verdict here
												//Inaccuracy rises with smaller drop rates

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
	
	
	ZoneID zone_id = this->meta_data_ptr_->GetZone(in_pos);
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
	return drop_ptr;
}