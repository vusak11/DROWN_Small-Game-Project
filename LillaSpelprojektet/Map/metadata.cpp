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
	light_positions_ = {
		//Special lights
		glm::vec2(0, 0),		// Player
		glm::vec2(1232, -550),	// Danger light at the throat of the dungeon
		glm::vec2(160, -1100),	// Boss attack light
		//POIs
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
		glm::vec2(460, -1650),
		//Light-only pos
		glm::vec2(945, -1640),
		glm::vec2(1050, -1550),
		glm::vec2(115, -1630),
		glm::vec2(665, -1485),
		glm::vec2(280, -1480),
		glm::vec2(50, -1390),
		glm::vec2(420, -1360),
		glm::vec2(800, -1342),
		glm::vec2(995, -1242),
		glm::vec2(1955, -1575),
		glm::vec2(1550, -1455),
		glm::vec2(1295, -1370),
		glm::vec2(1995, -1370),
		glm::vec2(1785, -1260),
		glm::vec2(1370, -1220),
		glm::vec2(1825, -1080),
		glm::vec2(1575, -1075),
		glm::vec2(1675, -920),
		glm::vec2(1100, -1010),
		glm::vec2(810, -1033),
		glm::vec2(770, -865),
		glm::vec2(415, -920),
		glm::vec2(1035, -710),
		glm::vec2(613, -680),
		glm::vec2(407, -745),
		glm::vec2(230, -715),
		glm::vec2(275, -540),
		glm::vec2(885, -350),
		glm::vec2(400, -365),
		glm::vec2(270, -335),
		glm::vec2(1585, -730),
		glm::vec2(1890, -570),
		glm::vec2(1365, -515),
		glm::vec2(1860, -350),
		glm::vec2(1460, -310),
		glm::vec2(1620, -65),
		glm::vec2(1132, -162),
		glm::vec2(913, -150),
		glm::vec2(535, -160),
		glm::vec2(310, -200)
	};
	zone_radius_ = 400;
	zone_origin_coords_ = FetchThreeRandomPOIs(zone_radius_*2);
	spawn_point_coords_ = FetchSpawnPoint();
	boss_door_coords_ = FetchRandomPOI();
	door_key_coords_ = FetchThreeRandomPOIs(zone_radius_);
}

std::vector<glm::vec2> MetaData::FetchThreeRandomPOIs(int offset) {
	srand((unsigned)time(0));
	int rand_num = ((rand() % (points_of_interest_.size() + 1)));		//Random number 0 - nr_of_pois_
	int rand_iterator = 1 + ((rand() % 5));								//Random number 1 - 5 used as an iterator

	std::vector<glm::vec2> three_random_pois;

	for (int i = rand_num; three_random_pois.size() < 3; i += rand_iterator) {
		//IF OUT OF BOUNDS
		if (i >= points_of_interest_.size()) {
			i = 1 + ((rand() % 5));			//START OVER ON NEW RANDOM VALUE BETWEEN 1 - 5
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
	int rand_num = ((rand() % (points_of_interest_.size() - 1)));		//Random number 0 - nr_of_pois_

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
		zone_code = "GRE";
	}
	//Check if point is within radius of the SOL ZONE
	else if (pow(zone_central_points.x - zone_origin_coords_[2].x, 2) + pow(zone_central_points.y - zone_origin_coords_[2].y, 2) <= pow(zone_radius_, 2)) {
		zone_code = "BLU";
	}
	return zone_code;
}

glm::vec2 MetaData::FetchSpawnPoint() {
	srand((unsigned)time(0));
	int rand_num = ((rand() % (points_of_interest_.size() - 1)));		//Random number 0 - nr_of_pois_
	glm::vec2 return_POI;

	for (int i = rand_num; i < points_of_interest_.size(); i++) {
		//IF OUT OF BOUNDS
		if (i >= points_of_interest_.size()) {
			i = 1 + ((rand() % (points_of_interest_.size() - 1)));			//START OVER ON NEW RANDOM VALUE BETWEEN 1 - 5
		}
		
		if (GetZone(points_of_interest_[i]) == "DEF") {
			return_POI = points_of_interest_[i];
			points_of_interest_.erase(points_of_interest_.begin() + i);		//Erase coords from list of available POIs
			return return_POI;
		}
	}
}

std::vector<glm::vec2> MetaData::GetRemainingPOIs() const {
	return points_of_interest_;
}

std::vector<glm::vec2> MetaData::GetZonePOIs() const
{
	return zone_origin_coords_;
}

std::vector<glm::vec2> MetaData::GetLightPositions() const
{
	return light_positions_;
}

glm::vec2 MetaData::GetSpawnPointCoords() const
{
	return spawn_point_coords_;
}

glm::vec2 MetaData::GetBossDoorCoords() const
{
	return boss_door_coords_;
}

std::vector<glm::vec2> MetaData::GetDoorKeyCoords() const
{
	return door_key_coords_;
}