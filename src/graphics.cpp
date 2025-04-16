#include <last-outpost/graphics.h>
#include <algorithm>
#include <SDL2/SDL2_gfxPrimitives.h>

namespace Game
{
	Graphics::Graphics(int screenWidth, int screenHeight, int tilesX, int tilesY, SDL_Renderer *renderer)
		: renderer(renderer)
	{
		if (!this->renderer)
		{
			SDL_Log("Renderer not provided to Graphics constructor!");
			throw std::runtime_error("Renderer is null");
		}

		int tileSize = std::min(screenWidth / tilesX, screenHeight / tilesY);
		this->tileWidth = tileSize;
		this->tileHeight = tileSize;
	}

	void Graphics::drawRect(const Vector &position, const Vector &size, const SDL_Color &color) const
	{
		if (!this->renderer)
		{
			SDL_Log("Renderer not set in Graphics!");
			return;
		}

		SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);

		SDL_Rect rect = {
			static_cast<int>(position.x * this->tileWidth),
			static_cast<int>(position.y * this->tileHeight),
			static_cast<int>(size.x * this->tileWidth),
			static_cast<int>(size.y * this->tileHeight)};

		SDL_RenderFillRect(this->renderer, &rect);
	}

	void Graphics::drawCircle(const Vector &center, float radius, const SDL_Color &color) const
	{
		if (!this->renderer)
		{
			SDL_Log("Renderer not set in Graphics!");
			return;
		}

		int centerX = static_cast<int>(center.x * this->tileWidth);
		int centerY = static_cast<int>(center.y * this->tileHeight);
		int pixelRadius = static_cast<int>(radius * this->tileWidth);

		filledCircleRGBA(this->renderer,
						 centerX,
						 centerY,
						 pixelRadius,
						 color.r,
						 color.g,
						 color.b,
						 color.a);
	}
}