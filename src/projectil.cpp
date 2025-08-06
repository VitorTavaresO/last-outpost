#include <last-outpost/projectil.h>
#include <last-outpost/enemy.h>
#include <cmath>

namespace Game
{
	Projectil::Projectil(int damage, float speed, Vector position, Vector direction)
		: damage(damage), speed(speed), direction(direction), targetEnemy(nullptr),
		  projectileSprite(std::make_unique<Sprite>()), projectileAnimation(nullptr)
	{
		this->setType(ObjectType::Projectil);
		this->setPosition(position.x, position.y);
		this->setColor({255, 255, 0, 255});
		this->setSize(0.3f, 0.3f);
	}

	void Projectil::setTargetEnemy(Enemy *enemy)
	{
		this->targetEnemy = enemy;
		if (this->targetEnemy)
		{
			this->updateTargetPosition();
		}
	}

	void Projectil::updateTargetPosition()
	{
		if (!this->targetEnemy)
			return;

		this->targetPosition = this->targetEnemy->getPosition();

		Vector currentPos = this->getPosition();
		Vector directionVector = this->targetPosition - currentPos;

		float length = directionVector.length();
		if (length > 0.0001f)
			this->direction = Mylib::Math::with_length(directionVector, this->speed);
	}

	void Projectil::setTargetPosition(const Vector &target)
	{
		this->targetEnemy = nullptr;
		this->targetPosition = target;

		Vector currentPos = this->getPosition();
		Vector directionVector = this->targetPosition - currentPos;

		float length = directionVector.length();
		if (length > 0.0001f)
			this->direction = Mylib::Math::with_length(directionVector, this->speed);
	}

	Vector Projectil::getTargetPosition() const
	{
		if (this->targetEnemy)
		{
			return this->targetEnemy->getPosition();
		}
		return this->targetPosition;
	}

	bool Projectil::loadSprite(SDL_Renderer *renderer, const std::string &spritePath)
	{
		if (!projectileSprite)
		{
			projectileSprite = std::make_unique<Sprite>();
		}

		bool success = projectileSprite->loadFromFile(spritePath, renderer);
		if (success)
		{
			projectileSprite->setScale(0.05f, 0.05f);
		}
		return success;
	}

	bool Projectil::loadAnimation(SDL_Renderer *renderer, const std::string &spriteAsset,
								  int spriteWidth, int spriteHeight, int spriteCols, int spriteRows,
								  float frameTime, int frameStart, int frameEnd, float scale)
	{
		animationSpriteAsset = spriteAsset;
		animationSpriteWidth = spriteWidth;
		animationSpriteHeight = spriteHeight;
		animationSpriteCols = spriteCols;
		animationSpriteRows = spriteRows;
		animationFrameTime = frameTime;
		animationFrameStart = frameStart;
		animationFrameEnd = frameEnd;
		animationScale = scale;
		hasAnimationConfig = true;

		projectileAnimation = std::make_unique<Animation>(spriteAsset, renderer, spriteWidth, spriteHeight, spriteCols, spriteRows);
		if (!projectileAnimation->isValid())
		{
			projectileAnimation.reset();
			hasAnimationConfig = false;
			return false;
		}

		projectileAnimation->setFrameTime(frameTime);
		projectileAnimation->setFrameRange(frameStart, frameEnd);
		projectileAnimation->setScale(scale, scale);
		projectileAnimation->play(true);

		projectileSprite.reset();

		return true;
	}

	void Projectil::cloneVisualConfiguration(const Projectil &source, SDL_Renderer *renderer)
	{
		projectileSprite.reset();
		projectileAnimation.reset();

		if (source.hasAnimationConfig)
		{
			loadAnimation(renderer, source.animationSpriteAsset,
						  source.animationSpriteWidth, source.animationSpriteHeight,
						  source.animationSpriteCols, source.animationSpriteRows,
						  source.animationFrameTime, source.animationFrameStart,
						  source.animationFrameEnd, source.animationScale);
		}
		else if (source.hasSprite())
		{
			loadSprite(renderer, "assets/sprites/projectile.png");
		}
	}

	void Projectil::update(float deltaTime)
	{
		if (projectileAnimation)
		{
			projectileAnimation->update(deltaTime);
		}

		if (this->targetEnemy)
		{
			this->updateTargetPosition();
		}

		Vector currentPos = this->getPosition();
		Vector targetPos = this->getTargetPosition();

		Vector toTarget = targetPos - currentPos;
		float distanceSquared = toTarget.length_squared();

		if (distanceSquared < 0.01f)
		{
			return;
		}

		currentPos.x += this->direction.x * this->speed * deltaTime;
		currentPos.y += this->direction.y * this->speed * deltaTime;

		this->setPosition(currentPos.x, currentPos.y);
	}

	void Projectil::render(Graphics &graphics, float deltaTime) const
	{
		if (projectileAnimation)
		{
			projectileAnimation->setPosition(this->getPosition().x, this->getPosition().y);
			projectileAnimation->render(graphics);
		}
		else if (projectileSprite)
		{
			projectileSprite->setPosition(this->getPosition());
			projectileSprite->render(graphics);
		}
		else
		{
			graphics.drawRect(this->getPosition(), this->getSize(), this->getColor());
		}
	}

	bool Projectil::hasReachedTarget() const
	{
		Vector targetPos = this->getTargetPosition();
		Vector toTarget = targetPos - this->getPosition();
		return toTarget.length_squared() < 0.01f;
	}
}