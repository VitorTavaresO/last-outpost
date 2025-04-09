#include <last-outpost/object.h>
#include <last-outpost/types.h>

namespace Game
{
	void Object::update(float deltaTime)
	{
	}

	void Object::render(Graphics &graphics, float deltaTime) const
	{
		graphics.drawRect(position, size, color);
	}
}