#include <last-outpost/map.h>

namespace Game
{
    Map::Map(int width, int height, const std::string &mapString)
        : width(width), height(height), terrain(height, width)
    {
        int idCounter = 0;

        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                char tileChar = mapString[row * width + col];

                Tile tile;
                tile.id = idCounter++;
                tile.type = tileChar;
                tile.object.setPosition(col, row);
                tile.object.setSize(1, 1);

                switch (tileChar)
                {
                case ' ':
                    tile.object.setColor({0, 100, 0, 255}); // Verde escuro
                    break;

                case 'F':
                case 'R':
                case 'L':
                case 'D':
                case 'U':
                case 'E':
                    tile.object.setColor({194, 178, 128, 255}); // Areia
                    break;

                case 'S':
                    tile.object.setColor({139, 69, 19, 255}); // Marrom
                    break;

                case 'T':
                    tile.object.setColor({64, 64, 64, 255}); // Cinza
                    break;

                default:
                    tile.object.setColor({0, 0, 0, 255}); // Preto
                    break;
                }

                terrain[row, col] = tile;
            }
        }
    }

    std::vector<std::pair<int, int>> Map::extractPath() const
    {
        std::vector<std::pair<int, int>> path;

        int currentRow = -1, currentCol = -1;

        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                if (terrain[row, col].type == 'F')
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

            if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width)
            {
                char neighborType = terrain[newRow, newCol].type;
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
            char currentType = terrain[currentRow, currentCol].type;

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

    void Map::render(Graphics &graphics) const
    {
        for (uint32_t row = 0; row < terrain.get_nrows(); ++row)
        {
            for (uint32_t col = 0; col < terrain.get_ncols(); ++col)
            {
                terrain[row, col].object.render(graphics);
            }
        }
    }

    void Map::update()
    {
    }

    const char rawStringMap[] =
        "                                "
        "                                "
        "                                "
        "    T   RRRD                    "
        "        U  D                    "
        "FRRRRRRRU  RRRRRRRRRRRRRRRD     "
        "            S             D     "
        "        DLLLLLLLLLLLLLLLLLL     "
        "   S    D                       "
        "        D                       "
        "        D              S        "
        "        D                       "
        "    T   D                       "
        "        D                       "
        "        D                       "
        "        D                       "
        "        D                       "
        "        E                       ";
}