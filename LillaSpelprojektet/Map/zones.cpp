#include "zones.h"

Zones::Zones() {
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
	nr_of_pois_ = points_of_interest_.size();
	//Print out list of POIs
	/*for (std::vector<glm::vec2>::size_type i = 0; i != points_of_interest_.size(); i++) {
		std::cout << "X: " << points_of_interest_[i].x << " | " << "Y: " << points_of_interest_[i].y << std::endl;
	}*/
}

Zones::~Zones() {
	
}

std::vector<glm::vec2> Zones::getRandomPOIs() {
	srand((unsigned)time(0));
	int rand_num = ((rand() % (nr_of_pois_ + 1)));		//Random number 0 - nr_of_pois_
	int rand_iterator = 1 + ((rand() % 5));				//Random number 1 - 5 used as an iterator

	std::vector<glm::vec2> return_list;

	for (std::vector<glm::vec2>::size_type i = rand_num; return_list.size() != 3; i + rand_iterator) {
		//IF OUT OF BOUNDS
		if (i >= nr_of_pois_) {
			i = 0;						//START OVER
		}
		//ELSE IF RETURN LIST IS EMPTY
		else if (return_list.size() == 0) {
			return_list.push_back(points_of_interest_[i]);
		}
		//Check if coordinate distances are greater than the desired offset
		/*else if (return_list[i].x ) {

		}*/
	}

	return return_list;
}

std::vector<glm::vec2> Zones::getPOIs() const {
	return points_of_interest_;
}
