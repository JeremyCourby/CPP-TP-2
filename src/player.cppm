// //
// // Created by jerem on 14/11/2024.
// //
// #include <SFML/Graphics.hpp>
// #include <iostream>
//
// module;
//
// export module player;
//
// export namespace player {
//
//     class Player {
//     public:
//         sf::Sprite sprite;
//         sf::Texture texture;
//         float speed = 300.0f;
//
//         Player() {
//             if (!texture.loadFromFile("assets/player.png")) {
//                 std::cerr << "Erreur lors du chargement de la texture du vaisseau.\n";
//             }
//             sprite.setTexture(texture);
//             sprite.setPosition(400.0f, 500.0f);
//         }
//
//         void update(float deltaTime) {
//             if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sprite.getPosition().x > 0)
//                 sprite.move(-speed * deltaTime, 0);
//             if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sprite.getPosition().x < 800 - sprite.getGlobalBounds().width)
//                 sprite.move(speed * deltaTime, 0);
//         }
//     };
//
// }

export module Player;

import <SFML/Graphics.hpp>;
import <iostream>;

export class Player {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed = 300.0f;

    Player() {
        if (!texture.loadFromFile("assets/player.png")) {
            std::cerr << "Erreur lors du chargement de la texture du vaisseau.\n";
        }
        sprite.setTexture(texture);
        sprite.setPosition(400.0f, 500.0f);
    }

    void update(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sprite.getPosition().x > 0)
            sprite.move(-speed * deltaTime, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sprite.getPosition().x < 800 - sprite.getGlobalBounds().width)
            sprite.move(speed * deltaTime, 0);
    }
};