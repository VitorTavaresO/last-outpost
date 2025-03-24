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
        ~Map() override = default;

        void update() override;
        void render(Graphics &graphics) const override;

        void loadFromString(const std::string &mapString, int rows, int cols);

        const Mylib::Matrix<Object> &getTerrain() const;
    };

    extern const std::string rawStringMap;
}

#endif