#include "metadata.h"

MetaData::MetaData() {

}

MetaData::~MetaData() {

}

void MetaData::Initialize() {
	points_of_interest_ = {
		glm::vec2(176, -86),
		glm::vec2(1990, -126),
		glm::vec2(75, -284),
		glm::vec2(525, -377),
		glm::vec2(1026, -525),
		glm::vec2(1530, -545),
		glm::vec2(46, -690),
		glm::vec2(2005, -712),
		glm::vec2(60, -823),
		glm::vec2(1013, -889),
		glm::vec2(1292, -891),
		glm::vec2(2005, -954),
		glm::vec2(60, -980),
		glm::vec2(640, -1235),
		glm::vec2(1177, -1239),
		glm::vec2(2011, -1267),
		glm::vec2(157, -1369),
		glm::vec2(1186, -1468),
		glm::vec2(1026, -525),
		glm::vec2(446, -1664)
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
			i = 1 + ((rand() % 5));			//START OVER ON NEW RANDOM VALUE
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

