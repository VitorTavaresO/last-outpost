#ifndef __LAST_OUTPOST_ENEMY_H__
#define __LAST_OUTPOST_ENEMY_H__

#include <last-outpost/object.h>
#include <last-outpost/types.h>
#include <last-outpost/animation.h>
#include <string>
#include <vector>
#include <utility>
#include <memory>

namespace Game
{
	class Enemy : public Object
	{
	private:
		int life;
		int damage;
		float speed;
		std::string spell;
		std::vector<PathPoint> path;
		uint32_t lastMoveTime;
		size_t currentStep;
		int goldReward; // Quantidade de ouro que este inimigo dá ao morrer

		// Animation support
		std::unique_ptr<Animation> walkAnimation;
		std::unique_ptr<Animation> idleAnimation;
		Animation *currentAnimation;
		EnemyState state;

	public:
		Enemy(int life = 100, int damage = 10, float speed = 1.0f, const std::string &spell = "", std::vector<PathPoint> path = {}, int goldReward = 10);

		Enemy(const Enemy &) = delete;
		Enemy &operator=(const Enemy &) = delete;

		Enemy(Enemy &&other) noexcept
			: Object(std::move(other)), life(other.life), damage(other.damage),
			  speed(other.speed), spell(std::move(other.spell)),
			  path(std::move(other.path)), lastMoveTime(other.lastMoveTime),
			  currentStep(other.currentStep), goldReward(other.goldReward)
		{
		}

		Enemy &operator=(Enemy &&other) noexcept
		{
			if (this != &other)
			{
				Object::operator=(std::move(other));
				life = other.life;
				damage = other.damage;
				speed = other.speed;
				spell = std::move(other.spell);
				path = std::move(other.path);
				lastMoveTime = other.lastMoveTime;
				currentStep = other.currentStep;
				goldReward = other.goldReward;
			}
			return *this;
		}

		using Object::setPosition;

		void setPosition(const Point &position)
		{
			setPosition(position.x, position.y);
		}

		void setLife(int life)
		{
			this->life = life;
		}

		int getLife() const
		{
			return this->life;
		}

		void setDamage(int damage)
		{
			this->damage = damage;
		}

		int getDamage() const
		{
			return this->damage;
		}

		void setSpeed(float speed)
		{
			this->speed = speed;
		}

		float getSpeed() const
		{
			return this->speed;
		}

		void setSpell(const std::string &spell)
		{
			this->spell = spell;
		}
		const std::string &getSpell() const
		{
			return this->spell;
		}

		void setGoldReward(int reward) { goldReward = reward; }
		int getGoldReward() const { return goldReward; }

		// Animation methods
		bool loadAnimations(SDL_Renderer *renderer);
		bool loadAnimations(SDL_Renderer *renderer, const std::string &spriteAsset,
							int spriteWidth, int spriteHeight, int spriteCols, int spriteRows,
							float walkFrameTime, float idleFrameTime,
							int walkFrameStart, int walkFrameEnd,
							int idleFrameStart, int idleFrameEnd, float scale);
		void setAnimationState(EnemyState newState);
		EnemyState getAnimationState() const { return state; }

		void update(float deltaTime) override;
		void render(Graphics &graphics, float deltaTime) const override;
	};
}

#endif