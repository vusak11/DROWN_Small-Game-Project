#ifndef START_MENU_H
#define START_MENU_H


#include <SFML/Graphics.hpp>
#include "../Globals.h"
#include <SFML/OpenGL.hpp>
#include <GL/GLU.h>

#define MAX_NUMBER_OF_ITEMS 3

class StartMenu{
private:
	int selected_item_index_;

	sf::Font font_;
	sf::Text menu_item_[MAX_NUMBER_OF_ITEMS];
public:
	StartMenu();
	~StartMenu();

	void Draw(sf::RenderWindow &window);
	void NavigateUp();
	void NavigateDown();
};
#endif