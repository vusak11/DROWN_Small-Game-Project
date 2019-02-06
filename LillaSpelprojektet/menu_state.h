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

	// GameFont variable

	//Currently selected menu alternative
	int selection_index_;

	// A pointer to the current active game (if any).

	// TEXT VARIABLES(?)
public:
	~MenuState();

	void KeyDown(WPARAM key);
	void Draw();
	void EnterState();

	static MenuState* GetInstance(StateHandler* handler);
};


#endif