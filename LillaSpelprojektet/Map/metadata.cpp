#include "metadata.h"

MetaData::MetaData() {

}

MetaData::~MetaData() {

}

void MetaData::Initialize() {
	points_of_interest_ = {
		glm::vec2(160, -70),
		glm::vec2(1995, -115),
		glm::vec2(80, -270),
		glm::vec2(520, -360),
		glm::vec2(1025, -515),
		glm::vec2(1515, -530),
		glm::vec2(60, -680),
		glm::vec2(2005, -700),
		glm::vec2(50, -810),
		glm::vec2(1020, -875),
		glm::vec2(1300, -875),
		glm::vec2(2010, -935),
		glm::vec2(50, -970),
		glm::vec2(645, -1220),
		glm::vec2(1130, -1225),
		glm::vec2(2015, -1255),
		glm::vec2(165, -1350),
		glm::vec2(1190, -1455),
		glm::vec2(1025, -515),
		glm::vec2(460, -1650)
	};
	zone_origin_coords_ = FetchThreeRandomPOIs(800);
	zone_radius_ = 400;

	spawn_point_coords_ = FetchRandomPOI();
	boss_door_coords_ = FetchRandomPOI();
	door_key_coords_ = FetchRandomPOI();
	boss_room_coords_ = glm::vec2(60, -1250);
}

std::vector<glm::vec2> MetaData::FetchThreeRandomPOIs(int offset) {
	srand((unsigned)time(0));
	int rand_num = ((rand() % (points_of_interest_.size() + 1)));		//Random number 0 - nr_of_pois_
	int rand_iterator = 1 + ((rand() % 5));								//Random number 1 - 5 used as an iterator

	std::vector<glm::vec2> three_random_pois;

	for (int i = rand_num; three_random_pois.size() < 3; i += rand_iterator) {
		//IF OUT OF BOUNDS
		if (i >= points_of_interest_.size()) {
			i = ((rand() % (points_of_interest_.size() + 1)));			//START OVER ON NEW RANDOM VALUE
		}
		//IF RETURN LIST IS EMPTY
		else if (three_random_pois.size() == 0) {
			three_random_pois.push_back(points_of_interest_[i]);			//Add coords to the list of zone POIs
			points_of_interest_.erase(points_of_interest_.begin() + i);		//Erase coords from list of available POIs
		}

		else if (three_random_pois.size() > 0) {
			//Compare the current POI with any POIs in the output list
			if (three_random_pois.size() == 1) {
				//Check if POI distances are greater than the desired offset
				if (sqrt(pow((three_random_pois[0].x - points_of_interest_[i].x), 2) + pow((three_random_pois[0].y - points_of_interest_[i].y), 2)) > offset) {
					three_random_pois.push_back(points_of_interest_[i]);			//Add coords to the list of zone POIs
					points_of_interest_.erase(points_of_interest_.begin() + i);		//Erase coords from list of available POIs
				}
			}
			else if (three_random_pois.size() == 2) {
				//Check if POI distances are greater than the desired offset
				if (sqrt(pow((three_random_pois[0].x - points_of_interest_[i].x), 2) + pow((three_random_pois[0].y - points_of_interest_[i].y), 2)) > offset &&
					sqrt(pow((three_random_pois[1].x - points_of_interest_[i].x), 2) + pow((three_random_pois[1].y - points_of_interest_[i].y), 2)) > offset) {
					three_random_pois.push_back(points_of_interest_[i]);			//Add coords to the list of zone POIs
					points_of_interest_.erase(points_of_interest_.begin() + i);		//Erase coords from list of available POIs
				}
			}
		}
	}
	return three_random_pois;
}

glm::vec2 MetaData::FetchRandomPOI() {
	srand((unsigned)time(0));
	int rand_num = ((rand() % (points_of_interest_.size() + 1)));		//Random number 0 - nr_of_pois_

	glm::vec2 return_POI = points_of_interest_[rand_num];
	points_of_interest_.erase(points_of_interest_.begin() + rand_num);		//Erase coords from list of available POIs

	return return_POI;
}

std::string MetaData::GetZone(glm::vec2 zone_central_points) {
	std::string zone_code = "DEF";
	//Check if point is within radius of the RED ZONE
	if (pow(zone_central_points.x - zone_origin_coords_[0].x, 2) + pow(zone_central_points.y - zone_origin_coords_[0].y, 2) <= pow(zone_radius_, 2)) {
		zone_code = "RED";
	}
	//Check if point is within radius of the BLU ZONE
	else if (pow(zone_central_points.x - zone_origin_coords_[1].x, 2) + pow(zone_central_points.y - zone_origin_coords_[1].y, 2) <= pow(zone_radius_, 2)) {
		zone_code = "BLU";
	}
	//Check if point is within radius of the SOL ZONE
	else if (pow(zone_central_points.x - zone_origin_coords_[2].x, 2) + pow(zone_central_points.y - zone_origin_coords_[2].y, 2) <= pow(zone_radius_, 2)) {
		zone_code = "SOL";
	}
	return zone_code;
}

std::vector<glm::vec2> MetaData::GetRemainingPOIs() const {
	return points_of_interest_;
}

std::vector<glm::vec2> MetaData::GetZonePOIs() const
{
	return zone_origin_coords_;
}

glm::vec2 MetaData::GetSpawnPointCoords() const
{
	return spawn_point_coords_;
}

glm::vec2 MetaData::GetBossDoorCoords() const
{
	return boss_door_coords_;
}

glm::vec2 MetaData::GetDoorKeyCoords() const
{
	return door_key_coords_;
}

glm::vec2 MetaData::GetBossRoomCoords() const {
	return boss_room_coords_;
}
