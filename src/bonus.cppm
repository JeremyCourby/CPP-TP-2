module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

using namespace sf;
using namespace std;

import Player;

export module Bonus;

export class Bonus {
protected:
    Sprite sprite;
    bool isActive = true;
    float speed{200.0f};

public:
    Bonus(const shared_ptr<Texture> texture) {
        sprite.setTexture(*texture);
    }

    virtual ~Bonus() = default;

    virtual void applyEffect(Player& player) = 0;

    void update(float deltaTime) {
        if (isActive) {
            sprite.move(0, speed * deltaTime);
            if (sprite.getPosition().y > 1080) {
                isActive = false;
            }
        }
    }

    virtual void spawn(const sf::Vector2f& position) {
        sprite.setPosition(position);
        isActive = true;
    }

    const sf::Sprite& getSprite() const { return sprite; }
    bool getIsActive() const { return isActive; }
    void setIsActive(bool active) { isActive = active; }

    void draw(auto &window) {
        if (isActive) {
            window.draw(sprite);
        }
    }
};
