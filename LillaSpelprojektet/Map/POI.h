#ifndef POI_H
#define POI_H

#include <iostream>
#include <GLM/glm.hpp>
#include <vector>
#include <math.h>
//#include <time.h>
#include <ctime>

class POI {
private:
	std::vector<glm::vec2> points_of_interest_;
	std::vector<glm::vec2> zone_origin_coords_;
	int zone_radius_;
	glm::vec2 spawn_point_coords_;
	glm::vec2 boss_door_coords_;
	glm::vec2 door_key_coords_;
	glm::vec2 boss_room_coords_;
public:
	POI();
	~POI();
	void Initialize();

	std::vector<glm::vec2> fetchThreeRandomPOIs(int offset);
	glm::vec2 fetchRandomPOI();
	std::string getZone(glm::vec2 zone_central_points);

	std::vector<glm::vec2> getRemainingPOIs() const;
	std::vector<glm::vec2> getZonePOIs() const;
	glm::vec2 getSpawnPointCoords() const;
	glm::vec2 getBossDoorCoords() const;
	glm::vec2 getDoorKeyCoords() const;
	glm::vec2 getBossRoomCoords() const;
};

#endif