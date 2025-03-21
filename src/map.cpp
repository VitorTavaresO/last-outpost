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

    void Game::Map::render(SDL_Renderer *renderer, int tileWidth, int tileHeight)
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                SDL_Rect tile;
                tile.x = col * tileWidth;
                tile.y = row * tileHeight;
                tile.w = tileWidth;
                tile.h = tileHeight;

                int terrainValue = terrain[row, col];
                if (terrainValue == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // Verde escuro
                }
                else if (terrainValue == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 194, 178, 128, 255); // Areia
                }
                else if (terrainValue == 2)
                {
                    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrom
                }
                else if (terrainValue == 3)
                {
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Cinza
                }

                SDL_RenderFillRect(renderer, &tile);
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