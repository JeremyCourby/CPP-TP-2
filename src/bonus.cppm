module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

using namespace sf;
using namespace std;

export module Bonus;

export class Bonus {
public:
    Sprite sprite;
    bool isActive{false};
    float speed{200.0f};
    int type{0};

    Bonus() = default;

    void spawn(auto position, auto texture, auto scale) {
        sprite.setPosition(position);
        sprite.setTexture(*texture);
        sprite.scale(scale, scale);
        isActive = true;
    }

    void update(auto deltaTime) {
        if (isActive) {
            sprite.move(0, speed * deltaTime);
            // Vérifie si le bonus sort de l'écran
            if (sprite.getPosition().y > 1080) {
                isActive = false;
            }
        }
    }

    void draw(auto &window) {
        if (isActive) {
            window.draw(sprite);
        }
    }
};
