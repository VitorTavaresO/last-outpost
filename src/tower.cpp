#include <last-outpost/tower.h>
#include <last-outpost/graphics.h>
#include <cmath>

namespace Game
{
	void Tower::update(float deltaTime)
	{
	}

	void Tower::render(Graphics &graphics, float deltaTime) const
	{
		graphics.drawRect(getPosition(), getSize(), {0, 0, 255, 255});
	}
}