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
    SpeedBonus(const shared_ptr<Texture> texture) : Bonus(texture) {}

    void applyEffect(Player& player) override {
        cout << player.haveSpeedBonus();
        if (!player.haveSpeedBonus()) {
            speedBonusDuration = 10.0f;
            player.setSpeed(600);
            speedBonusClock.restart();
        }
    }

    void updateBonus(Player& player) override {
        cout << player.haveSpeedBonus();
        if (player.haveSpeedBonus()) {

            float elapsed = speedBonusClock.getElapsedTime().asSeconds();

            if (elapsed >= speedBonusDuration) {
                player.setSpeed(400);
            }
        }
    }

private:
    float speedBonusDuration{0.0f};
    Clock speedBonusClock;
};