#include "sprites.h"
#include "luautils.h"

void set_sprite(SDL_Rect *source, char sprite_name[255], lua_State *L) {
    // Select the table named <sprite_name>
    lua_pushstring(L, sprite_name);
    lua_gettable(L, -2);

    // Get x
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    source->x = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Get y
    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    source->y = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Get width
    lua_pushstring(L, "width");
    lua_gettable(L, -2);
    source->w = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Get height
    lua_pushstring(L, "height");
    lua_gettable(L, -2);
    source->h = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_pop(L, 1);
}

void init_sprites() {
    lua_State *L = luaL_newstate();
    lua_init_state(L, "sprites.lua");
    lua_run(L);

    set_sprite(&sprites[PLAYER_UP], "player_up", L);
    set_sprite(&sprites[PLAYER_RIGHT], "player_right", L);
    set_sprite(&sprites[PLAYER_DOWN], "player_down", L);
    set_sprite(&sprites[PLAYER_LEFT], "player_left", L);
    set_sprite(&sprites[GRASS], "grass", L);
    set_sprite(&sprites[WATER], "water", L);
    set_sprite(&sprites[TREE], "tree", L);
    set_sprite(&sprites[AXE], "axe", L);
    set_sprite(&sprites[TINDERBOX], "tinderbox", L);
    set_sprite(&sprites[WHEAT], "wheat", L);
    set_sprite(&sprites[START], "start", L);
    set_sprite(&sprites[LOG], "log", L);

    lua_uninit_state(L);
}
