#ifndef LUAUTILS_H
#define LUAUTILS_H

#include <stdbool.h>
#include <lua.h>
#include <lauxlib.h>

void lua_init_state(lua_State *L, char *filename);
void lua_uninit_state(lua_State *L);

#endif
