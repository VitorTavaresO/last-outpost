#include <last-outpost/map.h>
#include <stdexcept>

namespace Game
{
    Map::Map(int width, int height)
        : width(width), height(height), terrain(width, height)
    {
        terrain.set_all(0);
    }

    void Map::setTerrainValue(int row, int col, int value)
    {
        terrain[row, col] = value;
    }

    int Map::getTerrainValue(int row, int col) const
    {
        return terrain[row, col];
    }

    void Map::update()
    {
    }

    void Map::loadFromString(const std::string &mapString, int rows, int cols)
    {

        terrain = Mylib::Matrix<int>(rows, cols);

        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                char tile = mapString[row * cols + col];
                terrain[row, col] = tile;
            }
        }
    }

    Mylib::Matrix<SDL_Color> Map::getTileColors() const
    {
        Mylib::Matrix<SDL_Color> colors(height, width);

        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                char tile = static_cast<char>(terrain[row, col]);
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

                colors[row, col] = color;
            }
        }

        return colors;
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