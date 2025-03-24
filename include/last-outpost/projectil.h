#ifndef __LAST_OUTPOST_PROJECTIL_H__
#define __LAST_OUTPOST_PROJECTIL_H__

#include <SDL.h>
#include <last-outpost/object.h>

namespace Game
{
    class Projectil : public Object
    {
    private:
        int damage;
        float speed;
        SDL_Point position;
        SDL_Point direction;

    public:
        Projectil(int damage = 0, float speed = 0.0f, SDL_Point position = {0, 0}, SDL_Point direction = {0, 0});
        ~Projectil() override = default;

        void setDamage(int damage);
        int getDamage() const;

        void setSpeed(float speed);
        float getSpeed() const;

        void setPosition(int x, int y);
        SDL_Point getPosition() const;

        void setDirection(int x, int y);
        SDL_Point getDirection() const;

        void update(float deltaTime) override;
        void render(SDL_Renderer *renderer, int x, int y, int width, int height) const override;
    };
}

#endif