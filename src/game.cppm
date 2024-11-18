module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <random>

export module Game;
import Player;
import Projectile;
import Enemy;

using namespace sf;
using namespace std;

export class Game {
public:
    RenderWindow window;
    Player player = Player(3);
    vector<Projectile> projectiles;
    vector<Enemy> enemies;

    Clock shootClock;
    float shootInterval = 0.1f;
    shared_ptr<Texture> projectileTexture;

    Clock enemySpawnClock;
    float spawnInterval = 2.0f;
    shared_ptr<Texture> enemyTexture;

    Texture lifeTexture;
    Texture backgroundTexture;
    Sprite backgroundSprite;

    Font font;

    Cursor handCursor;
    Cursor defaultCursor;

    bool isPaused{false};

    Text pauseText{"Pause", font, 300};
    Text resumeButton{"Reprendre", font, 50};
    Text closeButton{"Quitter", font, 50};

    Game() : window(VideoMode(1920, 1080), "Invasion", Style::Fullscreen), projectileTexture{make_shared<Texture>()}, enemyTexture{make_shared<Texture>()} {
        window.setFramerateLimit(144);

        // Chargement des textures

        if (!backgroundTexture.loadFromFile("src/assets/background.png")) {
            cerr << "Erreur lors du chargement de la texture du fond.\n";
        } else {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setPosition(0, 0);
        }

        if (!projectileTexture->loadFromFile("src/assets/projectile.png")) {
            cerr << "Erreur lors du chargement de la texture du projectile.\n";
        }

        if (!lifeTexture.loadFromFile("src/assets/life_2.png")) {
            cerr << "Erreur lors du chargement de la texture du coeur.\n";
        }

        if (!enemyTexture->loadFromFile("src/assets/enemy.png")) {
            cerr << "Erreur lors du chargement de la texture de l'ennemi.\n";
        }

        if (!font.loadFromFile("src/assets/Font/txt.ttf")) {
            cerr << "Erreur lors du chargement de la police.\n";
            return;
        }

        if (!handCursor.loadFromSystem(sf::Cursor::Hand)) {
            cerr << "Erreur lors du chargement du curseur de la main.\n";
        }

        if (!defaultCursor.loadFromSystem(sf::Cursor::Arrow)) {
            cerr << "Erreur lors du chargement du curseur par défaut.\n";
        }

        pauseText.setFillColor(Color::White);
        pauseText.setPosition(600, 100);

        resumeButton.setFillColor(Color::White);
        resumeButton.setPosition(860, 500);

        closeButton.setFillColor(Color::White);
        closeButton.setPosition(860, 600);

    }

    void run() {
        showMenu();

        Clock clock;
        while (window.isOpen() && player.isAlive()) {
            float deltaTime = clock.restart().asSeconds();
            processEvents();

            if (!isPaused) {
                update(deltaTime);
            }

            render();
        }

        showGameOver();
    }

    void showMenu() {

        Text textNameGame("Invasion", font, 300);
        textNameGame.setFillColor(Color::White);
        textNameGame.setPosition(500, 50);

        Text playButton("Jouer", font, 50);
        playButton.setFillColor(Color::White);
        playButton.setPosition(860, 500);  // Centrer le texte dans la fenêtre

        Text closeButton("Quitter", font, 50);
        closeButton.setFillColor(Color::White);
        closeButton.setPosition(860, 600);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (playButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        return;
                    }

                    if (closeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        window.close();
                        return;
                    }
                }

                auto mousePos{Mouse::getPosition(window)};

                if (playButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    window.setMouseCursor(handCursor);
                    playButton.setCharacterSize(60);
                } else if (closeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    window.setMouseCursor(handCursor);
                    closeButton.setCharacterSize(60);
                } else {
                    window.setMouseCursor(defaultCursor);
                    playButton.setCharacterSize(50);
                    closeButton.setCharacterSize(50);
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            window.draw(playButton);
            window.draw(closeButton);
            window.draw(textNameGame);
            window.display();
        }
    }

    void showPause() {
        window.draw(pauseText);
        window.draw(resumeButton);
        window.draw(closeButton);
    }

    void showGameOver() {

        Text replayButton("Rejouer", font, 50);
        replayButton.setFillColor(Color::White);
        replayButton.setPosition(860, 500);

        Text closeButton("Quitter", font, 50);
        closeButton.setFillColor(Color::White);
        closeButton.setPosition(860, 600);

        Text textGameOver("Game Over", font, 300);
        textGameOver.setFillColor(Color::White);
        textGameOver.setPosition(400, 50);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (replayButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        return;
                    }

                    if (closeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        window.close();
                        return;
                    }
                }

                auto mousePos{Mouse::getPosition(window)};

                if (replayButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    window.setMouseCursor(handCursor);
                    replayButton.setCharacterSize(60);
                } else if (closeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    window.setMouseCursor(handCursor);
                    closeButton.setCharacterSize(60);
                } else {
                    window.setMouseCursor(defaultCursor);
                    replayButton.setCharacterSize(50);
                    closeButton.setCharacterSize(50);
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            window.draw(replayButton);
            window.draw(closeButton);
            window.draw(textGameOver);
            window.display();
        }
    }

private:
    void processEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (!isPaused) {
                if (Keyboard::isKeyPressed(Keyboard::Space) && shootClock.getElapsedTime().asSeconds() >= shootInterval) {
                    projectiles.emplace_back(projectileTexture, player.sprite.getPosition().x + player.sprite.getGlobalBounds().width / 2, player.sprite.getPosition().y);
                    shootClock.restart();
                }
            }

            if (event.type == Event::KeyReleased && event.key.code == Keyboard::Escape) {
                isPaused = !isPaused;
            }

            if (isPaused) {
                Vector2i mousePos = Mouse::getPosition(window);

                FloatRect closeBounds(860, 600, 200, 50);
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (resumeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    {
                        resumeButton.setCharacterSize(60);
                        isPaused = false;
                        return;
                    }

                    if (closeBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        window.close();
                        return;
                    }
                }
            }
        }
    }

    void spawnEnemies(int numberOfEnemies) {

        random_device rd_spawn;
        mt19937 gen_spawn(rd_spawn());
        uniform_int_distribution<> dis_spawn(0, window.getSize().x - 100);

        random_device rd_scale;
        mt19937 gen_scale(rd_scale());
        uniform_int_distribution<> dis_scale(2.0f, 4.0f);

        random_device rd_speed;
        mt19937 gen_speed(rd_speed());
        uniform_int_distribution<> dis_speed(50.0f, 100.0f);

        for (int i = 0; i < numberOfEnemies; ++i) {
            auto startX = dis_spawn(gen_spawn);
            auto startY = -100;
            enemies.emplace_back(enemyTexture, startX, startY,dis_scale(gen_scale),dis_speed(gen_speed));
        }
    }

    void update(float deltaTime) {
        player.update(deltaTime);

        for (auto& proj : projectiles) {
            proj.update(deltaTime);
        }

        random_device rdNumberSpawn;
        mt19937 genNumberSpawn(rdNumberSpawn());
        uniform_int_distribution<> disNumberSpawn(1, 3);

        if (enemySpawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            spawnEnemies(disNumberSpawn(genNumberSpawn));
            enemySpawnClock.restart();
        }

        projectiles.erase(
            remove_if(projectiles.begin(), projectiles.end(),
                      [](const Projectile& proj) {
                          return proj.sprite.getPosition().y < 0;
                      }),
            projectiles.end()
        );

        for (auto it = enemies.begin(); it != enemies.end(); ) {
            it->update(deltaTime);

            // Vérification si un ennemi est sorti par le bas ou par les côtés de l'écran
            if (it->sprite.getPosition().y > window.getSize().y ||
                it->sprite.getPosition().x < 0 ||
                it->sprite.getPosition().x > window.getSize().x) {

                player.loseLife();

                it = enemies.erase(it);

                if (!player.isAlive()) {
                    std::cout << "Game Over!\n";
                    //  window.close();
                    // return;
                }
            } else {
                ++it;
            }
        }

        for (auto it = projectiles.begin(); it != projectiles.end(); ) {
            bool hit = false;
            for (auto et = enemies.begin(); et != enemies.end(); ) {
                if (it->sprite.getGlobalBounds().intersects(et->sprite.getGlobalBounds())) {
                    it = projectiles.erase(it);
                    et = enemies.erase(et);
                    hit = true;
                    break;
                } else {
                    ++et;
                }
            }
            if (!hit) {
                ++it;
            }
        }
    }

    void render() {
        window.clear();

        window.draw(backgroundSprite);
        window.draw(player.sprite);

        for (const auto& proj : projectiles) {
            window.draw(proj.sprite);
        }

        for (const auto& enemy : enemies) {
            window.draw(enemy.sprite);
        }

        for (int i = 0; i < player.lives; ++i) {
            Sprite lifeSprite;
            lifeSprite.setTexture(lifeTexture);
            lifeSprite.setPosition(10 + i * (lifeTexture.getSize().x + 50), 10);
            lifeSprite.scale(4.0f, 4.0f);
            window.draw(lifeSprite);
        }

        // Afficher l'écran de pause si le jeu est en pause
        if (isPaused) {
            showPause(); // Nouvelle fonction pour afficher l'overlay de pause
        }

        window.display();
    }
};
