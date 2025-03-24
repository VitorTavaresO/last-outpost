#ifndef __LAST_OUTPOST_MAP_H__
#define __LAST_OUTPOST_MAP_H__

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/graphics.h>
#include <SDL.h>
#include <string>

namespace Game
{
    class Map : public Object
    {
    private:
        int width;
        int height;
        Mylib::Matrix<Object> terrain;

    public:
        Map(int width, int height);

        void update() override final;
        void render(Graphics &graphics) const override final;

        void loadFromString(const std::string &mapString, int rows, int cols);

        const Mylib::Matrix<Object> &getTerrain() const; // Sobrecarga do operador []
    };

    extern const char rawStringMap[];
}

#endif