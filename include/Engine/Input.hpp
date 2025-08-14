#pragma once

#include <SDL.h>
#include <unordered_map>
#include <string>

namespace lite2d {

enum class ButtonState { Up, Pressed, Held, Released };

class Input {
public:
	void mapAction(const std::string& action, SDL_Scancode key);
	void handleEvent(const SDL_Event& e);
	void update();
	ButtonState getActionState(const std::string& action) const;
	bool isActionPressed(const std::string& action) const;
	bool isActionHeld(const std::string& action) const;
	bool isActionReleased(const std::string& action) const;

private:
	std::unordered_map<std::string, SDL_Scancode> actionToKey;
	std::unordered_map<SDL_Scancode, ButtonState> keyStates;
};

} // namespace lite2d