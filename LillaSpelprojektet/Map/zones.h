#ifndef ZONES_H
#define ZONES_H

#include <iostream>
#include <GLM/glm.hpp>
#include <vector>
//#include <math.h>
//#include <time.h>
#include <ctime>

class Zones {
private:
	std::vector<glm::vec2> points_of_interest_;
	int nr_of_pois_;
public:
	Zones();
	~Zones();
	std::vector<glm::vec2> getRandomPOIs();
	std::vector<glm::vec2> getPOIs() const;
};

#endif