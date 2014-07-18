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

void draw_sprite(char *sprite_name, int x, int y, SDL_Surface *screen_surface,
                 SDL_Surface *sheet, lua_State *L) {
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

    SDL_BlitSurface(sheet, &source, screen_surface, &destination);
}

SDL_Surface *load_surface(char *path, SDL_Surface *screen_surface) {
    // optimizedSurface will point to the final optimized image
    SDL_Surface *optimized_surface = NULL;

    // Load image from the specified path
    SDL_Surface *loaded_surface = IMG_Load(path);


    if (loaded_surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n",
               path,
               IMG_GetError());
    } else {
        // Convert surface to screen format
        optimized_surface = SDL_ConvertSurface(loaded_surface,
                                              screen_surface->format,
                                              NULL);
        if (optimized_surface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n",
                   path,
                   SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loaded_surface);
    }

    return optimized_surface;
}

int g;

int main(int argc, char* args[]) {
    // This will be the window we'll be rendering to
    SDL_Window *window = NULL;

    // This will be the surface contained by the window
    SDL_Surface *screen_surface = NULL;

    // This will store the spritesheet
    SDL_Surface *sheet = NULL;
    lua_State *L = luaL_newstate();
    lua_init_state(L, "sprites.lua");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
    } else {
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
            // Get window surface
            screen_surface = SDL_GetWindowSurface(window);
            // Get gotchi surface
            sheet = load_surface("img/sprites.png", screen_surface);
            // When quit is set to true, we'll stop running
            bool quit = false;
            piggle_scene_over = false;
            // Event handler
            SDL_Event sdl_event;

            // Set the current scene to the start scene
            piggle_scene_update = start_scene_update;

            while (!quit) {
                EventList events = EventList_new();
                while (SDL_PollEvent(&sdl_event) != 0) {
                    Event event;
                    // If the user presses that little "x"
                    if (sdl_event.type == SDL_QUIT) {
                        quit = true;
                    }
                    if (sdl_event.type == SDL_KEYDOWN) {
                        event.type = KEYDOWN;
                        switch (sdl_event.key.keysym.sym) {
                            case SDLK_UP:
                                event.value = KEY_UP;
                                break;
                            case SDLK_RIGHT:
                                event.value = KEY_RIGHT;
                                break;
                            case SDLK_DOWN:
                                event.value = KEY_DOWN;
                                break;
                            case SDLK_LEFT:
                                event.value = KEY_LEFT;
                                break;
                            default:
                                event.value = KEY_UP;
                                break;
                        }
                    }
                    events.add_event(&events, event);
                }
                // Fill the surface black
                SDL_FillRect(screen_surface,
                             NULL,
                             SDL_MapRGB(screen_surface->format,
                                        0x00,
                                        0x00,
                                        0x00));
                DrawActionList actions = DrawActionList_new();
                actions = piggle_scene_update(events);
                int i;
                for (i = 0; i < actions.length; i++) {
                    DrawAction action = actions.actions[i];
                    draw_sprite(action.sprite, action.x, action.y,
                                screen_surface, sheet, L);
                }
                // Update the surface
                SDL_UpdateWindowSurface(window);
                if (piggle_scene_over) {
                    piggle_scene_update = piggle_scene_next;
                }
            }
        }
    }

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    lua_uninit_state(L);

    return 0;
}
