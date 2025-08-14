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

void Renderer::drawTexture(SDL_Texture* tex, const Rect& src, const Rect& dst, bool applyCamera, double rotationDeg) {
	if (!tex) return;
	SDL_Rect sdlSrc{ static_cast<int>(src.x), static_cast<int>(src.y), static_cast<int>(src.w), static_cast<int>(src.h) };
	SDL_FRect sdlDst{ dst.x, dst.y, dst.w, dst.h };
	if (applyCamera) {
		const Vec2 camPos = camera.getPosition();
		sdlDst.x = (dst.x - camPos.x) * camera.getZoom();
		sdlDst.y = (dst.y - camPos.y) * camera.getZoom();
		sdlDst.w = dst.w * camera.getZoom();
		sdlDst.h = dst.h * camera.getZoom();
	}
	SDL_RenderCopyExF(sdlRenderer, tex, &sdlSrc, &sdlDst, rotationDeg, nullptr, SDL_FLIP_NONE);
}

void Renderer::drawRect(const Rect& rect, const SDL_Color& color, bool filled, bool applyCamera) {
	SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
	SDL_FRect r{ rect.x, rect.y, rect.w, rect.h };
	if (applyCamera) {
		const Vec2 camPos = camera.getPosition();
		r.x = (rect.x - camPos.x) * camera.getZoom();
		r.y = (rect.y - camPos.y) * camera.getZoom();
		r.w = rect.w * camera.getZoom();
		r.h = rect.h * camera.getZoom();
	}
	if (filled) SDL_RenderFillRectF(sdlRenderer, &r);
	else SDL_RenderDrawRectF(sdlRenderer, &r);
}

} // namespace lite2d