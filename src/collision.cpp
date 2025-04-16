#include <last-outpost/collision.h>
#include <cmath>

namespace Game
{
	Collision::Collision(float x, float y, float radius)
		: center{x, y}, radius(radius)
	{
	}

	void Collision::setCollision(float x, float y, float radius)
	{
		this->center = {x, y};
		this->radius = radius;
	}

	void Collision::setCollision(const Vector &center, float radius)
	{
		this->center = center;
		this->radius = radius;
	}

	bool Collision::checkCollision(const Collision &other) const
	{
		Vector distanceVector = this->center - other.center;
		float distanceSquared = distanceVector.length_squared();

		float radiusSum = this->radius + other.radius;

		return distanceSquared < (radiusSum * radiusSum);
	}
}