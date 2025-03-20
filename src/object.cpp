#include <last-outpost/object.h>
#include <iostream>

namespace Game
{

    Object::Object()
    {
    }

    void Object::update()
    {
        std::cout << "Object::update() chamado" << std::endl;
    }

    void Object::render(SDL_Renderer *renderer, int cellWidth, int cellHeight)
    {
        std::cout << "Object::render() chamado" << std::endl;
    }

}