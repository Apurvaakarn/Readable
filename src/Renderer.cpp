#include "Engine/Renderer.hpp"
#include <SDL_image.h>
#include <cmath>

namespace lite2d {

Texture::~Texture() {
	if (handle) {
		SDL_DestroyTexture(handle);
		handle = nullptr;
	}
}

Renderer::Renderer(SDL_Renderer* sdlRenderer) : sdlRenderer(sdlRenderer) {}
Renderer::~Renderer() {}

SDL_Texture* Renderer::createTextureFromFile(const std::string& path, int& outW, int& outH) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface) {
		SDL_Log("IMG_Load failed: %s", IMG_GetError());
		return nullptr;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlRenderer, surface);
	if (!tex) {
		SDL_Log("CreateTextureFromSurface failed: %s", SDL_GetError());
		SDL_FreeSurface(surface);
		return nullptr;
	}
	outW = surface->w;
	outH = surface->h;
	SDL_FreeSurface(surface);
	return tex;
}

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* surface, int& outW, int& outH) {
	if (!surface) return nullptr;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlRenderer, surface);
	if (!tex) {
		SDL_Log("CreateTextureFromSurface failed: %s", SDL_GetError());
		return nullptr;
	}
	outW = surface->w;
	outH = surface->h;
	return tex;
}

void Renderer::clear(const SDL_Color& color) {
	SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(sdlRenderer);
}

void Renderer::present() {
	SDL_RenderPresent(sdlRenderer);
}

void Renderer::beginFrame() {
	// Cache camera and output size for faster per-draw transforms
	camPosX = camera.getPosition().x;
	camPosY = camera.getPosition().y;
	camZoom = camera.getZoom();
	int w = 0, h = 0;
	SDL_GetRendererOutputSize(sdlRenderer, &w, &h);
	outputW = w; outputH = h;
}

static inline bool rectOffscreen(const SDL_FRect& r, int w, int h) {
	return (r.x + r.w <= 0.0f) || (r.y + r.h <= 0.0f) || (r.x >= static_cast<float>(w)) || (r.y >= static_cast<float>(h));
}

bool Renderer::isOnScreen(const SDL_FRect& r) const {
	if (outputW <= 0 || outputH <= 0) return true; // unknown: do not cull
	return !rectOffscreen(r, outputW, outputH);
}

void Renderer::drawTexture(SDL_Texture* tex, const Rect& src, const Rect& dst, bool applyCamera, double rotationDeg) {
	if (!tex) return;
	SDL_Rect sdlSrc{ static_cast<int>(src.x), static_cast<int>(src.y), static_cast<int>(src.w), static_cast<int>(src.h) };
	SDL_FRect sdlDst{ dst.x, dst.y, dst.w, dst.h };
	if (applyCamera) {
		sdlDst.x = (dst.x - camPosX) * camZoom;
		sdlDst.y = (dst.y - camPosY) * camZoom;
		sdlDst.w = dst.w * camZoom;
		sdlDst.h = dst.h * camZoom;
	}
	if (!isOnScreen(sdlDst)) return;
	if (rotationDeg == 0.0) {
		SDL_RenderCopyF(sdlRenderer, tex, &sdlSrc, &sdlDst);
	} else {
		SDL_RenderCopyExF(sdlRenderer, tex, &sdlSrc, &sdlDst, rotationDeg, nullptr, SDL_FLIP_NONE);
	}
}

void Renderer::drawRect(const Rect& rect, const SDL_Color& color, bool filled, bool applyCamera) {
	SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
	SDL_FRect r{ rect.x, rect.y, rect.w, rect.h };
	if (applyCamera) {
		r.x = (rect.x - camPosX) * camZoom;
		r.y = (rect.y - camPosY) * camZoom;
		r.w = rect.w * camZoom;
		r.h = rect.h * camZoom;
	}
	if (!isOnScreen(r)) return;
	if (filled) SDL_RenderFillRectF(sdlRenderer, &r);
	else SDL_RenderDrawRectF(sdlRenderer, &r);
}

} // namespace lite2d