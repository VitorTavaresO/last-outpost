#ifndef __LAST_OUTPOST_TYPES_H__
#define __LAST_OUTPOST_TYPES_H__

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
}

#endif