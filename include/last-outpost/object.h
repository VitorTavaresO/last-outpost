#ifndef _LAST_OUTPOST_OBJECT_H_
#define _LAST_OUTPOST_OBJECT_H_

#include <SDL.h>
#include <last-outpost/graphics.h>
#include <last-outpost/types.h>

namespace Game
{
	class Object
	{
	private:
		SDL_Color color;
		Vector position;
		Vector size;
		ObjectType type;

	public:
		Object() : type(ObjectType::Unknown) {}
		virtual ~Object() = default;

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
		}

		Vector getPosition() const
		{
			return this->position;
		}

		void setSize(float width, float height)
		{
			this->size = {width, height};
		}

		Vector getSize() const
		{
			return this->size;
		}

		void setType(ObjectType type)
		{
			this->type = type;
		}

		ObjectType getType() const
		{
			return this->type;
		}
	};
}

#endif