#include <last-outpost/animation.h>
#include <iostream>

namespace Game
{
	Animation::Animation(const std::string &filepath, SDL_Renderer *renderer,
						 int frameWidth, int frameHeight, int columns, int rows)
		: sprite(std::make_unique<Sprite>()),
		  currentFrame(0), frameTime(0.1f), elapsedTime(0.0f),
		  isPlaying(false), isLooping(true), totalFrames(0),
		  frameWidth(frameWidth), frameHeight(frameHeight),
		  columns(columns), rows(rows),
		  frameRangeStart(0), frameRangeEnd(0), initialized(false)
	{
		this->sprite->loadFromFile(filepath, renderer);

		this->totalFrames = columns * rows;
		this->frameRangeStart = 0;
		this->frameRangeEnd = this->totalFrames - 1;

		this->frames.clear();
		this->frames.reserve(this->totalFrames);

		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < columns; ++col)
			{
				SDL_Rect frameRect = {
					col * frameWidth,
					row * frameHeight,
					frameWidth,
					frameHeight};
				this->frames.push_back(frameRect);
			}
		}

		this->currentFrame = 0;
		this->sprite->setSourceRect(this->frames[0]);
		this->initialized = true;
	}

	Animation::~Animation() = default;

	bool Animation::isValid() const
	{
		return this->initialized;
	}

	void Animation::play(bool loop)
	{
		this->isPlaying = true;
		this->isLooping = loop;
	}

	void Animation::pause()
	{
		this->isPlaying = false;
	}
	void Animation::stop()
	{
		this->isPlaying = false;
		this->currentFrame = this->frameRangeStart;
		this->elapsedTime = 0.0f;
		this->sprite->setSourceRect(this->frames[this->currentFrame]);
	}
	void Animation::reset()
	{
		this->currentFrame = this->frameRangeStart;
		this->elapsedTime = 0.0f;
		this->sprite->setSourceRect(this->frames[this->currentFrame]);
	}

	void Animation::setFrame(int frameIndex)
	{
		if (frameIndex >= 0 && frameIndex < this->totalFrames)
		{
			this->currentFrame = frameIndex;
			this->sprite->setSourceRect(this->frames[this->currentFrame]);
		}
	}

	void Animation::setFrame(int column, int row)
	{
		if (column >= 0 && column < this->columns && row >= 0 && row < this->rows)
		{
			int frameIndex = row * this->columns + column;
			setFrame(frameIndex);
		}
	}

	void Animation::setFrameRange(int startFrame, int endFrame)
	{
		if (startFrame >= 0 && startFrame < this->totalFrames &&
			endFrame >= startFrame && endFrame < this->totalFrames)
		{
			this->frameRangeStart = startFrame;
			this->frameRangeEnd = endFrame;

			if (this->currentFrame < this->frameRangeStart || this->currentFrame > this->frameRangeEnd)
			{
				this->currentFrame = this->frameRangeStart;
				this->sprite->setSourceRect(this->frames[this->currentFrame]);
			}
		}
	}

	void Animation::setFrameRange(int startCol, int startRow, int endCol, int endRow)
	{
		if (startCol >= 0 && startCol < this->columns && startRow >= 0 && startRow < this->rows &&
			endCol >= 0 && endCol < this->columns && endRow >= 0 && endRow < this->rows)
		{
			int startFrame = startRow * this->columns + startCol;
			int endFrame = endRow * this->columns + endCol;
			setFrameRange(startFrame, endFrame);
		}
	}
	void Animation::update(float deltaTime)
	{
		if (!this->isPlaying || this->frameRangeEnd <= this->frameRangeStart)
			return;

		this->elapsedTime += deltaTime;

		if (this->elapsedTime >= this->frameTime)
		{
			this->elapsedTime -= this->frameTime;
			this->currentFrame++;

			if (this->currentFrame > this->frameRangeEnd)
			{
				if (this->isLooping)
				{
					this->currentFrame = this->frameRangeStart;
				}
				else
				{
					this->currentFrame = this->frameRangeEnd;
					this->isPlaying = false;
				}
			}

			this->sprite->setSourceRect(this->frames[this->currentFrame]);
		}
	}

	void Animation::render(Graphics &graphics) const
	{
		this->sprite->render(graphics);
	}
}