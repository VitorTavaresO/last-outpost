#ifndef _LAST_OUTPOST_PROJECTIL_H_
#define _LAST_OUTPOST_PROJECTIL_H_

#include <last-outpost/object.h>
#include <last-outpost/graphics.h>
#include <last-outpost/types.h>
#include <last-outpost/sprite.h>
#include <last-outpost/animation.h>
#include <memory>

namespace Game
{
	class Enemy;

	class Projectil : public Object
	{
	private:
		int damage;
		float speed;
		Vector direction;
		Vector targetPosition;
		Enemy *targetEnemy;
		std::unique_ptr<Sprite> projectileSprite;
		std::unique_ptr<Animation> projectileAnimation;

		std::string animationSpriteAsset;
		int animationSpriteWidth = 0;
		int animationSpriteHeight = 0;
		int animationSpriteCols = 0;
		int animationSpriteRows = 0;
		float animationFrameTime = 0.0f;
		int animationFrameStart = 0;
		int animationFrameEnd = 0;
		float animationScale = 1.0f;
		bool hasAnimationConfig = false;

	public:
		Projectil(int damage = 0, float speed = 0.0f, Vector position = {0, 0}, Vector direction = {0, 0});
		~Projectil() override = default;

		Projectil(const Projectil &) = delete;
		Projectil &operator=(const Projectil &) = delete;

		Projectil(Projectil &&other) noexcept
			: Object(std::move(other)), damage(other.damage), speed(other.speed),
			  direction(other.direction), targetPosition(other.targetPosition),
			  targetEnemy(other.targetEnemy), projectileSprite(std::move(other.projectileSprite)),
			  projectileAnimation(std::move(other.projectileAnimation)),
			  animationSpriteAsset(std::move(other.animationSpriteAsset)),
			  animationSpriteWidth(other.animationSpriteWidth),
			  animationSpriteHeight(other.animationSpriteHeight),
			  animationSpriteCols(other.animationSpriteCols),
			  animationSpriteRows(other.animationSpriteRows),
			  animationFrameTime(other.animationFrameTime),
			  animationFrameStart(other.animationFrameStart),
			  animationFrameEnd(other.animationFrameEnd),
			  animationScale(other.animationScale),
			  hasAnimationConfig(other.hasAnimationConfig)
		{
			other.targetEnemy = nullptr;
		}

		Projectil &operator=(Projectil &&other) noexcept
		{
			if (this != &other)
			{
				Object::operator=(std::move(other));
				damage = other.damage;
				speed = other.speed;
				direction = other.direction;
				targetPosition = other.targetPosition;
				targetEnemy = other.targetEnemy;
				projectileSprite = std::move(other.projectileSprite);
				projectileAnimation = std::move(other.projectileAnimation);
				animationSpriteAsset = std::move(other.animationSpriteAsset);
				animationSpriteWidth = other.animationSpriteWidth;
				animationSpriteHeight = other.animationSpriteHeight;
				animationSpriteCols = other.animationSpriteCols;
				animationSpriteRows = other.animationSpriteRows;
				animationFrameTime = other.animationFrameTime;
				animationFrameStart = other.animationFrameStart;
				animationFrameEnd = other.animationFrameEnd;
				animationScale = other.animationScale;
				hasAnimationConfig = other.hasAnimationConfig;
				other.targetEnemy = nullptr;
			}
			return *this;
		}

		using Object::setPosition;

		void setPosition(const Vector &position)
		{
			setPosition(position.x, position.y);
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

		void setDirection(const Vector &direction)
		{
			this->direction = direction;
		}

		Vector getDirection() const
		{
			return this->direction;
		}

		void setTargetEnemy(Enemy *enemy);
		void updateTargetPosition();

		void setTargetPosition(const Vector &target);

		void setTargetPosition(float x, float y)
		{
			setTargetPosition(Vector{x, y});
		}

		Vector getTargetPosition() const;

		Enemy *getTargetEnemy() const { return targetEnemy; }

		bool loadSprite(SDL_Renderer *renderer, const std::string &spritePath = "assets/sprites/projectile.png");
		bool loadAnimation(SDL_Renderer *renderer, const std::string &spriteAsset,
						   int spriteWidth, int spriteHeight, int spriteCols, int spriteRows,
						   float frameTime, int frameStart, int frameEnd, float scale = 1.0f);

		bool hasAnimation() const { return projectileAnimation != nullptr; }
		bool hasSprite() const { return projectileSprite != nullptr; }

		void cloneVisualConfiguration(const Projectil &source, SDL_Renderer *renderer);

		void update(float deltaTime) override;
		void render(Graphics &graphics, float deltaTime) const override;

		bool hasReachedTarget() const;
	};
}

#endif