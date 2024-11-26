module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

export module Player;

export class Player
{
public:
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

    auto getSprite() { return sprite; }
    auto setSprite(auto const sprite) { this->sprite = sprite; }

    auto getTexture() { return texture; }
    auto setTexture(auto const texture) { this->texture = texture; }

    auto getSpeed() { return speed; }
    auto setSpeed(auto const speed) { this->speed = speed; }

    auto getLives() { return lives; }
    auto setLives(auto const lives) { this->lives = lives; }

    auto getScore() { return score; }
    auto setScore(auto const score) { this->score = score; }

    auto getShootInterval() { return shootInterval; }
    auto setShootInterval(auto const shootInterval) { this->shootInterval = shootInterval; }

    auto getSpeedBoostTimer() { return speedBoostTimer; }

    auto getBulletBoostTimer() { return bulletBoostTimer; }

    void update(float const deltaTime) {
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

        if (speedBoostTimer > 0.0f) {
            speedBoostTimer -= deltaTime;
            if (speedBoostTimer <= 0.0f) {
                setSpeed(400);
            }
        }

        if (bulletBoostTimer > 0.0f) {
            bulletBoostTimer -= deltaTime;
            if (bulletBoostTimer <= 0.0f) {
                setShootInterval(0.5f);
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

    auto addScore(auto const score) {
        this->score += score;
        return this->score;
    }

    auto addLives(auto const lives) {
        if (this->lives < 5)
        {
            this->lives += lives;
        }
        return this->lives;
    }

    auto isAlive() const {
        return lives > 0;
    }

    auto haveSpeedBonus() {
        if(speed > 400) {
            return true;
        } else {
            return false;
        }
    }

    auto haveBulletBonus() {
        if(shootInterval > 0.5f) {
            return true;
        } else {
            return false;
        }
    }

    void activateSpeedBoost(float boostDuration, float speedAmount) {
        speedBoostTimer = boostDuration;
        speed = speedAmount;
    }

    void activateBulletBoost(float boostDuration, float shootInterval) {
        bulletBoostTimer = boostDuration;
        this->shootInterval = shootInterval;
    }

private:
    Sprite sprite;
    Texture texture;
    float speed{400.0f};
    int lives{3};
    int score{0};
    float speedBoostTimer{0};
    float bulletBoostTimer{0};
    float shootInterval{0.5f};

    bool isBlinking = false;
    Clock blinkClock;
    float blinkDuration{1.0f};
    float blinkInterval{0.1f};
    SoundBuffer lifeLostBuffer;
    Sound lifeLostSound;
};
