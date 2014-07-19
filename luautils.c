#include <stdlib.h>
#include "luautils.h"

void lua_init_state(lua_State *L, char *filename) {
    int status;

    luaL_openlibs(L);

    status = luaL_loadfile(L, filename);
    if (status) {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
        exit(1);
    }
}

void lua_run(lua_State *L) {
    int result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result) {
        fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
        exit(1);
    }
}

void lua_uninit_state(lua_State *L) {
    lua_pop(L, 1);
    lua_close(L);
}
