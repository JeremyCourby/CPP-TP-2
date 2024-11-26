module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

import Bonus;
import Player;

export module SpeedBonus;

export class SpeedBonus : public Bonus {

public:
    SpeedBonus(const shared_ptr<Texture> texture) : Bonus(texture)
    {
        sprite.setScale(1.5f,1.5f);
    }

    void applyEffect(Player& player) override {
        player.activateSpeedBoost(10, 600);
    }
};