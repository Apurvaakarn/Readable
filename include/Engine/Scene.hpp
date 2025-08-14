#pragma once

#include <vector>
#include <string>
#include <memory>
#include <SDL.h>
#include "Engine.hpp"
#include "Physics.hpp"
#include "Animation.hpp"

namespace lite2d {

struct Sprite {
	SDL_Texture* texture{nullptr};
	Rect src{0,0,0,0};
	Rect dst{0,0,0,0};
};

class Entity {
public:
	std::string name;
	Body body{};
	Sprite sprite{};
	AnimationPlayer animator{};

	bool isPlayer{false};
};

class Renderer;
class Input;

class Scene {
public:
	Scene(Renderer* renderer, Input* input);
	~Scene();

	Entity* createEntity(const std::string& name);
	void update(float fixedDt);
	void render();

	Entity* getPlayer() const { return player; }
	std::vector<std::unique_ptr<Entity>>& getEntities() { return entities; }

private:
	Renderer* renderer{nullptr};
	Input* input{nullptr};
	std::vector<std::unique_ptr<Entity>> entities;
	Entity* player{nullptr};
};

} // namespace lite2d