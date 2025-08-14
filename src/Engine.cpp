#include "Engine/Engine.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/Input.hpp"
#include "Engine/Physics.hpp"
#include "Engine/Scene.hpp"
#include <SDL_image.h>
#include <vector>

namespace lite2d {

Engine::Engine() {}
Engine::~Engine() { shutdown(); }

bool Engine::initialize(const EngineConfig& cfg) {
	if (initialized) return true;
	config = cfg;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return false;
	}
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if ((IMG_Init(imgFlags) & imgFlags) == 0) {
		SDL_Log("IMG_Init failed: %s", IMG_GetError());
		return false;
	}
	window = SDL_CreateWindow(
		config.windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		config.windowWidth, config.windowHeight,
		SDL_WINDOW_SHOWN
	);
	if (!window) {
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		return false;
	}
	sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!sdlRenderer) {
		SDL_Log("SDL_CreateRenderer failed (%s). Retrying with software renderer...", SDL_GetError());
		sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
		if (!sdlRenderer) {
			SDL_Log("SDL_CreateRenderer (software) also failed: %s", SDL_GetError());
			return false;
		}
	}
	renderer = std::make_unique<Renderer>(sdlRenderer);
	input = std::make_unique<Input>();
	physics = std::make_unique<Physics>();
	scene = std::make_unique<Scene>(renderer.get(), input.get());

	// Input mappings
	input->mapAction("move_left", SDL_SCANCODE_A);
	input->mapAction("move_right", SDL_SCANCODE_D);
	input->mapAction("jump", SDL_SCANCODE_SPACE);

	// Build a demo scene: player + ground tiles
	{
		Entity* player = scene->createEntity("Player");
		player->isPlayer = true;
		// Create a generated 4-frame sprite sheet (48x48 each)
		const int frameW = 48;
		const int frameH = 48;
		const int frames = 4;
		SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, frameW * frames, frameH, 32, SDL_PIXELFORMAT_RGBA32);
		if (surf) {
			// four colored squares
			Uint32 colors[4] = {
				SDL_MapRGBA(surf->format, 255, 99, 71, 255),   // tomato
				SDL_MapRGBA(surf->format, 50, 205, 50, 255),   // limegreen
				SDL_MapRGBA(surf->format, 65, 105, 225, 255),  // royalblue
				SDL_MapRGBA(surf->format, 255, 215, 0, 255)    // gold
			};
			for (int i = 0; i < frames; ++i) {
				SDL_Rect r{ i * frameW, 0, frameW, frameH };
				SDL_FillRect(surf, &r, colors[i]);
			}
			int tw=0, th=0;
			SDL_Texture* tex = renderer->createTextureFromSurface(surf, tw, th);
			if (tex) {
				player->sprite.texture = tex;
				player->sprite.src = { 0.0f, 0.0f, static_cast<float>(frameW), static_cast<float>(frameH) };
				player->sprite.dst = { 0.0f, 0.0f, static_cast<float>(frameW), static_cast<float>(frameH) };
				AnimationClip clip{ "idle", frameW, frameH, frames, 0, 0.15f, true };
				player->animator.addClip(clip);
				player->animator.play("idle", true);
			}
			SDL_FreeSurface(surf);
		}
		player->body.position = { 100.0f, 100.0f };
		player->body.collider.rect = { 0, 0, static_cast<float>(frameW), static_cast<float>(frameH) };
	}
	// Ground
	for (int i = 0; i < 25; ++i) {
		Entity* g = scene->createEntity("Ground");
		g->body.isStatic = true;
		g->body.position = { i * 64.0f, 600.0f };
		g->body.collider.rect = { 0, 0, 64, 64 };
		g->sprite.dst = { g->body.position.x, g->body.position.y, 64, 64 };
	}

	initialized = true;
	return true;
}

void Engine::shutdown() {
	if (!initialized) return;
	scene.reset();
	physics.reset();
	input.reset();
	renderer.reset();
	if (sdlRenderer) { SDL_DestroyRenderer(sdlRenderer); sdlRenderer = nullptr; }
	if (window) { SDL_DestroyWindow(window); window = nullptr; }
	IMG_Quit();
	SDL_Quit();
	initialized = false;
}

void Engine::run() {
	const float fixedDt = 1.0f / static_cast<float>(config.targetFps);
	bool running = true;
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = now;
	double accumulator = 0.0;

	while (running) {
		last = now;
		now = SDL_GetPerformanceCounter();
		double elapsed = static_cast<double>(now - last) / static_cast<double>(SDL_GetPerformanceFrequency());
		accumulator += elapsed;

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) running = false;
			if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
			input->handleEvent(e);
		}

		while (accumulator >= fixedDt) {
			// scene input-driven changes
			scene->update(fixedDt);

			// gather bodies for physics
			std::vector<Body*> bodies;
			bodies.reserve(scene->getEntities().size());
			for (auto& e : scene->getEntities()) bodies.push_back(&e->body);
			physics->update(bodies, fixedDt);

			// camera follows player
			if (!scene->getEntities().empty()) {
				Body& first = scene->getEntities()[0]->body;
				Vec2 camTarget{ first.position.x - config.windowWidth * 0.5f + 24.0f,
					first.position.y - config.windowHeight * 0.5f + 24.0f };
				renderer->getCamera().setPosition(camTarget);
			}

			input->update();
			accumulator -= fixedDt;
		}

		renderer->clear(SDL_Color{40, 44, 52, 255});
		scene->render();
		renderer->present();
	}
}

} // namespace lite2d