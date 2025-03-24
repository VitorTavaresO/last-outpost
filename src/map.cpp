#include <last-outpost/map.h>
#include <stdexcept>

namespace Game
{
    Map::Map(int width, int height)
        : width(width), height(height), terrain(width, height)
    {
    }

    void Map::update()
    {
    }

    int Map::getWidth() const
    {
        return width;
    }

    int Map::getHeight() const
    {
        return height;
    }

    void Map::loadFromString(const std::string &mapString, int rows, int cols)
    {
        if (rows * cols != static_cast<int>(mapString.size()))
        {
            throw std::invalid_argument("Map string size does not match dimensions.");
        }

        terrain = Mylib::Matrix<Object>(rows, cols);

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                char tile = mapString[row * cols + col];
                SDL_Color color;
                switch (tile)
                {
                case ' ':
                    color = {0, 100, 0, 255}; // Verde escuro
                    break;
                case 'P':
                    color = {194, 178, 128, 255}; // Areia
                    break;
                case 'S':
                    color = {139, 69, 19, 255}; // Marrom
                    break;
                case 'T':
                    color = {128, 128, 128, 255}; // Cinza
                    break;
                default:
                    color = {0, 0, 0, 255}; // Preto (valor padrão)
                    break;
                }

                terrain[row, col].setColor(color);
            }
        }
    }

    const Mylib::Matrix<Object> &Map::getTerrain() const
    {
        return terrain;
    }

    const std::string rawStringMap =
        "                              "
        "        P                     "
        "        P                     "
        "    S   P                     "
        "        P                     "
        "        P                     "
        "        P                     "
        "                              ";
}