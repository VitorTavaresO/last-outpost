#ifndef __LAST_OUTPOST_COLLISION_H__
#define __LAST_OUTPOST_COLLISION_H__

#include <last-outpost/types.h>

namespace Game
{
	class Collision
	{
	private:
		Vector center;
		float radius;

	public:
		Collision(float x = 0, float y = 0, float radius = 0.5f);
		~Collision() = default;

		void setCollision(float x, float y, float radius);
		void setCollision(const Vector &center, float radius);

		Vector getCenter() const
		{
			return this->center;
		}
		float getRadius() const
		{
			return this->radius;
		}

		bool checkCollision(const Collision &other) const;
	};
}

#endif