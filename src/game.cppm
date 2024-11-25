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

using namespace sf;
using namespace std;

export class Game {
public:

    Game() : window(VideoMode(1920, 1080), "Invasion", Style::Fullscreen), projectileTexture{make_shared<Texture>()}, enemyTexture{make_shared<Texture>()}, bonusLifeTexture{make_shared<Texture>()} {
        window.setFramerateLimit(144);

        // Chargement des textures

        if (!backgroundTexture.loadFromFile("src/assets/background.png")) {
            cerr << "Erreur lors du chargement de la texture du fond.\n";
        } else {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setPosition(0, 0);
        }

        if (!bonusLifeTexture->loadFromFile("src/assets/life_2.png")) {
            cerr << "Erreur lors du chargement de la texture du bonus life.\n";
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
        shootSound.setBuffer(shootBuffer);
        shootSound.setVolume(50); // Réglage du volume

        if (!explosionBuffer.loadFromFile("src/assets/Sound/explosion.wav")) {
            std::cerr << "Erreur lors du chargement du son d'explosion'.\n";
        }
        explosionSound.setBuffer(explosionBuffer);
        explosionSound.setVolume(50);

        if (!gameOverBuffer.loadFromFile("src/assets/Sound/gameOver.wav")) {
            std::cerr << "Erreur lors du chargement du son d'explosion'.\n";
        }
        gameOverSound.setBuffer(gameOverBuffer);
        gameOverSound.setVolume(50);

        if (!backgroundMusic.openFromFile("src/assets/Sound/fond.wav")) {
            std::cerr << "Erreur lors du chargement de la musique de fond.\n";
        }

        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);

        backgroundMusic.play();

        if (!explosionTexture.loadFromFile("src/assets/explosion.png")) {
            std::cerr << "Erreur lors du chargement de la texture d'explosion.\n";
        }

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

    vector<Bonus> bonuses;
    Clock bonusClock;

    void resetGame() {

        player = std::make_unique<Player>(3);

        projectiles.clear();
        enemies.clear();

        shootClock.restart();
        enemySpawnClock.restart();

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

    void update(float deltaTime) {
        player->update(deltaTime);
        scoreText.setString(format("Score : {}", player->getScore()));

        explosion.update(deltaTime);

        for (auto it = bonuses.begin(); it != bonuses.end();) {
            it->update(deltaTime);
            if (it->isActive && it->sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds())) {
                it->isActive = false;
                player->lives++;
                it = bonuses.erase(it);
            } else {
                ++it;
            }
        }

        if (!isPaused) {
            if (Keyboard::isKeyPressed(Keyboard::Space) && shootClock.getElapsedTime().asSeconds() >= shootInterval) {
                projectiles.emplace_back(projectileTexture, player->sprite.getPosition().x + player->sprite.getGlobalBounds().width / 2, player->sprite.getPosition().y);
                shootSound.play();
                shootClock.restart();
            }
        }

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

                    Bonus newBonus{};

                    random_device rdNumberBonus;
                    mt19937 genNumberBonus(rdNumberBonus());
                    uniform_int_distribution<> disNumberBonus(1, 3);

                    if (std::rand() % 100 < 10 ) {
                        switch (disNumberBonus(genNumberBonus))
                        {
                            case 1: // Bonus Life
                                if (player->getLives() <= 5)
                                {
                                    newBonus.spawn(et->sprite.getPosition(), bonusLifeTexture, 3.0f);
                                }

                            case 2:  // Bonus Vitesse
                                newBonus.spawn(et->sprite.getPosition(), bonusSpeedTexture, 3.0f);

                        }
                        bonuses.push_back(newBonus);

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
                it = explosions.erase(it); // Supprimer l'explosion terminée
            } else {
                ++it;
            }
        }
    }

    void render() {
        window.setMouseCursor(defaultCursor);

        window.clear();

        window.draw(backgroundSprite);
        window.draw(player->sprite);

        for (auto &bonus : bonuses) { bonus.draw(window); }

        for (const auto& proj : projectiles) { window.draw(proj.sprite); }
        for (const auto& enemy : enemies) { window.draw(enemy.sprite); }

        for (int i = 0; i < player->lives; ++i) {
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
