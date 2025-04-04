#include <last-outpost/object.h>
#include <last-outpost/types.h>

namespace Game
{
	Object::Object()
		: color({0, 0, 0, 255}), position({0, 0}), size({1, 1})
	{
	}

	void Object::update(float deltaTime)
	{
	}

	void Object::render(Graphics &graphics, float deltaTime) const
	{
		graphics.drawRect(position, size, color);
	}

}