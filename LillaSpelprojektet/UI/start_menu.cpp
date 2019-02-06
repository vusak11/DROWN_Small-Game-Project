#include "start_menu.h"

StartMenu::StartMenu() {
	if (!font_.loadFromFile("../Resources/Text/alittlepot.ttf")) {
		//error
	}
	sf::Color colour(255, 0, 0);

	menu_item_[0].setFont(font_);
	menu_item_[0].setFillColor(colour);
	menu_item_[0].setString("Start");
	menu_item_[0].setPosition(sf::Vector2f((WINDOW_WIDTH / 2) - (menu_item_->getLocalBounds().width / 2),
		WINDOW_HEIGHT / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	menu_item_[1].setFont(font_);
	menu_item_[1].setFillColor(colour);
	menu_item_[1].setString("Settings");
	menu_item_[1].setPosition(sf::Vector2f((WINDOW_WIDTH / 2) - (menu_item_->getLocalBounds().width / 2),
		WINDOW_HEIGHT / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	menu_item_[2].setFont(font_);
	menu_item_[2].setFillColor(colour);
	menu_item_[2].setString("Quit");
	menu_item_[2].setPosition(sf::Vector2f((WINDOW_WIDTH / 2) - (menu_item_->getLocalBounds().width / 2),
		WINDOW_HEIGHT / (MAX_NUMBER_OF_ITEMS + 1) * 3));
}

StartMenu::~StartMenu() {
}

void StartMenu::Draw(sf::RenderWindow &window) {
	//glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	for (int i = 0; i = MAX_NUMBER_OF_ITEMS; i++) {
		window.draw(menu_item_[i]);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void StartMenu::NavigateUp() {
}

void StartMenu::NavigateDown() {
}
