#include "randomizer.h"

//Private--------------------------------------------------

//Public---------------------------------------------------
Randomizer::Randomizer() {
	//Initiate the random seed
	//NOTE:	This should preferably be done nowhere else in the program
	//		As of now the zone generation does it twice before this part
	//		of the code
	srand(static_cast<unsigned>(time(0)));

	//Set the range of randomization for drops
	this->first_spawnable_drop_ = OBJECT_ID_DROP_HP_RESTORE;
	this->last_spawnable_drop_ = OBJECT_ID_DROP_KEY;
	int num_of_drops = this->last_spawnable_drop_ - this->first_spawnable_drop_ + 1;

	//Calculate the number of spawnable drops
	this->num_of_drops_ = this->last_spawnable_drop_ - this->first_spawnable_drop_ + 1;

	if (this->num_of_drops_ < 0) { this->num_of_drops_ = 0; }
}

Randomizer::~Randomizer() {

}

Drop* Randomizer::RandomNewDropPtr(glm::vec3 in_pos, float in_drop_rate) {
	//NOTE 1:
	//Note that this pointer returns a pointer to a Drop object
	//The Randomizer DOES NOT delete that object upon being deleted
	//itself but rather leaves that responsibility to the caller
	
	//NOTE 2:
	//This function can return a null pointer. Ensure to handle outside
	
	//Avoid division on 0
	if (this->num_of_drops_ == 0) { return NULL; }

	//Split the drop rate range into that many equally sized sections
	float range_per_drop = in_drop_rate / (float)this->num_of_drops_;

	//Get a random number in the range 0 - 100
	float verdict = static_cast<float>(rand()) / static_cast<float>(100);

	//If the verdic is higher than the drop rate, return null
	if (verdict < in_drop_rate) { return NULL; }
	
	//If the verdic is lower it means we should spawn a drop
	//Loop over all ranges to figure out which one it belongs to
	//We start from the lowest and increase for each iteration,
	//meaning we only need to check if the verdic is lower than
	//the next range maximum
	Drop* drop_ptr = NULL;
	for (unsigned int i = 1; i <= this->num_of_drops_; i++) {
		if (verdict < range_per_drop * i) {

			//drop_ptr = Call to drop selection function()
			//WORKING HERE

			//Break loop
			i = this->num_of_drops_ + 1;
		}
	}
	
	//Return drop pointer, if the drop was not propernly represented this
	//might reurn null
	return drop_ptr;
}