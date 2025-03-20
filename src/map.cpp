#include <last-outpost/map.h>
#include <SDL.h>

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

    void Map::render(SDL_Renderer *renderer, int tileWidth, int tileHeight)
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                SDL_Rect cell;
                cell.x = col * tileWidth;
                cell.y = row * tileHeight;
                cell.w = tileWidth;
                cell.h = tileHeight;

                if (terrain[row, col] == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                }
                else if (terrain[row, col] == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }

                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }

    void Game::Map::loadFromMatrix(const Mylib::Matrix<int> &matrix)
    {
        for (uint32_t row = 0; row < matrix.get_nrows(); ++row)
        {
            for (uint32_t col = 0; col < matrix.get_ncols(); ++col)
            {
                if (row < static_cast<uint32_t>(height) && col < static_cast<uint32_t>(width))
                {
                    terrain[row, col] = matrix[row, col];
                }
            }
        }
    }
}