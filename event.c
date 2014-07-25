#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "event.h"
#include "luautils.h"

lua_State *event_L;

void event_init() {
    event_L = luaL_newstate();
    lua_init_state(event_L, "keys.lua");
    // Actions
    lua_pushnumber(event_L, UP);
    lua_setglobal(event_L, "up");
    lua_pushnumber(event_L, RIGHT);
    lua_setglobal(event_L, "right");
    lua_pushnumber(event_L, DOWN);
    lua_setglobal(event_L, "down");
    lua_pushnumber(event_L, LEFT);
    lua_setglobal(event_L, "left");
    lua_pushnumber(event_L, ACTION);
    lua_setglobal(event_L, "action");
    lua_pushnumber(event_L, DROP);
    lua_setglobal(event_L, "drop");
    lua_pushnumber(event_L, GRAB);
    lua_setglobal(event_L, "grab");

    //SDLKs
    lua_pushnumber(event_L, SDLK_UP);
    lua_setglobal(event_L, "k_up");
    lua_pushnumber(event_L, SDLK_RIGHT);
    lua_setglobal(event_L, "k_right");
    lua_pushnumber(event_L, SDLK_DOWN);
    lua_setglobal(event_L, "k_down");
    lua_pushnumber(event_L, SDLK_LEFT);
    lua_setglobal(event_L, "k_left");
    lua_pushnumber(event_L, SDLK_x);
    lua_setglobal(event_L, "k_x");
    lua_pushnumber(event_L, SDLK_z);
    lua_setglobal(event_L, "k_z");
    lua_pushnumber(event_L, SDLK_c);
    lua_setglobal(event_L, "k_c");

    lua_run(event_L);
}

void event_uninit() {
    lua_uninit_state(event_L);
}

int event_value_from_key(int sdl_key) {
    int value;
    lua_pushnumber(event_L, sdl_key);
    lua_gettable(event_L, -2);
    value = (int)lua_tonumber(event_L, -1);
    lua_pop(event_L, 1);
    return value;
}

void EventList_add_event(EventList *self, Event event) {
    self->length++;
    self->events = realloc(self->events,
                            sizeof(Event) * self->length);
    self->events[self->length - 1] = event;
}

void EventList_destroy(EventList *self) {
    free(self->events);
}

EventList EventList_new() {
    EventList list;
    list.length = 0;
    list.events = malloc(sizeof(Event));
    list.add_event = EventList_add_event;
    list.destroy = EventList_destroy;

    return list;
}
