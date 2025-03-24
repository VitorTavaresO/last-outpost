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
        Mylib::Matrix<Object> terrain;

    public:
        Map(int width, int height);
        ~Map() override = default;

        void update() override;

        int getWidth() const;
        int getHeight() const;

        void loadFromString(const std::string &mapString, int rows, int cols);

        const Mylib::Matrix<Object> &getTerrain() const;
    };

    extern const std::string rawStringMap;
}

#endif