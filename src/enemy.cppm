module;
#include <SFML/Graphics.hpp>
#include <memory>

using namespace sf;
using namespace std;

export module Enemy;

export class Enemy {
public:
    Sprite sprite;
    shared_ptr<Texture> texture;
    float speed = 50.0f;

    Enemy(shared_ptr<Texture> sharedTexture, auto startX, auto startY, auto scale = 4.0f, auto speed = 50.0f): texture(sharedTexture), speed(speed) {
        sprite.setTexture(*texture);
        sprite.setPosition(startX, startY);
        sprite.scale(scale, scale);
    }

    float getSpeed(){ return speed; }
    void setSpeed(const float speed){ this->speed = speed; }

    void update(float deltaTime) {
        sprite.move(0, speed * deltaTime);
    }
};
