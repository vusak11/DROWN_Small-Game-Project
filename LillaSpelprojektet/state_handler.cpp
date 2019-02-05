#include "state_handler.h"

StateHandler::StateHandler() {
}

StateHandler::~StateHandler() {
}

void StateHandler::ChangeState(GameState * new_state) {
	if (active_state_)
		active_state_->LeaveState();
	active_state_ = new_state;
	active_state_->EnterState();
}

GameState * StateHandler::GetActiveState() const {
	return active_state_;
}

void StateHandler::KeyDown(WPARAM key) {
	if (active_state_)
		active_state_->KeyDown(key);
}

void StateHandler::KeyUp(WPARAM key) {
	if (active_state_)
		active_state_->KeyUp(key);
}

void StateHandler::Update(float in_deltatime) {
	if (active_state_)
		active_state_->Update(in_deltatime);
}

void StateHandler::Draw() {
	if (active_state_)
		active_state_->Draw();
}
