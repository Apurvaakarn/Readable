#pragma once

#include "Engine.hpp"
#include <vector>

namespace lite2d {

struct AABB {
	Rect rect;
};

struct Body {
	Vec2 position{0.0f, 0.0f};
	Vec2 velocity{0.0f, 0.0f};
	Vec2 acceleration{0.0f, 0.0f};
	AABB collider{};
	bool isStatic{false};
};

class Physics {
public:
	void setGravity(const Vec2& g) { gravity = g; }
	void update(std::vector<Body*>& bodies, float fixedDt);

	static bool overlaps(const AABB& a, const AABB& b);
	static bool resolveStaticCollision(Body& dynamicBody, const Body& staticBody);

private:
	Vec2 gravity{0.0f, 800.0f};
};

} // namespace lite2d