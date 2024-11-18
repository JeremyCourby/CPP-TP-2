module;
#include <SFML/Graphics.hpp>
#include <memory>

using namespace sf;
using namespace std;

export module Projectile;

export class Projectile {
public:
    Sprite sprite;
    shared_ptr<Texture> texture;
    float speed{500.0f};

    Projectile() = default;
    explicit Projectile(shared_ptr<Texture> sharedTexture, float startX, float startY): texture(sharedTexture) {
        sprite.setTexture(*texture);
        sprite.setPosition(startX, startY);
        sprite.scale(4.0f, 4.0f);
    }

    void update(float deltaTime) {
        sprite.move(0, -speed * deltaTime);
    }
};
