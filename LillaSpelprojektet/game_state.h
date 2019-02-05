#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Windows.h>

class StateHandler;	// ? ?? ? ? ?? que hora es ? ? ? ?? ? ?

class GameState {
protected:
	// Helper function to switch to a new active state.
	void ChangeState(GameState* new_state);

	// The state manager.
	StateHandler* state_handler_;
public:
	GameState(StateHandler* handler);
	virtual ~GameState();

	// The different 'events' functions. Child classes can 
	// implement the ones in which they are interested in.
	virtual void KeyDown(WPARAM);
	virtual void KeyUp(WPARAM);
	//virtual void OnChar(WPARAM);
	virtual void Update(DWORD);
	virtual void Draw();

	// Functions called when the state is entered or left
	// (transition from/to another state).
	virtual void EnterState();
	virtual void LeaveState();
};
#endif // !GAME_STATE_H