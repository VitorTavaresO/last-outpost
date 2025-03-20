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

    void Map::render(SDL_Renderer *renderer, int cellWidth, int cellHeight)
    {
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
                SDL_Rect cell;
                cell.x = col * cellWidth;
                cell.y = row * cellHeight;
                cell.w = cellWidth;
                cell.h = cellHeight;

                if (terrain[row, col] == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Verde
                }
                else if (terrain[row, col] == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Vermelho
                }

                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }

}