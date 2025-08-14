#pragma once

#include <SDL.h>
#include <memory>
#include <string>
#include "Engine.hpp"

namespace lite2d {

class Texture {
public:
	Texture(SDL_Texture* handle, int width, int height)
		: handle(handle), width(width), height(height) {}
	~Texture();

	SDL_Texture* getHandle() const { return handle; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }

private:
	SDL_Texture* handle{nullptr};
	int width{0};
	int height{0};
};

class Camera {
public:
	void setPosition(const Vec2& p) { position = p; }
	const Vec2& getPosition() const { return position; }
	float getZoom() const { return zoom; }
	void setZoom(float z) { zoom = z; }

private:
	Vec2 position{0.0f, 0.0f};
	float zoom{1.0f};
};

class Renderer {
public:
	explicit Renderer(SDL_Renderer* sdlRenderer);
	~Renderer();

	SDL_Texture* createTextureFromFile(const std::string& path, int& outW, int& outH);
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface, int& outW, int& outH);
	void clear(const SDL_Color& color);
	void present();

	void beginFrame();
	void drawTexture(SDL_Texture* tex, const Rect& src, const Rect& dst, bool applyCamera = true, double rotationDeg = 0.0);
	void drawRect(const Rect& rect, const SDL_Color& color, bool filled = true, bool applyCamera = true);

	Camera& getCamera() { return camera; }

private:
	SDL_Renderer* sdlRenderer{nullptr};
	Camera camera{};
	float camPosX{0.0f};
	float camPosY{0.0f};
	float camZoom{1.0f};
	int outputW{0};
	int outputH{0};
	bool isOnScreen(const SDL_FRect& r) const;
};

} // namespace lite2d