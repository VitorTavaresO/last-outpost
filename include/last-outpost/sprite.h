#ifndef __LAST_OUTPOST_SPRITE_H__
#define __LAST_OUTPOST_SPRITE_H__

#include <SDL.h>
#include <string>
#include <last-outpost/types.h>
#include <last-outpost/graphics.h>

namespace Game
{
	class Sprite
	{
	private:
		SDL_Texture *texture;
		SDL_Rect sourceRect;
		bool useSourceRect;
		Vector position;
		Vector size;
		Vector scale;
		Vector origin;
		float rotation;
		SDL_Color color;
		bool flippedX;
		bool flippedY;

		int frameWidth, frameHeight;
		int columns, rows;
		int currentFrame;

	public:
		Sprite();
		~Sprite();
		bool loadFromFile(const std::string &path, SDL_Renderer *renderer);

		bool loadSpriteSheet(const std::string &filepath, SDL_Renderer *renderer,
							 int frameWidth, int frameHeight, int columns, int rows);
		void setFrame(int frameIndex);
		void setFrame(int column, int row);
		int getCurrentFrame() const { return currentFrame; }

		void render(Graphics &graphics) const;

		void setSourceRect(int x, int y, int width, int height)
		{
			this->sourceRect.x = x;
			this->sourceRect.y = y;
			this->sourceRect.w = width;
			this->sourceRect.h = height;
			this->useSourceRect = true;
		}

		void setSourceRect(const SDL_Rect &rect)
		{
			this->sourceRect = rect;
			this->useSourceRect = true;
		}

		void resetSourceRect()
		{
			this->useSourceRect = false;
		}

		SDL_Rect getSourceRect() const
		{
			return this->sourceRect;
		}

		bool isUsingSourceRect() const
		{
			return this->useSourceRect;
		}

		void setPosition(float x, float y)
		{
			this->position.x = x;
			this->position.y = y;
		}

		void setPosition(const Vector &position)
		{
			this->position = position;
		}

		Vector getPosition() const
		{
			return this->position;
		}

		void setSize(float width, float height)
		{
			this->size.x = width;
			this->size.y = height;
		}

		void setSize(const Vector &size)
		{
			this->size = size;
		}

		Vector getSize() const
		{
			return this->size;
		}

		void setScale(float x, float y)
		{
			this->scale.x = x;
			this->scale.y = y;
		}

		void setScale(const Vector &scale)
		{
			this->scale = scale;
		}

		Vector getScale() const
		{
			return this->scale;
		}

		void setOrigin(float x, float y)
		{
			this->origin.x = x;
			this->origin.y = y;
		}

		void setOrigin(const Vector &origin)
		{
			this->origin = origin;
		}

		Vector getOrigin() const
		{
			return this->origin;
		}

		void setRotation(float rotation)
		{
			this->rotation = rotation;
		}

		float getRotation() const
		{
			return this->rotation;
		}

		void setColor(const SDL_Color &color)
		{
			this->color = color;
		}

		SDL_Color getColor() const
		{
			return this->color;
		}

		void setFlipped(bool x, bool y)
		{
			this->flippedX = x;
			this->flippedY = y;
		}

		bool isFlippedX() const
		{
			return this->flippedX;
		}

		bool isFlippedY() const
		{
			return this->flippedY;
		}
	};
}

#endif