#ifndef _LAST_OUTPOST_TOWER_H_
#define _LAST_OUTPOST_TOWER_H_

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/projectil.h>
#include <last-outpost/collision.h>
#include <last-outpost/types.h>

namespace Game
{
	class Tower : public Object
	{
	private:
		float range;
		float damage;
		Projectil projectile;

	public:
		Tower(float range = 0.0f, float damage = 0.0f, const Projectil &projectile = Projectil())
			: range(range), damage(damage), projectile(projectile)
		{
			this->setType(ObjectType::Tower);
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

		void setProjectile(const Projectil &projectile) const
		{
			this->projectile = projectile;
		}
		const Projectil &getProjectile() const;
		{
			return projectile;
		}

		void update(float deltaTime) override;
		void render(Graphics &graphics, float deltaTime) const override;
	};
}

#endif