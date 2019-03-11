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
	std::vector<glm::vec2> light_positions_;
	std::vector<glm::vec2> door_key_coords_;
	int zone_radius_;
	
	glm::vec2 spawn_point_coords_;
	glm::vec2 boss_door_coords_;
public:
	MetaData();
	~MetaData();
	void Initialize();

	std::vector<glm::vec2> FetchThreeRandomPOIs(int offset);
	glm::vec2 FetchRandomPOI();
	std::string GetZone(glm::vec2 zone_central_points);
	glm::vec2 FetchSpawnPoint();

	std::vector<glm::vec2> GetRemainingPOIs() const;
	std::vector<glm::vec2> GetZonePOIs() const;
	std::vector<glm::vec2> GetLightPositions() const;

	glm::vec2 GetSpawnPointCoords() const;
	glm::vec2 GetBossDoorCoords() const;
	std::vector<glm::vec2> GetDoorKeyCoords() const;
};

#endif