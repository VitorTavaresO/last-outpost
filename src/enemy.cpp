#include <last-outpost/enemy.h>
#include <last-outpost/types.h>
#include <utility>
#include <iostream>

namespace Game
{
	Enemy::Enemy(int life, int damage, float speed, const std::string &spell, std::vector<PathPoint> path, int goldReward)
		: life(life), damage(damage), speed(speed), spell(spell), path(std::move(path)), lastMoveTime(0), currentStep(0), goldReward(goldReward),
		  walkAnimation(nullptr), idleAnimation(nullptr), currentAnimation(nullptr), state(EnemyState::Idle)
	{
		this->setType(ObjectType::Enemy);
		if (this->path.empty())
		{
			this->currentStep = 0;
			this->setPosition(0, 0);
		}
		else
		{
			this->currentStep = 0;
			this->setPosition(this->path[0].x, this->path[0].y);
		}
	}

	bool Enemy::loadAnimations(SDL_Renderer *renderer)
	{
		walkAnimation = std::make_unique<Animation>("assets/base-enemy.png", renderer, 307, 512, 5, 2);
		walkAnimation->setFrameTime(0.2f);
		walkAnimation->setFrameRange(0, 3);
		walkAnimation->setScale(0.1f, 0.1f);

		idleAnimation = std::make_unique<Animation>("assets/base-enemy.png", renderer, 307, 512, 5, 2);
		idleAnimation->setFrameTime(0.5f);
		idleAnimation->setFrameRange(0, 1);

		setAnimationState(EnemyState::Idle);
		return true;
	}

	bool Enemy::loadAnimations(SDL_Renderer *renderer, const std::string &spriteAsset,
							   int spriteWidth, int spriteHeight, int spriteCols, int spriteRows,
							   float walkFrameTime, float idleFrameTime,
							   int walkFrameStart, int walkFrameEnd,
							   int idleFrameStart, int idleFrameEnd, float scale)
	{
		walkAnimation = std::make_unique<Animation>(spriteAsset, renderer, spriteWidth, spriteHeight, spriteCols, spriteRows);
		walkAnimation->setFrameTime(walkFrameTime);
		walkAnimation->setFrameRange(walkFrameStart, walkFrameEnd);
		walkAnimation->setScale(scale, scale);

		idleAnimation = std::make_unique<Animation>(spriteAsset, renderer, spriteWidth, spriteHeight, spriteCols, spriteRows);
		idleAnimation->setFrameTime(idleFrameTime);
		idleAnimation->setFrameRange(idleFrameStart, idleFrameEnd);
		idleAnimation->setScale(scale, scale);

		setAnimationState(EnemyState::Idle);
		return true;
	}

	void Enemy::setAnimationState(EnemyState newState)
	{
		if (state == newState)
			return;

		state = newState;

		switch (state)
		{
		case EnemyState::Idle:
			currentAnimation = idleAnimation.get();
			break;
		case EnemyState::Walking:
			currentAnimation = walkAnimation.get();
			break;
		default:
			currentAnimation = idleAnimation.get();
			break;
		}

		if (currentAnimation)
		{
			currentAnimation->play(true);
		}
	}

	void Enemy::update(float deltaTime)
	{
		if (this->path.empty() || this->currentStep >= this->path.size())
		{
			setAnimationState(EnemyState::Idle);
		}
		else if (this->currentStep < this->path.size())
		{
			if (this->currentStep + 1 < this->path.size())
			{
				const auto currentPos = this->getPosition();
				const auto nextPos = Point(this->path[this->currentStep + 1].x, this->path[this->currentStep + 1].y);
				const auto direction = nextPos - currentPos;
				const float distance = direction.length();

				const float distanceTraveled = this->speed * deltaTime;

				if (distanceTraveled >= distance)
				{
					this->setPosition(nextPos);
					this->currentStep++;
				}
				else
				{
					const auto newPos = currentPos + Mylib::Math::with_length(direction, distanceTraveled);
					this->setPosition(newPos);
				}

				setAnimationState(EnemyState::Walking);
			}
			else
			{
				this->currentStep++;
				setAnimationState(EnemyState::Idle);
			}
		}

		if (currentAnimation)
		{
			currentAnimation->update(deltaTime);

			auto pos = this->getPosition();
			currentAnimation->setPosition(pos.x, pos.y);
		}
	}
	void Enemy::render(Graphics &graphics, float deltaTime) const
	{
		if (currentAnimation)
		{
			currentAnimation->render(graphics);
		}
		else
		{
			graphics.drawRect(this->getPosition(), {1, 1}, {255, 0, 0, 255});
		}
	}
}