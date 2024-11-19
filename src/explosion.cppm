module;

#include <SFML/Graphics.hpp>
#include <vector>


using namespace sf;
using namespace std;

export module Explosion;

export class Explosion {
public:
    Sprite sprite;
    std::vector<IntRect> frames;
    float frameDuration;
    float currentFrameTime = 0.0f;
    size_t currentFrame = 0;
    bool isFinished = false;

    Explosion(const Texture& texture, int frameWidth, int frameHeight, int frameCount, float frameDuration)
        : frameDuration(frameDuration) {
        sprite.setTexture(texture);

        for (int i = 0; i < frameCount; ++i) {
            frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
        }

        sprite.setTextureRect(frames[0]);
    }

    void update(float deltaTime) {
        if (isFinished) return;

        currentFrameTime += deltaTime;

        if (currentFrameTime >= frameDuration) {
            currentFrameTime = 0.0f;
            currentFrame++;

            if (currentFrame < frames.size()) {
                sprite.setTextureRect(frames[currentFrame]);
            } else {
                isFinished = true;
            }
        }
    }
};
