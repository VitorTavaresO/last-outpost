#include <last-outpost/map.h>

namespace Game
{
    Map::Map(int width, int height)
        : width(width), height(height), terrain(width, height)
    {
    }

    void Map::update()
    {
        for (uint32_t row = 0; row < terrain.get_nrows(); ++row)
        {
            for (uint32_t col = 0; col < terrain.get_ncols(); ++col)
            {
                terrain[row, col].update();
            }
        }
    }

    void Map::render(Graphics &graphics) const
    {
        for (uint32_t row = 0; row < terrain.get_nrows(); ++row)
        {
            for (uint32_t col = 0; col < terrain.get_ncols(); ++col)
            {
                terrain[row, col].render(graphics, {static_cast<int>(col), static_cast<int>(row)}, {1, 1});
            }
        }
    }

    void Map::loadFromString(const std::string &mapString, int rows, int cols)
    {
        terrain = Mylib::Matrix<Object>(rows, cols);

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                char tile = mapString[row * cols + col];

                switch (tile)
                {
                case ' ':
                    terrain[row, col] = Object();
                    terrain[row, col].setColor({0, 100, 0, 255}); // Verde escuro
                    break;

                case 'P':
                    terrain[row, col] = Object();
                    terrain[row, col].setColor({194, 178, 128, 255}); // Areia
                    break;

                case 'S':
                    terrain[row, col] = Object();
                    terrain[row, col].setColor({139, 69, 19, 255}); // Marrom
                    break;

                case 'T':
                    terrain[row, col] = Object();
                    terrain[row, col].setColor({64, 64, 64, 255}); // Cinza
                    break;

                default:
                    terrain[row, col] = Object();
                    terrain[row, col].setColor({0, 0, 0, 255}); // Preto
                    break;
                }
            }
        }
    }

    const Mylib::Matrix<Object> &Map::getTerrain() const
    {
        return terrain;
    }

    const std::string rawStringMap =
        "PPPPPPPPPPPPPPPP"
        "PPPPPPPPPPPPPPPP"
        "        P       "
        "    T   P       "
        "        P       "
        "        P       "
        "        P   S   "
        "        P       "
        "        P       ";
}