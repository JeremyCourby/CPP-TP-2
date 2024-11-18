module;
#include  <SFML/Graphics.hpp>
#include  <iostream>

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

    explicit Player(auto lives):lives{lives} {
        if (!texture.loadFromFile("src/assets/player.png")) {
            cerr << "Erreur lors du chargement de la texture du vaisseau.\n";
        }

        sprite.setTexture(texture);
        sprite.setPosition((1920 - sprite.getGlobalBounds().width) / 2, 900);
        sprite.scale(4.0f, 4.0f);
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
    }

    void loseLife() {
        if (lives > 0) {
            lives--;
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