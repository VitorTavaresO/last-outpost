#ifndef _LAST_OUTPOST_TYPES_H_
#define _LAST_OUTPOST_TYPES_H_

#include <my-lib/math-vector.h>

namespace Game
{
	using Vector = Mylib::Math::Vector<float, 2>;
	using Point = Vector;

	struct PathPoint
	{
		int x;
		int y;

		PathPoint(int x = 0, int y = 0) : x(x), y(y) {}
	};
	enum class ObjectType
	{
		Enemy,
		Space,
		Tower,
		First,
		End,
		Path,
		Projectil,
		Unknown
	};

	enum class EnemyState
	{
		Idle,
		Walking,
		Attacking,
		Dying
	};
}

#endif