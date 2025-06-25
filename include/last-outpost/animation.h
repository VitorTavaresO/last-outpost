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

	public:
		Animation();
		~Animation();

		bool loadSpriteSheet(const std::string &filepath, SDL_Renderer *renderer,
							 int frameWidth, int frameHeight, int columns, int rows);

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

		void setFrameTime(float time)
		{
			frameTime = time;
		}

		int getCurrentFrame() const
		{
			return currentFrame;
		}
		int getTotalFrames() const
		{
			return totalFrames;
		}

		void setPosition(float x, float y)
		{
			sprite->setPosition(x, y);
		}

		void setPosition(const Vector &pos)
		{
			sprite->setPosition(pos);
		}

		Vector getPosition() const
		{
			return sprite->getPosition();
		}

		void setScale(float x, float y)
		{
			sprite->setScale(x, y);
		}

		void setScale(const Vector &s)
		{
			sprite->setScale(s);
		}

		Vector getScale() const
		{
			return sprite->getScale();
		}

		void setOrigin(float x, float y)
		{
			sprite->setOrigin(x, y);
		}

		void setOrigin(const Vector &o)
		{
			sprite->setOrigin(o);
		}

		Vector getOrigin() const
		{
			return sprite->getOrigin();
		}

		void setRotation(float rot)
		{
			sprite->setRotation(rot);
		}

		float getRotation() const
		{
			return sprite->getRotation();
		}

		void setColor(const SDL_Color &c)
		{
			sprite->setColor(c);
		}

		SDL_Color getColor() const
		{
			return sprite->getColor();
		}

		void setFlipped(bool x, bool y)
		{
			sprite->setFlipped(x, y);
		}

		bool isFlippedX() const
		{
			return sprite->isFlippedX();
		}

		bool isFlippedY() const
		{
			return sprite->isFlippedY();
		}

		bool getIsPlaying() const
		{
			return isPlaying;
		}
		bool getIsLooping() const
		{
			return isLooping;
		}

		SDL_Rect getFrameRect(int frameIndex) const
		{
			return frames[frameIndex];
		}
	};
}

#endif