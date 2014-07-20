#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "luautils.h"
#include "event.h"
#include "draw_action.h"
#include "start_scene.h"
#include "scene.h"

// Screen dimensions will be stored in SCREEN_WIDTH and SCREEN_HEIGHT
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void draw_sprite(char *sprite_name, int x, int y, SDL_Renderer *renderer,
                 SDL_Texture *sheet, lua_State *L) {
    SDL_Rect source;
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    
    // Select the table named <sprite_name>
    lua_pushstring(L, sprite_name);
    lua_gettable(L, -2);

    // Get x
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    source.x = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Get y
    lua_pushstring(L, "y");
    lua_gettable(L, -2);
    source.y = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Get width
    lua_pushstring(L, "width");
    lua_gettable(L, -2);
    source.w = (int)lua_tonumber(L, -1);
    destination.w = source.w;
    lua_pop(L, 1);

    // Get height
    lua_pushstring(L, "height");
    lua_gettable(L, -2);
    source.h = (int)lua_tonumber(L, -1);
    destination.h = source.h;
    lua_pop(L, 1);

    lua_pop(L, 1);

    SDL_RenderCopy(renderer, sheet, &source, &destination);
}

SDL_Texture *loadTexture(char *path, SDL_Renderer *renderer) {
    SDL_Texture *texture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Couldn't load %s. Shit's fucked, because %s, dude.\n", path,
               IMG_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == NULL) {
            printf("Unable to create texture from %s. Shit's fucked, because %s"
                   ", dude\n", path, SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return texture;
}

int g;

int timer_start_ticks;
void timer_start() {
    timer_start_ticks = SDL_GetTicks();
}
int timer_get_ticks() {
    return SDL_GetTicks() - timer_start_ticks;
}

int main(int argc, char* args[]) {
    // This will be the window we'll be rendering to
    SDL_Window *window = NULL;

    SDL_Renderer *renderer = NULL;

    SDL_Texture *sheet = NULL;

    lua_State *L = luaL_newstate();
    lua_init_state(L, "sprites.lua");
    lua_run(L);
    event_init();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
    } else {
        //if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 1)) {
        //    printf("Warning: Linear texture filtering not enabled");
        //}
        window = SDL_CreateWindow("Piggle",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                printf("Renderer couldn't be created. Shit's fucked, becuase %s"
                       ", dude.\n", SDL_GetError());
            } else {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image couldn't be initialized. Shit's fucked, "
                           "because %s, dude.\n", IMG_GetError());
                }
            }
            sheet = loadTexture("img/sprites.png", renderer);
            // When quit is set to true, we'll stop running
            bool quit = false;
            piggle_scene_over = false;
            // Event handler
            SDL_Event sdl_event;

            // Set the current scene to the start scene
            piggle_scene_update = start_scene_update;

            while (!quit) {
                timer_start();
                EventList events = EventList_new();
                while (SDL_PollEvent(&sdl_event) != 0) {
                    Event event;
                    // If the user presses that little "x"
                    if (sdl_event.type == SDL_QUIT) {
                        quit = true;
                    } else if (sdl_event.type == SDL_KEYDOWN) {
                        event.type = KEYDOWN;
                        int key = sdl_event.key.keysym.sym;
                        event.value = event_value_from_key(key);
                    } else if (sdl_event.type == SDL_KEYUP) {
                        event.type = KEYUP;
                        int key = sdl_event.key.keysym.sym;
                        event.value = event_value_from_key(key);
                    } else {
                        continue;
                    }
                    events.add_event(&events, event);
                }
                SDL_RenderClear(renderer);
                DrawActionList actions = DrawActionList_new();
                actions = piggle_scene_update(events);
                int i;
                for (i = 0; i < actions.length; i++) {
                    DrawAction action = actions.actions[i];
                    draw_sprite(action.sprite, action.x, action.y,
                                renderer, sheet, L);
                }
                SDL_RenderPresent(renderer);
                if (piggle_scene_over) {
                    piggle_scene_update = piggle_scene_next;
                }
                if (timer_get_ticks() < 1000 / 60) {
                    SDL_Delay((1000 / 60) - timer_get_ticks());
                }
            }
        }
    }

    SDL_DestroyTexture(sheet);
    sheet = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();

    lua_uninit_state(L);

    return 0;
}
