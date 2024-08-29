#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include <citro2d.h>
#include <string>
#include <vector>

class SpriteManager {
public:
    SpriteManager();
    ~SpriteManager();

    bool loadSpriteSheet(const std::string& path);
    C2D_Image getBallSprite();
    C2D_Image getPaddleSprite();

private:
    C2D_SpriteSheet spriteSheet;
    std::vector<C2D_Image> sprites;

    void cutSpriteSheet();
};

#endif // SPRITE_MANAGER_HPP