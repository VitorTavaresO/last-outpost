#ifndef __LAST_OUTPOST_MAP_H__
#define __LAST_OUTPOST_MAP_H__

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/graphics.h>
#include <last-outpost/types.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <utility>

namespace Game
{
	struct Tile
	{
		int id;
		char type;
		Object object;
	};

	class Map : public Object
	{
	private:
		int width;
		int height;
		Mylib::Matrix<Tile> terrain;

	public:
		Map(int width, int height, const std::string &mapString);

		void update(float deltaTime) override final;
		void render(Graphics &graphics, float deltaTime) const override final;

		inline Tile &operator()(const uint32_t row, const uint32_t col)
		{
			return this->terrain[row, col];
		}

		inline const Tile &operator()(const uint32_t row, const uint32_t col) const
		{
			return this->terrain[row, col];
		}

		std::vector<PathPoint> extractPath() const;
	};

	extern const char rawStringMap[];
}

#endif