#ifndef SPRITE_H
#define SPRITE_H

namespace frame{
    class spritesheet{
        public:
        int sizeX;
        int sizeY;
        int splitX;
        int splitY;
        char[50] name;
        int numImgs = (sizeX / splitX) * (sizeY / splitY);
        std::string file;
    }
    class sprite{
        public:
        spritesheet sheet;
        char[50] state;
    }
}
#endif SPRITE_H
