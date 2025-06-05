#include <last-outpost/sprite.h>
#include <SDL_image.h>
#include <iostream>

namespace Game
{
	Sprite::Sprite()
		: texture(nullptr), useSourceRect(false), position{0, 0},
		  size{1, 1}, scale{1, 1}, origin{0, 0}, rotation(0),
		  color{255, 255, 255, 255}, flippedX(false), flippedY(false),
		  frameWidth(0), frameHeight(0), columns(0), rows(0),
		  currentFrame(0)
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
		if (!surface)
		{
			std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
			return false;
		}

		if (surface->format->BytesPerPixel == 4)
		{
			SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
		}
		this->texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!this->texture)
		{
			std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
			SDL_FreeSurface(surface);
			return false;
		}
		SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);

		this->size.x = surface->w;
		this->size.y = surface->h;

		SDL_FreeSurface(surface);

		return true;
	}

	bool Sprite::loadSpriteSheet(const std::string &filepath, SDL_Renderer *renderer,
								 int frameWidth, int frameHeight, int columns, int rows)
	{
		if (this->loadFromFile(filepath, renderer))
		{
			this->frameWidth = frameWidth;
			this->frameHeight = frameHeight;
			this->columns = columns;
			this->rows = rows;
			this->currentFrame = 0;

			this->setFrame(0);
			return true;
		}
		return false;
	}

	void Sprite::setFrame(int frameIndex)
	{

		int totalFrames = this->columns * this->rows;
		if (frameIndex < 0 || frameIndex >= totalFrames)
			return;

		this->currentFrame = frameIndex;

		int col = frameIndex % this->columns;
		int row = frameIndex / this->columns;

		this->setSourceRect(col * this->frameWidth, row * this->frameHeight, this->frameWidth, this->frameHeight);
	}

	void Sprite::setFrame(int column, int row)
	{
		if (column < 0 || column >= this->columns || row < 0 || row >= this->rows)
			return;

		int frameIndex = row * this->columns + column;
		this->setFrame(frameIndex);
	}
	void Sprite::render(Graphics &graphics) const
	{
		if (!this->texture)
			return;

		int tileWidth = graphics.getTileWidth();
		int tileHeight = graphics.getTileHeight();

		Vector finalSize;

		finalSize.x = static_cast<float>(this->frameWidth) * this->scale.x;
		finalSize.y = static_cast<float>(this->frameHeight) * this->scale.y;

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
}