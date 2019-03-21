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

	monster_spawn_bonus_ = {
		glm::vec2(284,-137),
		glm::vec2(667,-114),
		glm::vec2(1945,-215),
		glm::vec2(1900,-290),
		glm::vec2(1628,-258),
		glm::vec2(1480,-390),
		glm::vec2(1160,-330),
		glm::vec2(1044,-405),
		glm::vec2(237,-258),
		glm::vec2(149,-481),
		glm::vec2(570,-460),
		glm::vec2(335,-643),
		glm::vec2(585,-616),
		glm::vec2(903,-624),
		glm::vec2(235,-866),
		glm::vec2(710,-954),
		glm::vec2(1060,-935),
		glm::vec2(1413,-636),
		glm::vec2(1620,-593),
		glm::vec2(1765,-698),
		glm::vec2(1980,-760),
		glm::vec2(1828,-966),
		glm::vec2(1395,-995),
		glm::vec2(1527,-1153),
		glm::vec2(1847,-1183),
		glm::vec2(1544,-1218),
		glm::vec2(1580,-1318),
		glm::vec2(1538,-1583),
		glm::vec2(1800,-1600),
		glm::vec2(888,-1247),
		glm::vec2(741,-1253),
		glm::vec2(516,-1329),
		glm::vec2(1015,-1460),
		glm::vec2(490,-1575),
		glm::vec2(327,-1340),
		glm::vec2(169,-1447),
		glm::vec2(63,-1978),
		glm::vec2(126,-1835),
		glm::vec2(195,-1932),
		glm::vec2(247,-1787),
		glm::vec2(312,-1807),
		glm::vec2(330,-1924),
		glm::vec2(436,-1850),
		glm::vec2(524,-1995),
		glm::vec2(584,-1923),
		glm::vec2(717,-1881),
		glm::vec2(743,-2003),
		glm::vec2(832,-1821),
		glm::vec2(905,-1980),
		glm::vec2(990,-1930),
		glm::vec2(992,-1775),
		glm::vec2(1080,-1983),
		glm::vec2(1160,-1877),
		glm::vec2(1321,-1817),
		glm::vec2(1352,-1985),
		glm::vec2(1418,-1907),
		glm::vec2(1444,-1735),
		glm::vec2(1540,-1820),
		glm::vec2(1544,-1966),
		glm::vec2(1640,-1965),
		glm::vec2(1700,-1835),
		glm::vec2(1827,-1914),
		glm::vec2(1846,-1755),
		glm::vec2(1920,-1975),
		glm::vec2(1990,-1890)
	};

	chest_spawn_bonus_ = {
		glm::vec2(62,-1996),
		glm::vec2(495,-1940),
		glm::vec2(630,-2020),
		glm::vec2(1005,-2015),
		glm::vec2(1163,-2015),
		glm::vec2(1570,-1980),
		glm::vec2(1627,-1674),
		glm::vec2(1804,-1932)
	};

	zone_radius_ = 400;
	zone_origin_coords_ = FetchThreeRandomPOIs(zone_radius_*2);
	spawn_point_coords_ = FetchSpawnPoint();
	boss_door_coords_ = FetchRandomPOI();
	door_key_coords_ = FetchThreeRandomPOIs(zone_radius_);
}

std::vector<glm::vec2> MetaData::FetchThreeRandomPOIs(int offset) {

	int rand_num = ((GlobalSettings::GetRandomInt() % (points_of_interest_.size() + 1)));		//Random number 0 - nr_of_pois_
	int rand_iterator = 1 + ((GlobalSettings::GetRandomInt() % 5));								//Random number 1 - 5 used as an iterator

	std::vector<glm::vec2> three_random_pois;

	for (unsigned int i = rand_num; three_random_pois.size() < 3; i += rand_iterator) {
		//IF OUT OF BOUNDS
		if (i >= points_of_interest_.size()) {
			i = 1 + ((GlobalSettings::GetRandomInt() % 5));			//START OVER ON NEW RANDOM VALUE BETWEEN 1 - 5
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

	int rand_num = ((GlobalSettings::GetRandomInt() % (points_of_interest_.size() - 1)));		//Random number 0 - nr_of_pois_

	glm::vec2 return_POI = points_of_interest_[rand_num];
	points_of_interest_.erase(points_of_interest_.begin() + rand_num);		//Erase coords from list of available POIs

	return return_POI;
}

ZoneID MetaData::GetZone(glm::vec2 in_pos) {
	//Checks which zone the argument lies in and returns its ID

	ZoneID zone_code = DEF;

	//Check if point is within radius of the RED ZONE
	if (pow(in_pos.x - zone_origin_coords_[0].x, 2) + pow(in_pos.y - zone_origin_coords_[0].y, 2) <= pow(zone_radius_, 2)) {
		zone_code = RED;
	}
	//Check if point is within radius of the BLU ZONE
	else if (pow(in_pos.x - zone_origin_coords_[1].x, 2) + pow(in_pos.y - zone_origin_coords_[1].y, 2) <= pow(zone_radius_, 2)) {
		zone_code = GRE;
	}
	//Check if point is within radius of the SOL ZONE
	else if (pow(in_pos.x - zone_origin_coords_[2].x, 2) + pow(in_pos.y - zone_origin_coords_[2].y, 2) <= pow(zone_radius_, 2)) {
		zone_code = BLU;
	}
	return zone_code;
}

glm::vec2 MetaData::FetchSpawnPoint() {
	
	glm::vec2 return_POI = glm::vec2(0.0f);


	int rand_num = 0;
	while (return_POI.x == 0.0f) {
		rand_num = ((GlobalSettings::GetRandomInt() % (points_of_interest_.size())));		//Random number 0 - nr_of_pois_

		if (GetZone(points_of_interest_[rand_num]) == DEF) {
			return_POI = points_of_interest_[rand_num];
			points_of_interest_.erase(points_of_interest_.begin() + rand_num);		//Erase coords from list of available POIs
		}
	}

	
	return return_POI;
}

std::vector<glm::vec2> MetaData::GetRemainingPOIs() const {
	return points_of_interest_;
}

std::vector<glm::vec2> MetaData::GetZonePOIs() const {
	return zone_origin_coords_;
}

std::vector<glm::vec2> MetaData::GetLightPositions() const {
	return light_positions_;
}

std::vector<glm::vec2> MetaData::GetMonsterPositionsBonus() const {
	return monster_spawn_bonus_;
}

std::vector<glm::vec2> MetaData::GetChestPositionsBonus() const {
	return chest_spawn_bonus_;
}

glm::vec2 MetaData::GetSpawnPointCoords() const {
	return spawn_point_coords_;
}

glm::vec2 MetaData::GetBossDoorCoords() const {
	return boss_door_coords_;
}

std::vector<glm::vec2> MetaData::GetDoorKeyCoords() const {
	return door_key_coords_;
}