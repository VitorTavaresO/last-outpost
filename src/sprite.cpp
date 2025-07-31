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
		this->sourceRect = {0, 0, 0, 0};
	}

	Sprite::~Sprite()
	{
		if (this->texture)
		{
			SDL_DestroyTexture(this->texture);
			this->texture = nullptr;
		}
	}

	bool Sprite::loadFromFile(const std::string &path, SDL_Renderer *renderer)
	{
		if (this->texture)
		{
			SDL_DestroyTexture(this->texture);
			this->texture = nullptr;
		}

		SDL_Surface *surface = IMG_Load(path.c_str());

		if (surface->format->BytesPerPixel == 4)
		{
			SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
		}
		this->texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

		this->size.x = surface->w;
		this->size.y = surface->h;

		SDL_FreeSurface(surface);
		return true;
	}

	void Sprite::render(Graphics &graphics) const
	{
		if (!this->texture)
			return;

		int tileWidth = graphics.getTileWidth();
		int tileHeight = graphics.getTileHeight();
		Vector finalSize;

		if (this->useSourceRect)
		{
			finalSize.x = static_cast<float>(this->sourceRect.w) * this->scale.x;
			finalSize.y = static_cast<float>(this->sourceRect.h) * this->scale.y;
		}
		else
		{
			finalSize.x = this->size.x * this->scale.x;
			finalSize.y = this->size.y * this->scale.y;
		}

		Vector finalPosition = this->position - this->origin;

		finalPosition.x = finalPosition.x * tileWidth + (tileWidth - finalSize.x) / 2.0f;
		finalPosition.y = finalPosition.y * tileHeight + (tileHeight - finalSize.y) / 2.0f;

		SDL_Rect destRect = {
			static_cast<int>(finalPosition.x),
			static_cast<int>(finalPosition.y),
			static_cast<int>(finalSize.x),
			static_cast<int>(finalSize.y)};
		SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
		if (this->flippedX)
			flipFlags = static_cast<SDL_RendererFlip>(flipFlags | SDL_FLIP_HORIZONTAL);
		if (this->flippedY)
			flipFlags = static_cast<SDL_RendererFlip>(flipFlags | SDL_FLIP_VERTICAL);

		SDL_SetTextureColorMod(this->texture, this->color.r, this->color.g, this->color.b);
		SDL_SetTextureAlphaMod(this->texture, this->color.a);

		if (this->useSourceRect)
			SDL_RenderCopyEx(graphics.getRenderer(), this->texture, &this->sourceRect, &destRect, this->rotation, nullptr, flipFlags);
		else
			SDL_RenderCopyEx(graphics.getRenderer(), this->texture, nullptr, &destRect, this->rotation, nullptr, flipFlags);
	}

	void Sprite::renderAt(SDL_Renderer *renderer, const SDL_Rect &destRect) const
	{
		if (!this->texture || !renderer)
			return;

		SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
		if (this->flippedX)
			flipFlags = static_cast<SDL_RendererFlip>(flipFlags | SDL_FLIP_HORIZONTAL);
		if (this->flippedY)
			flipFlags = static_cast<SDL_RendererFlip>(flipFlags | SDL_FLIP_VERTICAL);

		SDL_SetTextureColorMod(this->texture, this->color.r, this->color.g, this->color.b);
		SDL_SetTextureAlphaMod(this->texture, this->color.a);

		if (this->useSourceRect)
			SDL_RenderCopyEx(renderer, this->texture, &this->sourceRect, &destRect, this->rotation, nullptr, flipFlags);
		else
			SDL_RenderCopyEx(renderer, this->texture, nullptr, &destRect, this->rotation, nullptr, flipFlags);
	}
}
