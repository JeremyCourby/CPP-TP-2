module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <SFML/Audio.hpp>

export module Game;
import Player;
import Projectile;
import Enemy;
import Explosion;
import Bonus;
import LifeBonus;
import SpeedBonus;
import BulletBonus;

using namespace sf;
using namespace std;

export class Game {
public:

    Game() : window(VideoMode(1920, 1080), "Invasion", Style::Fullscreen), projectileTexture{make_shared<Texture>()}, enemyTexture{make_shared<Texture>()}, bonusLifeTexture{make_unique<Texture>()}, bonusSpeedTexture{make_unique<Texture>()}, bonusBulletTexture{make_unique<Texture>()} {
        window.setFramerateLimit(144);

        loadTexture();

        shootSound.setBuffer(shootBuffer);
        shootSound.setVolume(50);

        explosionSound.setBuffer(explosionBuffer);
        explosionSound.setVolume(50);

        gameOverSound.setBuffer(gameOverBuffer);
        gameOverSound.setVolume(50);

        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();

        pauseText.setFillColor(Color::White);
        pauseText.setPosition(700, 100);

        resumeButton.setFillColor(Color::White);
        resumeButton.setPosition(860, 500);

        closeButton.setFillColor(Color::White);
        closeButton.setPosition(860, 600);

        scoreText.setFillColor(Color::White);
        scoreText.setPosition(20, 50);
    }

    void run() {

        showMenu();

        Clock clock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            processEvents();

            if (!isPaused && player->isAlive()) {
                update(deltaTime);
            }

            if (!player->isAlive()) {
                showGameOver();
                resetGame();
                continue;
            }

            render();
        }
    }

private:

    RenderWindow window;
    unique_ptr<Player> player = make_unique<Player>(3);
    vector<Projectile> projectiles;
    vector<Enemy> enemies;

    Clock shootClock;
    float shootInterval = 0.4f;
    shared_ptr<Texture> projectileTexture;

    Clock enemySpawnClock;
    float spawnInterval = 2.0f;
    shared_ptr<Texture> enemyTexture;

    Texture lifeTexture;

    shared_ptr<Texture> bonusLifeTexture;
    shared_ptr<Texture> bonusSpeedTexture;
    shared_ptr<Texture> bonusBulletTexture;

    Texture backgroundTexture;
    Sprite backgroundSprite;

    Font font;

    Cursor handCursor;
    Cursor defaultCursor;

    bool isPaused{false};

    Text pauseText{"Pause", font, 300};
    Text resumeButton{"Reprendre", font, 50};
    Text closeButton{"Quitter", font, 50};

    Text scoreText{"Score : ", font, 50};

    SoundBuffer shootBuffer;
    Sound shootSound;

    SoundBuffer explosionBuffer;
    Sound explosionSound;

    SoundBuffer gameOverBuffer;
    Sound gameOverSound;

    Music backgroundMusic;

    Texture explosionTexture;
    Explosion explosion{explosionTexture, 64, 64, 8, 0.1f};

    vector<Explosion> explosions;

    //vector<Bonus> bonuses;
    vector<unique_ptr<Bonus>> bonuses;
    Clock bonusClock;

    void loadTexture()
    {
        if (!backgroundTexture.loadFromFile("src/assets/background.png")) {
            cerr << "Erreur lors du chargement de la texture du fond.\n";
        } else {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setPosition(0, 0);
        }

        if (!bonusLifeTexture->loadFromFile("src/assets/life_2.png")) {
            cerr << "Erreur lors du chargement de la texture du bonus life.\n";
        }

        if (!bonusSpeedTexture->loadFromFile("src/assets/speed.png")) {
            cerr << "Erreur lors du chargement de la texture du bonus de speed.\n";
        }

        if (!bonusBulletTexture->loadFromFile("src/assets/bullet.png")) {
            cerr << "Erreur lors du chargement de la texture du bonus bullet.\n";
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

        if (!shootBuffer.loadFromFile("src/assets/Sound/laser.wav")) {
            std::cerr << "Erreur lors du chargement du son de tir.\n";
        }

        if (!explosionBuffer.loadFromFile("src/assets/Sound/explosion.wav")) {
            std::cerr << "Erreur lors du chargement du son d'explosion'.\n";
        }

        if (!gameOverBuffer.loadFromFile("src/assets/Sound/gameOver.wav")) {
            std::cerr << "Erreur lors du chargement du son d'explosion'.\n";
        }

        if (!backgroundMusic.openFromFile("src/assets/Sound/fond.wav")) {
            std::cerr << "Erreur lors du chargement de la musique de fond.\n";
        }

        if (!explosionTexture.loadFromFile("src/assets/explosion.png")) {
            std::cerr << "Erreur lors du chargement de la texture d'explosion.\n";
        }
    }

    void resetGame() {

        player = std::make_unique<Player>(3);

        projectiles.clear();
        enemies.clear();
        bonuses.clear();

        shootClock.restart();
        enemySpawnClock.restart();
        bonusClock.restart();

        // Remettre le jeu en mode "non-pausé"
        isPaused = false;
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
                    playButton.setFillColor(Color::Yellow);
                    playButton.setStyle(Text::Bold);
                } else if (closeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    window.setMouseCursor(handCursor);
                    closeButton.setFillColor(Color::Yellow);
                    closeButton.setStyle(Text::Bold);
                } else {
                    window.setMouseCursor(defaultCursor);
                    playButton.setFillColor(Color::White);
                    closeButton.setFillColor(Color::White);
                    playButton.setStyle(Text::Regular);
                    closeButton.setStyle(Text::Regular);
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
        auto mousePos{Mouse::getPosition(window)};

        if (resumeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            window.setMouseCursor(handCursor);
            resumeButton.setFillColor(Color::Yellow);
        } else if (closeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            window.setMouseCursor(handCursor);
            closeButton.setFillColor(Color::Yellow);
        } else {
            window.setMouseCursor(defaultCursor);
            resumeButton.setFillColor(Color::White);
            closeButton.setFillColor(Color::White);
        }

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

        Text textScore(format("Score : {}", player->getScore()), font, 100);
        textScore.setFillColor(Color::White);
        textScore.setPosition(750, 350);

        gameOverSound.play();

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (replayButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        resetGame();
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
                    replayButton.setFillColor(Color::Yellow);
                } else if (closeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    window.setMouseCursor(handCursor);
                    closeButton.setFillColor(Color::Yellow);
                } else {
                    window.setMouseCursor(defaultCursor);
                    replayButton.setFillColor(Color::White);
                    closeButton.setFillColor(Color::White);
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            window.draw(replayButton);
            window.draw(closeButton);
            window.draw(textGameOver);
            window.draw(textScore);
            window.display();
        }
    }

    void spawnEnemies(int numberOfEnemies) {
        for (int i = 0; i < numberOfEnemies; ++i) {
            enemies.emplace_back(enemyTexture, rand() % (window.getSize().x - 100), -100,rand() % 4 + 2,rand() % 100 + 50);
        }
    }

    void processEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::KeyReleased && event.key.code == Keyboard::Escape) {
                isPaused = !isPaused;
            }

            if (isPaused) {
                const auto mousePos = Mouse::getPosition(window);

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

    void update(float deltaTime) {
        player->update(deltaTime);
        scoreText.setString(format("Score : {}", player->getScore()));

        explosion.update(deltaTime);

        for (auto itBonus = bonuses.begin(); itBonus != bonuses.end();) {
            (*itBonus)->update(deltaTime);

            if ((*itBonus)->getIsActive() && (*itBonus)->getSprite().getGlobalBounds().intersects(player->getSprite().getGlobalBounds())) {
                (*itBonus)->applyEffect(*player);

                (*itBonus)->setIsActive(false);
                itBonus = bonuses.erase(itBonus);
            } else {
                ++itBonus;
            }
        }



        if (!isPaused) {
            if (Keyboard::isKeyPressed(Keyboard::Space) && shootClock.getElapsedTime().asSeconds() >= shootInterval) {
                projectiles.emplace_back(projectileTexture, player->getSprite().getPosition().x + player->getSprite().getGlobalBounds().width / 2, player->getSprite().getPosition().y);
                shootSound.play();
                shootClock.restart();
            }
        }

        for (auto& proj : projectiles) {
            proj.update(deltaTime);
        }

        if (enemySpawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            spawnEnemies(rand() % 3 + 1);
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

            if (it->sprite.getPosition().y > window.getSize().y ||
                it->sprite.getPosition().x < 0 ||
                it->sprite.getPosition().x > window.getSize().x) {

                player->loseLife();

                it = enemies.erase(it);

            } else {
                ++it;
            }
        }



        for (auto it = projectiles.begin(); it != projectiles.end(); ) {
            bool hit = false;
            for (auto et = enemies.begin(); et != enemies.end(); ) {
                if (it->sprite.getGlobalBounds().intersects(et->sprite.getGlobalBounds())) {
                    explosionSound.play();
                    explosions.emplace_back(explosionTexture, 64, 64, 8, 0.1f);
                    explosions.back().sprite.setPosition(et->sprite.getPosition());
                    explosions.back().sprite.setScale(et->sprite.getScale());

                    if (rand() % 100 < 100) {
                        std::unique_ptr<Bonus> newBonus;

                        int bonusType = rand() % 3; // 0 pour LifeBonus, 1 pour SpeedBonus, 2 pour BulletBonus
                        if (bonusType == 0 && player->getLives() < 5) {
                            // Bonus de vie
                            newBonus = std::make_unique<LifeBonus>(bonusLifeTexture);
                        } else if (bonusType == 1 && !player->haveSpeedBonus()) {
                            // Bonus de vitesse
                            newBonus = std::make_unique<SpeedBonus>(bonusSpeedTexture);
                        } else if (bonusType == 2 && !player->haveSpeedBonus()) {
                            // Bonus de vitesse de tir
                            newBonus = std::make_unique<BulletBonus>(bonusBulletTexture);
                        }

                        if (newBonus) {
                            newBonus->spawn(et->sprite.getPosition(), 3.0f); // Apparition avec une durée limitée
                            bonuses.push_back(std::move(newBonus));
                        }
                    }

                    it = projectiles.erase(it);
                    et = enemies.erase(et);
                    hit = true;
                    player->addScore(100);
                    break;
                } else {
                    ++et;
                }
            }
            if (!hit) {
                ++it;
            }
        }

        for (auto it = explosions.begin(); it != explosions.end(); ) {
            it->update(deltaTime);
            if (it->isFinished) {
                it = explosions.erase(it);
            } else {
                ++it;
            }
        }

        for (auto& bonus : bonuses) {
            bonus->updateBonus(*player);
        }
    }

    void render() {
        window.setMouseCursor(defaultCursor);

        window.clear();

        window.draw(backgroundSprite);
        window.draw(player->getSprite());

        for (auto &bonus : bonuses) { bonus->draw(window); }

        for (const auto& proj : projectiles) { window.draw(proj.sprite); }
        for (const auto& enemy : enemies) { window.draw(enemy.sprite); }

        for (int i = 0; i < player->getLives(); ++i) {
            Sprite lifeSprite;
            lifeSprite.setTexture(lifeTexture);
            lifeSprite.setPosition(10 + i * (lifeTexture.getSize().x + 50), 10);
            lifeSprite.scale(4.0f, 4.0f);
            window.draw(lifeSprite);
        }

        for (const auto& explosion : explosions) { window.draw(explosion.sprite); }

        window.draw(scoreText);

        if (isPaused) { showPause(); }

        window.display();
    }
};
