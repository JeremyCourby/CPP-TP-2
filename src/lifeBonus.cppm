module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

import Bonus;
import Player;

export module LifeBonus;

export class LifeBonus : public Bonus {
public:
    LifeBonus(const  shared_ptr<Texture> texture) : Bonus(texture) {}

    void applyEffect(Player& player) override {
        player.addLives(1);
    }
};