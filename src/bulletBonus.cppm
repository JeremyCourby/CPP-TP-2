module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

import Bonus;
import Player;

export module BulletBonus;

export class BulletBonus : public Bonus {

public:
    BulletBonus(const shared_ptr<Texture> texture) : Bonus(texture)
    {
        sprite.setScale(0.30f,0.30f);
    }

    void applyEffect(Player& player) override {
        player.activateBulletBoost(10, 0.25f);
    }
};