#ifndef _LAST_OUTPOST_OBJECT_H_
#define _LAST_OUTPOST_OBJECT_H_

#include <SDL.h>
#include <memory>
#include <last-outpost/graphics.h>
#include <last-outpost/types.h>
#include <last-outpost/collision.h>
#include <last-outpost/sprite.h>

namespace Game
{
	class Object
	{
	private:
		SDL_Color color;
		Vector position;
		Vector size;
		ObjectType type;
		Collision collider;
		float collisionRadius;
		std::unique_ptr<Sprite> sprite;

	public:
		Object() : type(ObjectType::Unknown), collisionRadius(0.5f)
		{
			updateCollider();
		}

		virtual ~Object() = default;

		Object(const Object &) = delete;
		Object &operator=(const Object &) = delete;

		Object(Object &&other) noexcept
			: color(other.color), position(other.position), size(other.size),
			  type(other.type), collider(other.collider),
			  collisionRadius(other.collisionRadius), sprite(std::move(other.sprite))
		{
		}

		Object &operator=(Object &&other) noexcept
		{
			if (this != &other)
			{
				color = other.color;
				position = other.position;
				size = other.size;
				type = other.type;
				collider = other.collider;
				collisionRadius = other.collisionRadius;
				sprite = std::move(other.sprite);
			}
			return *this;
		}

		virtual void update(float deltaTime);
		virtual void render(Graphics &graphics, float deltaTime) const;

		void setColor(const SDL_Color &color)
		{
			this->color = color;
		}

		SDL_Color getColor() const
		{
			return this->color;
		}

		void setPosition(float x, float y)
		{
			this->position = {x, y};
			updateCollider();
		}

		Vector getPosition() const
		{
			return this->position;
		}

		void setSize(float width, float height)
		{
			this->size = {width, height};
			collisionRadius = (width + height) / 4.0f;
			updateCollider();
		}

		Vector getSize() const
		{
			return this->size;
		}

		void setCollisionRadius(float radius)
		{
			this->collisionRadius = radius;
			updateCollider();
		}

		float getCollisionRadius() const
		{
			return this->collisionRadius;
		}

		void setType(ObjectType type)
		{
			this->type = type;
		}

		ObjectType getType() const
		{
			return this->type;
		}

		void updateCollider()
		{
			collider.setCollision(position.x, position.y, collisionRadius);
		}

		const Collision &getCollider() const
		{
			return collider;
		}

		bool isColliding(const Object &other) const
		{
			return collider.checkCollision(other.getCollider());
		}

		void setSprite(std::unique_ptr<Sprite> newSprite)
		{
			sprite = std::move(newSprite);
		}

		bool hasSprite() const
		{
			return sprite != nullptr;
		}

		Sprite *getSprite() const
		{
			return sprite.get();
		}
	};
}

#endif