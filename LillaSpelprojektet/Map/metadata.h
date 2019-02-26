#ifndef METADATA_H
#define METADATA_H

#include <iostream>
#include <GLM/glm.hpp>
#include <vector>
#include <math.h>
#include <ctime>

//Point Of Interest
class MetaData {
private:
	std::vector<glm::vec2> points_of_interest_;
	std::vector<glm::vec2> zone_origin_coords_;
	int zone_radius_;
	glm::vec2 spawn_point_coords_;
	glm::vec2 boss_door_coords_;
	glm::vec2 door_key_coords_;
	glm::vec2 boss_room_coords_;
public:
	MetaData();
	~MetaData();
	void Initialize();

	std::vector<glm::vec2> FetchThreeRandomPOIs(int offset);
	glm::vec2 FetchRandomPOI();
	std::string GetZone(glm::vec2 zone_central_points);

	std::vector<glm::vec2> GetRemainingPOIs() const;
	std::vector<glm::vec2> GetZonePOIs() const;
	glm::vec2 GetSpawnPointCoords() const;
	glm::vec2 GetBossDoorCoords() const;
	glm::vec2 GetDoorKeyCoords() const;
	glm::vec2 GetBossRoomCoords() const;
};

#endif