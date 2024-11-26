module;
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
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
        shootSound.setVolume(30);

        explosionSound.setBuffer(explosionBuffer);
        explosionSound.setVolume(50);

        gameOverSound.setBuffer(gameOverBuffer);
        gameOverSound.setVolume(50);

        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();

        initText(pauseText,"Pause",font,300,Vector2f{0,100},true);
        initText(resumeButton,"Reprendre",font,50,Vector2f{0,500},true);
        initText(closeButton,"Quitter",font,50,Vector2f{0,600},true);
        initText(scoreText,"Score : ",font,50,Vector2f{20,50},false);
        initText(speedBonusText,"",font,50,Vector2f{1870,10},false);
        initText(bulletBonusText,"",font,50,Vector2f{1870,60},false);

        speedSprite.setTexture(speedTexture);
        speedSprite.setPosition(1818, 25);
        speedSprite.scale(1.5f, 1.5f);

        bulletSprite.setTexture(bulletTexture);
        bulletSprite.setPosition(1818,93);
        bulletSprite.scale(0.25f, 0.25f);

    }

    void run() {
        showMenu();
        Clock clock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            processEvents();
            if (!isPaused && player->isAlive()) { update(deltaTime); }
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
    Clock shootClock, enemySpawnClock;
    shared_ptr<Texture> projectileTexture, enemyTexture, bonusLifeTexture, bonusSpeedTexture, bonusBulletTexture;
    Texture lifeTexture, speedTexture, bulletTexture, backgroundTexture, explosionTexture;
    Sprite speedSprite, bulletSprite, backgroundSprite;
    Font font;
    Cursor handCursor, defaultCursor;
    Text pauseText, resumeButton, closeButton,scoreText, speedBonusText, bulletBonusText, textNameGame, playButton,replayButton,textGameOver,textScore;
    SoundBuffer shootBuffer, explosionBuffer, gameOverBuffer;
    Sound shootSound, explosionSound, gameOverSound;
    Music backgroundMusic;
    Explosion explosion{explosionTexture, 64, 64, 8, 0.1f};
    vector<Explosion> explosions;
    vector<unique_ptr<Bonus>> bonuses;
    float spawnInterval = 2.0f;
    bool isPaused{false};

    void loadTexture()
    {
        if (!backgroundTexture.loadFromFile("src/assets/background.png")) {
            cerr << "Erreur lors du chargement de la texture du fond.\n";
        } else {
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setPosition(0, 0);
        }

        if (!bonusLifeTexture->loadFromFile("src/assets/life_2.png")) { cerr << "Erreur lors du chargement de la texture du bonus life.\n"; }
        if (!bonusSpeedTexture->loadFromFile("src/assets/speed.png")) { cerr << "Erreur lors du chargement de la texture du bonus de speed.\n"; }
        if (!bonusBulletTexture->loadFromFile("src/assets/bullet.png")) { cerr << "Erreur lors du chargement de la texture du bonus bullet.\n"; }
        if (!projectileTexture->loadFromFile("src/assets/projectile.png")) { cerr << "Erreur lors du chargement de la texture du projectile.\n"; }
        if (!lifeTexture.loadFromFile("src/assets/life_2.png")) { cerr << "Erreur lors du chargement de la texture du coeur.\n"; }
        if (!speedTexture.loadFromFile("src/assets/speed.png")) { cerr << "Erreur lors du chargement de la texture du coeur.\n"; }
        if (!bulletTexture.loadFromFile("src/assets/bullet.png")) { cerr << "Erreur lors du chargement de la texture du coeur.\n"; }
        if (!enemyTexture->loadFromFile("src/assets/enemy.png")) { cerr << "Erreur lors du chargement de la texture de l'ennemi.\n"; }
        if (!font.loadFromFile("src/assets/Font/txt.ttf")) { cerr << "Erreur lors du chargement de la police.\n"; }
        if (!handCursor.loadFromSystem(sf::Cursor::Hand)) { cerr << "Erreur lors du chargement du curseur de la main.\n"; }
        if (!defaultCursor.loadFromSystem(sf::Cursor::Arrow)) { cerr << "Erreur lors du chargement du curseur par défaut.\n"; }
        if (!shootBuffer.loadFromFile("src/assets/Sound/laser.wav")) { cerr << "Erreur lors du chargement du son de tir.\n"; }
        if (!explosionBuffer.loadFromFile("src/assets/Sound/explosion.wav")) { cerr << "Erreur lors du chargement du son d'explosion'.\n"; }
        if (!gameOverBuffer.loadFromFile("src/assets/Sound/gameOver.wav")) { cerr << "Erreur lors du chargement du son d'explosion'.\n"; }
        if (!backgroundMusic.openFromFile("src/assets/Sound/fond.wav")) { cerr << "Erreur lors du chargement de la musique de fond.\n"; }
        if (!explosionTexture.loadFromFile("src/assets/explosion.png")) { cerr << "Erreur lors du chargement de la texture d'explosion.\n"; }
    }

    void resetGame() {
        player = std::make_unique<Player>(3);
        projectiles.clear();
        enemies.clear();
        bonuses.clear();
        shootClock.restart();
        enemySpawnClock.restart();
        isPaused = false;
    }

    static void initText(auto &text, auto textString, auto &font, auto textSize, auto vector, auto isCenter){
        text.setString(textString);
        text.setFont(font);
        text.setCharacterSize(textSize);
        text.setFillColor(Color::White);
        if (!isCenter){
            text.setPosition(vector);
        } else{
            text.setPosition((1920 - text.getGlobalBounds().getSize().x)/2,vector.y);
        }

    }

    void showMenu() {

        initText(textNameGame,"Invasion",font,300,Vector2f{0,50}, true);
        initText(playButton,"Jouer",font,50,Vector2f{0,500},true);
        initText(closeButton,"Quitter",font,50,Vector2f{0,600},true);

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

        initText(textGameOver,"Game Over",font,300,Vector2f{0,50},true);
        initText(textScore,format("Score : {}", player->getScore()),font,100,Vector2f{0,350},true);
        initText(replayButton,"Rejouer",font,50,Vector2f{0,500},true);
        initText(closeButton,"Quitter",font,50,Vector2f{0,600},true);

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

            if (event.type == Event::Closed) { window.close(); }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::Escape) { isPaused = !isPaused; }
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
        explosion.update(deltaTime);

        scoreText.setString(format("Score : {}", player->getScore()));
        speedBonusText.setString(format("{}", static_cast<Int32>(player->getSpeedBoostTimer())));
        bulletBonusText.setString(format("{}", static_cast<Int32>(player->getBulletBoostTimer())));

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
            if (Keyboard::isKeyPressed(Keyboard::Space) && shootClock.getElapsedTime().asSeconds() >= player->getShootInterval()) {
                projectiles.emplace_back(projectileTexture, player->getSprite().getPosition().x + player->getSprite().getGlobalBounds().width / 2, player->getSprite().getPosition().y);
                shootSound.play();
                shootClock.restart();
            }
        }

        for (auto& proj : projectiles) { proj.update(deltaTime); }

        if (enemySpawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            spawnEnemies(rand() % 3 + 1);
            enemySpawnClock.restart();
        }

        projectiles.erase(
            remove_if(projectiles.begin(), projectiles.end(),[](const Projectile& proj) {
                return proj.sprite.getPosition().y < 0;
            }),
            projectiles.end()
        );

        for (auto itEnemy = enemies.begin(); itEnemy != enemies.end(); ) {
            itEnemy->update(deltaTime);

            if (itEnemy->sprite.getPosition().y > window.getSize().y || itEnemy->sprite.getPosition().x < 0 || itEnemy->sprite.getPosition().x > window.getSize().x) {
                player->loseLife();
                itEnemy = enemies.erase(itEnemy);
            } else {
                ++itEnemy;
            }
        }

        for (auto itProj = projectiles.begin(); itProj != projectiles.end(); ) {
            bool hit = false;
            for (auto et = enemies.begin(); et != enemies.end(); ) {
                if (itProj->sprite.getGlobalBounds().intersects(et->sprite.getGlobalBounds())) {
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
                        } else if (bonusType == 1 && player->getSpeedBoostTimer() <= 0) {
                            // Bonus de vitesse
                            newBonus = std::make_unique<SpeedBonus>(bonusSpeedTexture);
                        } else if (bonusType == 2 && player->getBulletBoostTimer() <= 0) {
                            // Bonus de vitesse de tir
                            newBonus = std::make_unique<BulletBonus>(bonusBulletTexture);
                        }

                        if (newBonus) {
                            newBonus->spawn(et->sprite.getPosition());
                            bonuses.push_back(std::move(newBonus));
                        }
                    }

                    itProj = projectiles.erase(itProj);
                    et = enemies.erase(et);
                    hit = true;
                    player->addScore(100);
                    break;
                } else {
                    ++et;
                }
            }
            if (!hit) {
                ++itProj;
            }
        }

        for (auto itExplo = explosions.begin(); itExplo != explosions.end(); ) {
            itExplo->update(deltaTime);
            if (itExplo->isFinished) {
                itExplo = explosions.erase(itExplo);
            } else {
                ++itExplo;
            }
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

        if (player->getSpeedBoostTimer() > 0) {
            window.draw(speedSprite);
            window.draw(speedBonusText);
        }

        if (player->getBulletBoostTimer() > 0) {
            window.draw(bulletSprite);
            window.draw(bulletBonusText);
        }

        if (isPaused) { showPause(); }

        window.display();
    }
};
