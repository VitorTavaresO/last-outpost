#include <last-outpost/map.h>

namespace Game
{
	Map::Map(int width, int height, const std::string &mapString)
		: width(width), height(height), terrain(height, width)
	{
		int idCounter = 0;

		for (int row = 0; row < this->height; ++row)
		{
			for (int col = 0; col < this->width; ++col)
			{
				char tileChar = mapString[row * this->width + col];

				Object tileObject;
				tileObject.setPosition(col, row);
				tileObject.setSize(1, 1);

				switch (tileChar)
				{
				case ' ':
					tileObject.setColor({0, 100, 0, 255}); // Verde escuro
					tileObject.setType(ObjectType::Unknown);
					break;

				case 'F':
					tileObject.setColor({194, 178, 128, 255}); // Areia
					tileObject.setType(ObjectType::First);
					break;

				case 'R':
				case 'L':
				case 'D':
				case 'U':
					tileObject.setColor({194, 178, 128, 255}); // Areia
					tileObject.setType(ObjectType::Path);
					break;

				case 'E':
					tileObject.setColor({194, 178, 128, 255}); // Areia
					tileObject.setType(ObjectType::End);
					break;

				case 'S':
					tileObject.setColor({139, 69, 19, 255}); // Marrom
					tileObject.setType(ObjectType::Space);
					break;

				default:
					tileObject.setColor({0, 0, 0, 255}); // Preto
					tileObject.setType(ObjectType::Unknown);
					break;
				}

				Tile tile(idCounter++, tileChar, std::move(tileObject));

				this->terrain[row, col] = std::move(tile);
			}
		}
	}

	std::vector<PathPoint> Map::extractPath() const
	{
		std::vector<PathPoint> path;

		int currentRow = -1, currentCol = -1;

		for (int row = 0; row < this->height; ++row)
		{
			for (int col = 0; col < this->width; ++col)
			{
				if (this->terrain[row, col].type == 'F')
				{
					currentRow = row;
					currentCol = col;
					path.emplace_back(currentCol, currentRow);
					break;
				}
			}
			if (currentRow != -1)
				break;
		}

		if (currentRow == -1 || currentCol == -1)
		{
			return path;
		}

		const std::vector<std::pair<int, int>> directions = {
			{0, 1},
			{0, -1},
			{1, 0},
			{-1, 0}};

		for (const auto &[dRow, dCol] : directions)
		{
			int newRow = currentRow + dRow;
			int newCol = currentCol + dCol;

			if (newRow >= 0 && newRow < this->height && newCol >= 0 && newCol < this->width)
			{
				char neighborType = this->terrain[newRow, newCol].type;
				if (neighborType == 'R' || neighborType == 'L' || neighborType == 'D' || neighborType == 'U')
				{
					currentRow = newRow;
					currentCol = newCol;
					path.emplace_back(currentCol, currentRow);
					break;
				}
			}
		}

		while (true)
		{
			char currentType = this->terrain[currentRow, currentCol].type;

			if (currentType == 'E')
			{
				break;
			}

			if (currentType == 'R')
			{
				++currentCol;
			}
			else if (currentType == 'L')
			{
				--currentCol;
			}
			else if (currentType == 'D')
			{
				++currentRow;
			}
			else if (currentType == 'U')
			{
				--currentRow;
			}
			else
			{
				break;
			}

			path.emplace_back(currentCol, currentRow);
		}

		return path;
	}

	void Map::render(Graphics &graphics, float deltaTime) const
	{
		for (uint32_t row = 0; row < this->terrain.get_nrows(); ++row)
		{
			for (uint32_t col = 0; col < this->terrain.get_ncols(); ++col)
			{
				this->terrain[row, col].object.render(graphics, deltaTime);
			}
		}
	}

	void Map::update(float deltaTime)
	{
	}

	const char rawStringMap[] =
		"                                "
		"                                "
		"                                "
		"    S   RRRD                    "
		"        U  D                    "
		"FRRRRRRRU  RRRRRRRRRRRRRRRD     "
		"            S             D     "
		"                          D     "
		"   S    DLLLLLLLLLLLLLLLLLL     "
		"        D                       "
		"        D              S        "
		"        D                       "
		"    S   D                       "
		"        D                       "
		"        D                       "
		"        D                       "
		"        D                       "
		"        E                       ";
}