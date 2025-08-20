#include <last-outpost/map.h>
#include <iostream>

namespace Game
{
	Map::Map(int width, int height, const std::string &mapString, SDL_Renderer *renderer)
		: width(width), height(height), terrain(height, width), renderer(renderer)
	{
		if (renderer)
		{
			grassSprite = std::make_unique<Sprite>();
			pathSprite = std::make_unique<Sprite>();

			grassSprite->loadFromFile("assets/sprites/environment/grass.jpg", renderer);
			pathSprite->loadFromFile("assets/sprites/environment/path.jpg", renderer);
		}
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
				path.emplace_back(currentCol, currentRow);
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
		int tileWidth = graphics.getTileWidth();
		int tileHeight = graphics.getTileHeight();

		for (uint32_t row = 0; row < this->terrain.get_nrows(); ++row)
		{
			for (uint32_t col = 0; col < this->terrain.get_ncols(); ++col)
			{
				const Tile &tile = this->terrain[row, col];

				if (renderer)
				{
					int x = col * tileWidth;
					int y = row * tileHeight;

					Sprite *spriteToRender = nullptr;

					switch (tile.type)
					{
					case ' ':
						spriteToRender = grassSprite.get();
						break;
					case 'F': // First
					case 'R': // Right
					case 'L': // Left
					case 'D': // Down
					case 'U': // Up
					case 'E': // End
						spriteToRender = pathSprite.get();
						break;
					case 'S':
						spriteToRender = pathSprite.get();
						break;
					default:
						spriteToRender = grassSprite.get();
						break;
					}

					if (spriteToRender)
					{
						SDL_Rect destRect = {x, y, tileWidth, tileHeight};
						spriteToRender->renderAt(renderer, destRect);
					}
					else
					{
						tile.object.render(graphics, deltaTime);
					}
				}
				else
				{
					tile.object.render(graphics, deltaTime);
				}
			}
		}
	}

	void Map::update(float deltaTime)
	{
	}

	const char rawStringMap[] =
		"                                "
		"         S                      "
		"                                "
		"    S   RRRD        S           "
		"        U  D                    "
		"FRRRRRRRU  RRRRRRRRRRRRRRRD     "
		"                          D     "
		"         S           S    D     "
		"                          D     "
		"   S    DLLLLLLLLLLLLLLLLLL     "
		"        D                       "
		"        D    S         S        "
		"        D                       "
		"    S   D                       "
		"        D                       "
		"        D    S                  "
		"        D                       "
		"        E                       ";
}