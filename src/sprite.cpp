#include <last-outpost/sprite.h>
#include <SDL_image.h>
#include <iostream>

namespace Game
{
	Sprite::Sprite()
		: texture(nullptr), useSourceRect(false), position{0, 0},
		  size{1, 1}, scale{1, 1}, origin{0, 0}, rotation(0),
		  color{255, 255, 255, 255}, flippedX(false), flippedY(false)
	{
		sourceRect = {0, 0, 0, 0};
	}

	Sprite::~Sprite()
	{
		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

	bool Sprite::loadFromFile(const std::string &path, SDL_Renderer *renderer)
	{
		if (!renderer)
		{
			std::cerr << "Renderer is null in Sprite::loadFromFile" << std::endl;
			return false;
		}

		if (texture)
		{
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}

		SDL_Surface *surface = IMG_Load(path.c_str());
		if (!surface)
		{
			std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
			return false;
		}

		if (surface->format->BytesPerPixel == 4)
		{
			SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
		}

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!texture)
		{
			std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
			SDL_FreeSurface(surface);
			return false;
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		size.x = surface->w;
		size.y = surface->h;

		SDL_FreeSurface(surface);
		return true;
	}

	void Sprite::render(Graphics &graphics) const
	{
		if (!texture)
			return;

		int tileWidth = graphics.getTileWidth();
		int tileHeight = graphics.getTileHeight();

		Vector finalSize = {
			static_cast<float>(tileWidth) * scale.x,
			static_cast<float>(tileHeight) * scale.y};

		Vector finalPosition = position - origin;

		finalPosition.x = finalPosition.x * tileWidth + (tileWidth - finalSize.x) / 2.0f;
		finalPosition.y = finalPosition.y * tileHeight + (tileHeight - finalSize.y) / 2.0f;

		SDL_Rect destRect = {
			static_cast<int>(finalPosition.x),
			static_cast<int>(finalPosition.y),
			static_cast<int>(finalSize.x),
			static_cast<int>(finalSize.y)};

		SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
		if (flippedX)
			flipFlags = static_cast<SDL_RendererFlip>(flipFlags | SDL_FLIP_HORIZONTAL);
		if (flippedY)
			flipFlags = static_cast<SDL_RendererFlip>(flipFlags | SDL_FLIP_VERTICAL);

		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
		SDL_SetTextureAlphaMod(texture, color.a);

		if (useSourceRect)
		{
			SDL_RenderCopyEx(graphics.getRenderer(), texture, &sourceRect, &destRect,
							 rotation, nullptr, flipFlags);
		}
		else
		{
			SDL_RenderCopyEx(graphics.getRenderer(), texture, nullptr, &destRect,
							 rotation, nullptr, flipFlags);
		}
	}
}