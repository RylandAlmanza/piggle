#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>

enum Sprites {
    PLAYER_UP,
    PLAYER_RIGHT,
    PLAYER_DOWN,
    PLAYER_LEFT,
    GRASS,
    WATER,
    TREE,
    AXE,
    TINDERBOX,
    WHEAT,
    LOG,
    START,
    FISHING_ROD
};

SDL_Rect sprites[13];

void init_sprites();

#endif
