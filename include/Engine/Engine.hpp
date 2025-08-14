#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <SDL.h>

namespace lite2d {

struct Vec2 {
	float x{0.0f};
	float y{0.0f};
};

struct Rect {
	float x{0.0f};
	float y{0.0f};
	float w{0.0f};
	float h{0.0f};
};

class Texture;
class Renderer;
class Input;
class AnimationPlayer;
class Physics;
class Scene;

struct EngineConfig {
	int windowWidth{1280};
	int windowHeight{720};
	std::string windowTitle{"Lite2D Engine"};
	int targetFps{60};
};

class Engine {
public:
	Engine();
	~Engine();

	bool initialize(const EngineConfig& config);
	void shutdown();
	void run();

	Renderer* getRenderer() const { return renderer.get(); }
	Input* getInput() const { return input.get(); }
	Physics* getPhysics() const { return physics.get(); }
	Scene* getScene() const { return scene.get(); }

	SDL_Window* getWindow() const { return window; }

private:
	bool initialized{false};
	EngineConfig config{};

	SDL_Window* window{nullptr};
	SDL_Renderer* sdlRenderer{nullptr};

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Input> input;
	std::unique_ptr<Physics> physics;
	std::unique_ptr<Scene> scene;
};

} // namespace lite2d