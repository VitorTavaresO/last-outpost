#ifndef LAST_OUTPOST_TOWER_H
#define LAST_OUTPOST_TOWER_H

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/types.h>

namespace Game
{
	class Tower : public Object
	{
	private:
		float range;
		float damage;

	public:
		Tower(float range = 0.0f, float damage = 0.0f, SDL_Color color = {0, 0, 255, 255})
			: range(range), damage(damage)
		{
			this->setType(ObjectType::Tower);
			this->setColor(color);
			this->setSize(1, 1);
		}

		void setRange(int range)
		{
			this->range = range;
		}

		float getRange() const
		{
			return this->range;
		}

		void setDamage(float damage)
		{
			this->damage = damage;
		}

		float getDamage() const
		{
			return this->damage;
		}

		void update(float deltaTime) override;
		void render(Graphics &graphics, float deltaTime) const override;
	};
}

#endif