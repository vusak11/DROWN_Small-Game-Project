#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "game_state.h"
#include <GL\gl.h>

class MenuState : public GameState {
protected:
	MenuState(StateHandler* handler);
private:
	void NavUp();
	void NavDown();
	void Select();

	//GameFont variable
	int selection_index;


public:
};


#endif