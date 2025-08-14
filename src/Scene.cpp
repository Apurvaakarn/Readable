#include "Engine/Scene.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/Input.hpp"
#include "Engine/Animation.hpp"
#include <algorithm>

namespace lite2d {

Scene::Scene(Renderer* renderer, Input* input)
	: renderer(renderer), input(input) {}

Scene::~Scene() {}

Entity* Scene::createEntity(const std::string& name) {
	auto e = std::make_unique<Entity>();
	e->name = name;
	Entity* ptr = e.get();
	entities.emplace_back(std::move(e));
	return ptr;
}

void Scene::update(float fixedDt) {
	if (!input) return;
	if (player) {
		const float moveSpeed = 300.0f;
		if (input->isActionHeld("move_left")) player->body.velocity.x = -moveSpeed;
		else if (input->isActionHeld("move_right")) player->body.velocity.x = moveSpeed;
		else player->body.velocity.x = 0.0f;

		if (input->isActionPressed("jump")) {
			// simplistic jump impulse if near ground (y velocity approx zero)
			if (player->body.velocity.y == 0.0f) player->body.velocity.y = -450.0f;
		}
	}
	// update animations and adjust sprite src per entity
	for (auto& e : entities) {
		if (e->animator.isPlaying()) {
			e->animator.update(fixedDt);
			const AnimationClip* clip = e->animator.getCurrentClip();
			if (clip) {
				int frameIndex = clip->startFrame + e->animator.getCurrentFrameIndex();
				e->sprite.src = { static_cast<float>(frameIndex * clip->frameWidth), 0.0f, static_cast<float>(clip->frameWidth), static_cast<float>(clip->frameHeight) };
				if (e->sprite.dst.w == 0 || e->sprite.dst.h == 0) {
					e->sprite.dst.w = static_cast<float>(clip->frameWidth);
					e->sprite.dst.h = static_cast<float>(clip->frameHeight);
				}
			}
		}
	}
}

void Scene::render() {
	if (!renderer) return;
	for (auto& e : entities) {
		Rect dst = e->sprite.dst;
		dst.x = e->body.position.x;
		dst.y = e->body.position.y;
		if (e->sprite.texture) {
			renderer->drawTexture(e->sprite.texture, e->sprite.src, dst, true);
		} else {
			renderer->drawRect(dst, SDL_Color{255,255,255,255}, true, true);
		}
	}
}

} // namespace lite2d