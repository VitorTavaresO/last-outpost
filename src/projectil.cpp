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
		targetEnemy = enemy;
		if (targetEnemy)
		{
			updateTargetPosition();
		}
	}

	void Projectil::updateTargetPosition()
	{
		if (!targetEnemy)
			return;

		targetPosition = targetEnemy->getPosition();

		Vector currentPos = getPosition();
		Vector directionVector = targetPosition - currentPos;

		float length = directionVector.length();
		if (length > 0.0001f)
		{
			direction = {directionVector.x / length, directionVector.y / length};
		}
	}

	void Projectil::setTargetPosition(const Vector &target)
	{
		targetEnemy = nullptr;
		targetPosition = target;

		Vector currentPos = getPosition();
		Vector directionVector = targetPosition - currentPos;

		float length = directionVector.length();
		if (length > 0.0001f)
		{
			direction = {directionVector.x / length, directionVector.y / length};
		}
	}

	Vector Projectil::getTargetPosition() const
	{
		if (targetEnemy)
		{
			return targetEnemy->getPosition();
		}
		return targetPosition;
	}

	void Projectil::update(float deltaTime)
	{
		if (targetEnemy)
		{
			updateTargetPosition();
		}

		Vector currentPos = getPosition();
		Vector targetPos = getTargetPosition();

		Vector toTarget = targetPos - currentPos;
		float distanceSquared = toTarget.length_squared();

		if (distanceSquared < 0.01f)
		{
			return;
		}

		currentPos.x += direction.x * speed * deltaTime;
		currentPos.y += direction.y * speed * deltaTime;

		setPosition(currentPos.x, currentPos.y);
	}

	void Projectil::render(Graphics &graphics, float deltaTime) const
	{
		graphics.drawRect(getPosition(), getSize(), getColor());
	}

	bool Projectil::hasReachedTarget() const
	{
		Vector targetPos = getTargetPosition();
		Vector toTarget = targetPos - getPosition();
		return toTarget.length_squared() < 0.01f;
	}
}