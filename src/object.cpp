#include <last-outpost/object.h>
#include <last-outpost/types.h>

namespace Game
{
	void Object::update(float deltaTime)
	{
	}

	void Object::render(Graphics &graphics, float deltaTime) const
	{
		if (this->sprite)
		{
			this->sprite->setPosition(this->position);
			this->sprite->render(graphics);
		}
		else
		{
			graphics.drawRect(this->position, this->size, this->color);
		}
	}
}