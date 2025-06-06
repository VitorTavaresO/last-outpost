#ifndef LAST_OUTPOST_TOWER_H
#define LAST_OUTPOST_TOWER_H

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/types.h>
#include <last-outpost/projectil.h>
#include <memory>
#include <vector>

namespace Game
{
	class Enemy;

	class Tower : public Object
	{
	private:
		float range;
		float lastFireTime;
		float fireRate;
		Projectil projectil;

	public:
		Tower(float range = 0.0f,
			  Projectil projectil = Projectil(),
			  SDL_Color color = {0, 0, 255, 255})
			: range(range), lastFireTime(0), fireRate(1.0f),
			  projectil(std::move(projectil))
		{
			this->setType(ObjectType::Tower);
			this->setColor(color);
			this->setSize(1, 1);
		}

		Tower(const Tower &) = delete;
		Tower &operator=(const Tower &) = delete;

		Tower(Tower &&other) noexcept
			: Object(std::move(other)), range(other.range),
			  lastFireTime(other.lastFireTime), fireRate(other.fireRate),
			  projectil(std::move(other.projectil))
		{
		}

		Tower &operator=(Tower &&other) noexcept
		{
			if (this != &other)
			{
				Object::operator=(std::move(other));
				range = other.range;
				lastFireTime = other.lastFireTime;
				fireRate = other.fireRate;
				projectil = std::move(other.projectil);
			}
			return *this;
		}

		void setRange(float range)
		{
			this->range = range;
		}
		float getRange() const
		{
			return this->range;
		}

		void setFireRate(float rate)
		{
			this->fireRate = rate;
		}
		float getFireRate() const
		{
			return this->fireRate;
		}

		void setProjectil(Projectil projectil)
		{
			this->projectil = std::move(projectil);
		}

		const Projectil &getProjectil() const
		{
			return this->projectil;
		}

		std::unique_ptr<Projectil> findTargetAndTryFireAt(const std::vector<std::unique_ptr<Enemy>> &enemies, float currentTime);
		bool isWithinRange(const Vector &targetPosition) const;
		std::unique_ptr<Projectil> tryFireAt(Enemy *enemy, float currentTime);

		void update(float deltaTime) override;
		void render(Graphics &graphics, float deltaTime) const override;
	};
}

#endif