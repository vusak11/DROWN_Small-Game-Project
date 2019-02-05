#include "game_state.h"
#include "state_handler.h"

GameState::GameState(StateHandler* handler)
	: state_handler_(handler) {

}

GameState::~GameState() {

}

void GameState::ChangeState(GameState* new_state) {
	state_handler_->ChangeState(new_state);
}