#ifndef __LAST_OUTPOST_MAP_H__
#define __LAST_OUTPOST_MAP_H__

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <SDL.h>
#include <string>
#include <vector>

namespace Game
{

    class Map : public Object
    {
    private:
        int width;
        int height;
        Mylib::Matrix<int> terrain;

    public:
        Map(int width, int height);
        ~Map() override = default;

        void update() override;
        void render(SDL_Renderer *renderer, int tileWidth, int tileHeight) override;

        int getWidth() const;
        int getHeight() const;

        void setTerrainValue(int row, int col, int value);
        int getTerrainValue(int row, int col) const;

        void loadFromMatrix(const Mylib::Matrix<int> &matrix);
    };

}

#endif