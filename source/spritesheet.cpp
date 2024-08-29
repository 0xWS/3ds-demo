#include "spritesheet.hpp"
#include <cstdio>
#include <stdio.h>

//Constructor
SpriteManager::SpriteManager() : spriteSheet(nullptr) {}

//Destructor
SpriteManager::~SpriteManager() {
    if (spriteSheet) {
        C2D_SpriteSheetFree(spriteSheet);
    }
}

bool SpriteManager::loadSpriteSheet(const std::string& path) {
    spriteSheet = C2D_SpriteSheetLoad(path.c_str());
    if (!spriteSheet) {
        return false;
    }
    cutSpriteSheet();
    return true;
}

void SpriteManager::cutSpriteSheet() {
    // Assuming the first sprite is the ball and the second is the paddle
    sprites.push_back(C2D_SpriteSheetGetImage(spriteSheet, 0));
    sprites.push_back(C2D_SpriteSheetGetImage(spriteSheet, 1));
}

C2D_Image SpriteManager::getBallSprite() {
    return sprites[0];
}

C2D_Image SpriteManager::getPaddleSprite() {
    return sprites[1];
}