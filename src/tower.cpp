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
		graphics.drawCircle(getPosition(), range, {0, 0, 255, 40});
		graphics.drawRect(getPosition(), getSize(), getColor());
	}
	bool Tower::isWithinRange(const Vector &targetPosition) const
	{
		float distanceSquared = (this->getPosition() - targetPosition).length_squared();
		return distanceSquared <= (this->range * this->range);
	}
}