#ifndef __LAST_OUTPOST_PROJECTIL_H__
#define __LAST_OUTPOST_PROJECTIL_H__

#include <last-outpost/object.h>
#include <my-lib/math-vector.h>

namespace Game
{
    class Projectil : public Object
    {
    private:
        int damage;
        MyLib::Vector2 direction;

    public:
        Projectil(int damage = 0, float speed = 0.0f, MyLib::Vector2 position = {0, 0}, MyLib::Vector2 direction = {0, 0});
        ~Projectil() override = default;

        void setDamage(int damage);
        int getDamage() const;

        void setSpeed(float speed);
        float getSpeed() const;

        void setPosition(float x, float y);
        MyLib::Vector2 getPosition() const;

        void setDirection(float x, float y);
        MyLib::Vector2 getDirection() const;

        void update() override;
        void render(Graphics &graphics) const override;
    };
}

#endif