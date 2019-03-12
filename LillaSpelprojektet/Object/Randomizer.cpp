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
Randomizer::Randomizer() {
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

	//If the verdic is higher than the drop rate, return null
	if (verdict > in_drop_rate) { return NULL; }
	
	//Otherwise determine a drop using the drop functions
	//Each of these functions start with the lowest range
	//by checking if the verdic is below its maximum.
	//If it isn't we can simply increase the maximum to cover
	//the next range while practically excluding the first
	//(as we know there to be no match).
	Drop* drop_ptr = NULL;
	switch (zone) {
	case "RED":
		drop_ptr = this->DropZoneRed(verdict);
		break;
	case "GRE":
		drop_ptr = this->DropZoneBlu(verdict);
		break;
	case "BLU":
		drop_ptr = this->DropZoneGre(verdict);
		break;
	default:
		drop_ptr = this->DropZoneDef(verdict);
		break;
	}
	
	
	//Return drop pointer, if the drop was not propernly represented this
	//might reurn null
	return drop_ptr;
}