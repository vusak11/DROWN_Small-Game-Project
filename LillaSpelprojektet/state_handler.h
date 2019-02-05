#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include "game_state.h"

class StateHandler {
private:
	GameState* active_state_;
public:
	StateHandler ();
	~StateHandler ();
	void ChangeState(GameState* new_state);
	GameState* GetActiveState() const;

	void KeyDown(WPARAM key);
	void KeyUp(WPARAM key);
	void Update(float in_deltatime);
	void Draw();
};

#endif