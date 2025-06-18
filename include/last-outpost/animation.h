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
		void setFrameTime(float time) { frameTime = time; }

		void setFrame(int frameIndex);
		void setFrame(int column, int row);
		void setFrameRange(int startFrame, int endFrame);
		void setFrameRange(int startCol, int startRow, int endCol, int endRow);
		int getCurrentFrame() const { return currentFrame; }
		int getTotalFrames() const { return totalFrames; }
		void update(float deltaTime);
		void render(Graphics &graphics) const;

		// Delegate methods to sprite for convenience
		void setPosition(float x, float y);
		void setPosition(const Vector &pos);
		Vector getPosition() const;

		void setScale(float x, float y);
		void setScale(const Vector &s);
		Vector getScale() const;

		void setOrigin(float x, float y);
		void setOrigin(const Vector &o);
		Vector getOrigin() const;

		void setRotation(float rot);
		float getRotation() const;

		void setColor(const SDL_Color &c);
		SDL_Color getColor() const;

		void setFlipped(bool x, bool y);
		bool isFlippedX() const;
		bool isFlippedY() const;

		bool getIsPlaying() const { return isPlaying; }
		bool getIsLooping() const { return isLooping; }

		SDL_Rect getFrameRect(int frameIndex) const;
	};
}

#endif