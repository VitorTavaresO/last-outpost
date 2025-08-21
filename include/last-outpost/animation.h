#ifndef __LAST_OUTPOST_ANIMATION_H__
#define __LAST_OUTPOST_ANIMATION_H__

#include <SDL.h>
#include <string>
#include <vector>
#include <memory>
#include <last-outpost/types.h>
#include <last-outpost/graphics.h>
#include <last-outpost/sprite.h>

namespace Game
{
	class Animation
	{
	private:
		std::unique_ptr<Sprite> sprite;
		std::vector<SDL_Rect> frames;

		int currentFrame;
		float frameTime;
		float elapsedTime;
		bool isPlaying;
		bool isLooping;
		int totalFrames;
		int frameWidth;
		int frameHeight;
		int columns;
		int rows;
		int frameRangeStart;
		int frameRangeEnd;
		bool initialized;

	public:
		Animation(const std::string &filepath, SDL_Renderer *renderer,
				  int frameWidth, int frameHeight, int columns, int rows);
		~Animation();

		bool isValid() const;

		void play(bool loop = true);
		void pause();
		void stop();
		void reset();

		void setFrame(int frameIndex);
		void setFrame(int column, int row);
		void setFrameRange(int startFrame, int endFrame);
		void setFrameRange(int startCol, int startRow, int endCol, int endRow);

		void update(float deltaTime);
		void render(Graphics &graphics) const;
		void renderWithOverlay(Graphics &graphics, const SDL_Color &overlayColor) const;

		void setFrameTime(float time)
		{
			this->frameTime = time;
		}

		int getCurrentFrame() const
		{
			return this->currentFrame;
		}
		int getTotalFrames() const
		{
			return this->totalFrames;
		}

		void setPosition(float x, float y)
		{
			this->sprite->setPosition(x, y);
		}

		void setPosition(const Vector &pos)
		{
			this->sprite->setPosition(pos);
		}

		Vector getPosition() const
		{
			return this->sprite->getPosition();
		}

		void setScale(float x, float y)
		{
			this->sprite->setScale(x, y);
		}

		void setScale(const Vector &s)
		{
			this->sprite->setScale(s);
		}

		Vector getScale() const
		{
			return this->sprite->getScale();
		}

		void setOrigin(float x, float y)
		{
			this->sprite->setOrigin(x, y);
		}

		void setOrigin(const Vector &o)
		{
			this->sprite->setOrigin(o);
		}

		Vector getOrigin() const
		{
			return this->sprite->getOrigin();
		}

		void setRotation(float rot)
		{
			this->sprite->setRotation(rot);
		}

		float getRotation() const
		{
			return this->sprite->getRotation();
		}

		void setColor(const SDL_Color &c)
		{
			this->sprite->setColor(c);
		}

		SDL_Color getColor() const
		{
			return this->sprite->getColor();
		}

		void setFlipped(bool x, bool y)
		{
			this->sprite->setFlipped(x, y);
		}

		bool isFlippedX() const
		{
			return this->sprite->isFlippedX();
		}

		bool isFlippedY() const
		{
			return this->sprite->isFlippedY();
		}

		bool getIsPlaying() const
		{
			return this->isPlaying;
		}
		bool getIsLooping() const
		{
			return this->isLooping;
		}

		SDL_Rect getFrameRect(int frameIndex) const
		{
			return this->frames[frameIndex];
		}
	};
}

#endif