#include <last-outpost/map.h>

namespace Game
{
    Map::Map(int width, int height)
        : width(width), height(height), terrain(height, width)
    {
    }
    void Map::loadFromString(const std::string &mapString, int rows, int cols)
    {
        terrain = Mylib::Matrix<Object>(rows, cols);

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                char tile = mapString[row * cols + col];

                Object obj;
                obj.setPosition(col, row);
                obj.setSize(1, 1);

                switch (tile)
                {
                case ' ':
                    obj.setColor({0, 100, 0, 255}); // Verde escuro
                    break;

                case 'P':
                    obj.setColor({194, 178, 128, 255}); // Areia
                    break;

                case 'S':
                    obj.setColor({139, 69, 19, 255}); // Marrom
                    break;

                case 'T':
                    obj.setColor({64, 64, 64, 255}); // Cinza
                    break;

                default:
                    obj.setColor({0, 0, 0, 255}); // Preto
                    break;
                }

                terrain[row, col] = obj;
            }
        }
    }

    void Map::render(Graphics &graphics) const
    {
        for (uint32_t row = 0; row < terrain.get_nrows(); ++row)
        {
            for (uint32_t col = 0; col < terrain.get_ncols(); ++col)
            {
                terrain[row, col].render(graphics);
            }
        }
    }

    void Map::update()
    {
    }

    const char rawStringMap[] =
        "        P       "
        "        P       "
        "        P       "
        "    T   P       "
        "        P       "
        "        P       "
        "        P   S   "
        "        P       "
        "        P       ";
}