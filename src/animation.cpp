#include <last-outpost/animation.h>
#include <iostream>

namespace Game
{
	Animation::Animation()
		: sprite(std::make_unique<Sprite>()),
		  currentFrame(0), frameTime(0.1f), elapsedTime(0.0f),
		  isPlaying(false), isLooping(true), totalFrames(0),
		  frameWidth(0), frameHeight(0), columns(0), rows(0),
		  frameRangeStart(0), frameRangeEnd(0)
	{
	}

	Animation::~Animation() = default;
	bool Animation::loadSpriteSheet(const std::string &filepath, SDL_Renderer *renderer,
									int frameWidth, int frameHeight, int columns, int rows)
	{
		if (!sprite->loadFromFile(filepath, renderer))
		{
			std::cerr << "Failed to load animation sprite sheet: " << filepath << std::endl;
			return false;
		}

		this->frameWidth = frameWidth;
		this->frameHeight = frameHeight;
		this->columns = columns;
		this->rows = rows;
		this->totalFrames = columns * rows;
		this->frameRangeStart = 0;
		this->frameRangeEnd = totalFrames - 1;

		frames.clear();
		frames.reserve(totalFrames);

		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < columns; ++col)
			{
				SDL_Rect frameRect = {
					col * frameWidth,
					row * frameHeight,
					frameWidth,
					frameHeight};
				frames.push_back(frameRect);
			}
		}

		currentFrame = 0;
		if (!frames.empty())
		{
			sprite->setSourceRect(frames[0]);
		}

		return true;
	}

	void Animation::play(bool loop)
	{
		isPlaying = true;
		isLooping = loop;
	}

	void Animation::pause()
	{
		isPlaying = false;
	}
	void Animation::stop()
	{
		isPlaying = false;
		currentFrame = frameRangeStart;
		elapsedTime = 0.0f;
		if (currentFrame < static_cast<int>(frames.size()))
		{
			sprite->setSourceRect(frames[currentFrame]);
		}
	}
	void Animation::reset()
	{
		currentFrame = frameRangeStart;
		elapsedTime = 0.0f;
		if (currentFrame < static_cast<int>(frames.size()))
		{
			sprite->setSourceRect(frames[currentFrame]);
		}
	}

	void Animation::setFrame(int frameIndex)
	{
		if (frameIndex >= 0 && frameIndex < totalFrames && frameIndex < static_cast<int>(frames.size()))
		{
			currentFrame = frameIndex;
			sprite->setSourceRect(frames[currentFrame]);
		}
	}

	void Animation::setFrame(int column, int row)
	{
		if (column >= 0 && column < columns && row >= 0 && row < rows)
		{
			int frameIndex = row * columns + column;
			setFrame(frameIndex);
		}
	}

	void Animation::setFrameRange(int startFrame, int endFrame)
	{
		if (startFrame >= 0 && startFrame < totalFrames &&
			endFrame >= startFrame && endFrame < totalFrames)
		{
			frameRangeStart = startFrame;
			frameRangeEnd = endFrame;

			if (currentFrame < frameRangeStart || currentFrame > frameRangeEnd)
			{
				currentFrame = frameRangeStart;
				if (currentFrame < static_cast<int>(frames.size()))
				{
					sprite->setSourceRect(frames[currentFrame]);
				}
			}
		}
	}

	void Animation::setFrameRange(int startCol, int startRow, int endCol, int endRow)
	{
		if (startCol >= 0 && startCol < columns && startRow >= 0 && startRow < rows &&
			endCol >= 0 && endCol < columns && endRow >= 0 && endRow < rows)
		{
			int startFrame = startRow * columns + startCol;
			int endFrame = endRow * columns + endCol;
			setFrameRange(startFrame, endFrame);
		}
	}
	void Animation::update(float deltaTime)
	{
		if (!isPlaying || frameRangeEnd <= frameRangeStart)
			return;

		elapsedTime += deltaTime;

		if (elapsedTime >= frameTime)
		{
			elapsedTime -= frameTime;
			currentFrame++;

			if (currentFrame > frameRangeEnd)
			{
				if (isLooping)
				{
					currentFrame = frameRangeStart;
				}
				else
				{
					currentFrame = frameRangeEnd;
					isPlaying = false;
				}
			}

			if (currentFrame < static_cast<int>(frames.size()))
			{
				sprite->setSourceRect(frames[currentFrame]);
			}
		}
	}

	void Animation::render(Graphics &graphics) const
	{
		if (sprite)
		{
			sprite->render(graphics);
		}
	}

	void Animation::setPosition(float x, float y)
	{
		if (sprite)
		{
			sprite->setPosition(x, y);
		}
	}

	void Animation::setPosition(const Vector &pos)
	{
		if (sprite)
		{
			sprite->setPosition(pos);
		}
	}

	Vector Animation::getPosition() const
	{
		if (sprite)
		{
			return sprite->getPosition();
		}
		return {0, 0};
	}

	void Animation::setScale(float x, float y)
	{
		if (sprite)
		{
			sprite->setScale(x, y);
		}
	}

	void Animation::setScale(const Vector &s)
	{
		if (sprite)
		{
			sprite->setScale(s);
		}
	}

	Vector Animation::getScale() const
	{
		if (sprite)
		{
			return sprite->getScale();
		}
		return {1, 1};
	}

	void Animation::setOrigin(float x, float y)
	{
		if (sprite)
		{
			sprite->setOrigin(x, y);
		}
	}

	void Animation::setOrigin(const Vector &o)
	{
		if (sprite)
		{
			sprite->setOrigin(o);
		}
	}

	Vector Animation::getOrigin() const
	{
		if (sprite)
		{
			return sprite->getOrigin();
		}
		return {0, 0};
	}

	void Animation::setRotation(float rot)
	{
		if (sprite)
		{
			sprite->setRotation(rot);
		}
	}

	float Animation::getRotation() const
	{
		if (sprite)
		{
			return sprite->getRotation();
		}
		return 0.0f;
	}

	void Animation::setColor(const SDL_Color &c)
	{
		if (sprite)
		{
			sprite->setColor(c);
		}
	}

	SDL_Color Animation::getColor() const
	{
		if (sprite)
		{
			return sprite->getColor();
		}
		return {255, 255, 255, 255};
	}

	void Animation::setFlipped(bool x, bool y)
	{
		if (sprite)
		{
			sprite->setFlipped(x, y);
		}
	}

	bool Animation::isFlippedX() const
	{
		if (sprite)
		{
			return sprite->isFlippedX();
		}
		return false;
	}

	bool Animation::isFlippedY() const
	{
		if (sprite)
		{
			return sprite->isFlippedY();
		}
		return false;
	}

	SDL_Rect Animation::getFrameRect(int frameIndex) const
	{
		if (frameIndex >= 0 && frameIndex < static_cast<int>(frames.size()))
		{
			return frames[frameIndex];
		}
		return {0, 0, 0, 0};
	}
}