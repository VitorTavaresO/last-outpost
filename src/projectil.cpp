#include <last-outpost/projectil.h>
#include <last-outpost/enemy.h>
#include <cmath>

namespace Game
{
	Projectil::Projectil(int damage, float speed, Vector position, Vector direction)
		: damage(damage), speed(speed), direction(direction), targetEnemy(nullptr)
	{
		this->setType(ObjectType::Projectil);
		this->setPosition(position.x, position.y);
		this->setColor({255, 255, 0, 255});
		this->setSize(0.3f, 0.3f);
	}

	void Projectil::setTargetEnemy(Enemy *enemy)
	{
		this->targetEnemy = enemy;
		if (this->targetEnemy)
		{
			this->updateTargetPosition();
		}
	}

	void Projectil::updateTargetPosition()
	{
		if (!this->targetEnemy)
			return;

		this->targetPosition = this->targetEnemy->getPosition();

		Vector currentPos = this->getPosition();
		Vector directionVector = this->targetPosition - currentPos;

		float length = directionVector.length();
		if (length > 0.0001f)
			this->direction = Mylib::Math::with_length(directionVector, this->speed);
	}

	void Projectil::setTargetPosition(const Vector &target)
	{
		this->targetEnemy = nullptr;
		this->targetPosition = target;

		Vector currentPos = this->getPosition();
		Vector directionVector = this->targetPosition - currentPos;

		float length = directionVector.length();
		if (length > 0.0001f)
			this->direction = Mylib::Math::with_length(directionVector, this->speed);
	}

	Vector Projectil::getTargetPosition() const
	{
		if (this->targetEnemy)
		{
			return this->targetEnemy->getPosition();
		}
		return this->targetPosition;
	}

	void Projectil::update(float deltaTime)
	{
		if (this->targetEnemy)
		{
			this->updateTargetPosition();
		}

		Vector currentPos = this->getPosition();
		Vector targetPos = this->getTargetPosition();

		Vector toTarget = targetPos - currentPos;
		float distanceSquared = toTarget.length_squared();

		if (distanceSquared < 0.01f)
		{
			return;
		}

		currentPos.x += this->direction.x * this->speed * deltaTime;
		currentPos.y += this->direction.y * this->speed * deltaTime;

		this->setPosition(currentPos.x, currentPos.y);
	}

	void Projectil::render(Graphics &graphics, float deltaTime) const
	{
		graphics.drawRect(this->getPosition(), this->getSize(), this->getColor());
	}

	bool Projectil::hasReachedTarget() const
	{
		Vector targetPos = this->getTargetPosition();
		Vector toTarget = targetPos - this->getPosition();
		return toTarget.length_squared() < 0.01f;
	}
}