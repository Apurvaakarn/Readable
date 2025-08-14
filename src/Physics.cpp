#include "Engine/Physics.hpp"
#include <algorithm>

namespace lite2d {

static inline Rect computeAABBWorldRect(const Body& b) {
	Rect r = b.collider.rect;
	r.x += b.position.x;
	r.y += b.position.y;
	return r;
}

bool Physics::overlaps(const AABB& a, const AABB& b) {
	const Rect& ar = a.rect;
	const Rect& br = b.rect;
	return (ar.x < br.x + br.w && ar.x + ar.w > br.x && ar.y < br.y + br.h && ar.y + ar.h > br.y);
}

bool Physics::resolveStaticCollision(Body& dynamicBody, const Body& staticBody) {
	Rect dr = computeAABBWorldRect(dynamicBody);
	Rect sr = computeAABBWorldRect(staticBody);
	if (!(dr.x < sr.x + sr.w && dr.x + dr.w > sr.x && dr.y < sr.y + sr.h && dr.y + dr.h > sr.y)) {
		return false;
	}
	float moveRight = (sr.x + sr.w) - dr.x;
	float moveLeft = (dr.x + dr.w) - sr.x;
	float moveDown = (sr.y + sr.h) - dr.y;
	float moveUp = (dr.y + dr.h) - sr.y;
	float minX = std::min(moveRight, moveLeft);
	float minY = std::min(moveDown, moveUp);
	if (minX < minY) {
		if (moveRight < moveLeft) dynamicBody.position.x += moveRight;
		else dynamicBody.position.x -= moveLeft;
		dynamicBody.velocity.x = 0.0f;
	} else {
		if (moveDown < moveUp) dynamicBody.position.y += moveDown;
		else dynamicBody.position.y -= moveUp;
		dynamicBody.velocity.y = 0.0f;
	}
	return true;
}

void Physics::update(std::vector<Body*>& bodies, float fixedDt) {
	for (Body* body : bodies) {
		if (!body || body->isStatic) continue;
		body->acceleration = gravity;
		body->velocity.x += body->acceleration.x * fixedDt;
		body->velocity.y += body->acceleration.y * fixedDt;
		body->position.x += body->velocity.x * fixedDt;
		body->position.y += body->velocity.y * fixedDt;
	}
	// naive broadphase O(n^2) static collision pass
	for (Body* a : bodies) {
		if (!a || a->isStatic) continue;
		for (Body* b : bodies) {
			if (!b || !b->isStatic) continue;
			resolveStaticCollision(*a, *b);
		}
	}
}

} // namespace lite2d