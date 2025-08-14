#include "Engine/Input.hpp"

namespace lite2d {

void Input::mapAction(const std::string& action, SDL_Scancode key) {
	actionToKey[action] = key;
	if (!keyStates.count(key)) keyStates[key] = ButtonState::Up;
}

void Input::handleEvent(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && !e.key.repeat) {
		SDL_Scancode sc = e.key.keysym.scancode;
		keyStates[sc] = ButtonState::Pressed;
	} else if (e.type == SDL_KEYUP) {
		SDL_Scancode sc = e.key.keysym.scancode;
		keyStates[sc] = ButtonState::Released;
	}
}

void Input::update() {
	for (auto& kv : keyStates) {
		if (kv.second == ButtonState::Pressed) kv.second = ButtonState::Held;
		else if (kv.second == ButtonState::Released) kv.second = ButtonState::Up;
	}
}

ButtonState Input::getActionState(const std::string& action) const {
	auto it = actionToKey.find(action);
	if (it == actionToKey.end()) return ButtonState::Up;
	auto it2 = keyStates.find(it->second);
	if (it2 == keyStates.end()) return ButtonState::Up;
	return it2->second;
}

bool Input::isActionPressed(const std::string& action) const {
	return getActionState(action) == ButtonState::Pressed;
}

bool Input::isActionHeld(const std::string& action) const {
	return getActionState(action) == ButtonState::Held;
}

bool Input::isActionReleased(const std::string& action) const {
	return getActionState(action) == ButtonState::Released;
}

} // namespace lite2d