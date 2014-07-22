#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "luautils.h"
#include "event.h"
#include "draw_action.h"
#include "start_scene.h"
#include "scene.h"
#include "sprites.h"

typedef struct TextStruct Text;

struct TextStruct {
    SDL_Texture *texture;
    int width;
    int height;
};

// Screen dimensions will be stored in SCREEN_WIDTH and SCREEN_HEIGHT
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

void *textureFromText(char *str, SDL_Color color, Text *text, TTF_Font *font,
                      SDL_Renderer *renderer) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, str, color);
    if (surface == NULL) {
        printf("Unable to render text surface. Shit's fucked, because %s, dude"
               ".", TTF_GetError());
    } else {
        text->texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (text->texture == NULL) {
            printf("Unable to create texture from rendered text. Shit's fucked"
                   " because %s, dude.", SDL_GetError());
        } else {
            text->width = surface->w;
            text->height = surface->h;
        }
        SDL_FreeSurface(surface);
    }
}

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

    TTF_Font *font;

    SDL_Texture *sheet = NULL;
    Text text;

    init_sprites();
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

                if (TTF_Init() == -1) {
                    printf("SDL_ttf couldn't initialize. Shit's fucked, becuase"
                           " %s, dude.\n", TTF_GetError());
                }
            }
            font = TTF_OpenFont("DroidSansMono.ttf", 16);
            if (font == NULL) {
                printf("Failed to load lazy font. Shit's fucked, because %s, "
                       "dude.", TTF_GetError());
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
                        event.type = QUIT;
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
                piggle_scene_update(&events, &actions);
                events.destroy(&events);
                int i;
                for (i = 0; i < actions.length; i++) {
                    DrawAction action = actions.actions[i];
                    if (action.type == SPRITE) {
                        SDL_Rect *sprite = &sprites[action.sprite];
                        SDL_Rect dest;
                        dest.x = action.x;
                        dest.y = action.y;
                        dest.w = sprite->w;
                        dest.h = sprite->h;
                        SDL_RenderCopy(renderer, sheet, sprite, &dest);
                    } else if (action.type == TEXT) {
                        SDL_Color color = {action.text.red,
                                           action.text.green,
                                           action.text.blue};
                        textureFromText(action.text.text, color, &text, font,
                                        renderer);
                        SDL_Rect textRect;
                        textRect.x = action.x;
                        textRect.y = action.y;
                        textRect.w = text.width;
                        textRect.h = text.height;
                        SDL_RenderCopy(renderer, text.texture, NULL, &textRect);
                        SDL_DestroyTexture(text.texture);
                    } else if (action.type == RECTANGLE) {
                        SDL_Rect rect = {.x = action.x,
                                         .y = action.y,
                                         .w = action.rect.width,
                                         .h = action.rect.height};
                        SDL_SetRenderDrawColor(renderer, action.rect.red,
                                               action.rect.green,
                                               action.rect.blue, 255);
                        SDL_RenderFillRect(renderer, &rect);
                    }
                }
                SDL_RenderPresent(renderer);
                actions.destroy(&actions);
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
    SDL_DestroyTexture(text.texture);
    text.texture = NULL;
    sheet = NULL;
    TTF_CloseFont(font);
    font = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    event_uninit();

    return 0;
}
