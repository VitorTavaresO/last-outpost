#ifndef _LAST_OUTPOST_MAP_H_
#define _LAST_OUTPOST_MAP_H_

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/graphics.h>
#include <last-outpost/types.h>
#include <last-outpost/sprite.h>
#include <SDL.h>
#include <string>
#include <vector>
#include <utility>
#include <memory>

namespace Game
{
	struct Tile
	{
		int id;
		char type;
		Object object;

		Tile() : id(0), type(' ') {}

		Tile(int id, char type, Object &&obj)
			: id(id), type(type), object(std::move(obj)) {}

		Tile(const Tile &) = delete;
		Tile &operator=(const Tile &) = delete;

		Tile(Tile &&other) noexcept
			: id(other.id), type(other.type), object(std::move(other.object))
		{
		}

		Tile &operator=(Tile &&other) noexcept
		{
			if (this != &other)
			{
				id = other.id;
				type = other.type;
				object = std::move(other.object);
			}
			return *this;
		}
	};

	class Map : public Object
	{
	private:
		int width;
		int height;
		Mylib::Matrix<Tile> terrain;

		std::unique_ptr<Sprite> grassSprite;
		std::unique_ptr<Sprite> pathSprite;
		SDL_Renderer *renderer;

	public:
		Map(int width, int height, const std::string &mapString, SDL_Renderer *renderer = nullptr);

		int getWidth() const
		{
			return this->width;
		}

		int getHeight() const
		{
			return this->height;
		}

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