module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

export module Player;

export class Player {
public:
    Sprite sprite;
    Texture texture;
    float speed{400.0f};
    int lives{3};
    int score{0};

    // Variables pour la gestion du clignotement
    bool isBlinking = false;
    Clock blinkClock;
    float blinkDuration = 1.0f; // Durée totale du clignotement en secondes
    float blinkInterval = 0.1f; // Intervalle de clignotement (on/off)

    SoundBuffer lifeLostBuffer;
    Sound lifeLostSound;

    explicit Player(auto lives):lives{lives} {
        if (!texture.loadFromFile("src/assets/player.png")) {
            cerr << "Erreur lors du chargement de la texture du vaisseau.\n";
        }

        sprite.setTexture(texture);
        sprite.setPosition((1920 - sprite.getGlobalBounds().width) / 2, 900);
        sprite.scale(4.0f, 4.0f);

        if (!lifeLostBuffer.loadFromFile("src/assets/Sound/lifeLost.wav")) {
            std::cerr << "Erreur lors du chargement du son d'explosion'.\n";
        }
        lifeLostSound.setBuffer(lifeLostBuffer);
        lifeLostSound.setVolume(50);
    }

    void update(float deltaTime) {
        if (Keyboard::isKeyPressed(Keyboard::Left) && sprite.getPosition().x > 0)
            sprite.move(-speed * deltaTime, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right) && sprite.getPosition().x < 1920 - sprite.getGlobalBounds().width)
            sprite.move(speed * deltaTime, 0);
        if (Keyboard::isKeyPressed(Keyboard::Q) && sprite.getPosition().x > 0)
            sprite.move(-speed * deltaTime, 0);
        if (Keyboard::isKeyPressed(Keyboard::D) && sprite.getPosition().x < 1920 - sprite.getGlobalBounds().width)
            sprite.move(speed * deltaTime, 0);

        if (isBlinking) {
            float elapsedTime = blinkClock.getElapsedTime().asSeconds();
            if (elapsedTime < blinkDuration) {
                if (static_cast<int>(elapsedTime / blinkInterval) % 2 == 0) {
                    sprite.setColor(Color(255, 0, 0, 50));
                } else {
                    sprite.setColor(Color(255, 0, 0, 255));
                }
            } else {
                isBlinking = false;
                sprite.setColor(Color(255, 255, 255, 255));
            }
        }
    }

    void loseLife() {
        if (lives > 0) {
            lives--;
            lifeLostSound.play();
            isBlinking = true;
            blinkClock.restart();
        }
    }

    int addScore(auto score) {
        this->score += score;
        return this->score;
    }

    int getScore() { return score; }
    int setScore(auto score) { this->score = score; }

    bool isAlive() const {
        return lives > 0;
    }
};
