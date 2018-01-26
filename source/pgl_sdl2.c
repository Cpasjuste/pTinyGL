#include <SDL2/SDL.h>

#include "pTinyGL/pgl.h"
#include "api/zbuffer.h"
#include "api/zgl.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *window_surface;
static SDL_Surface *blit_surface;
static ZBuffer *frameBuffer;

int pglInit(int width, int height) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("could not init sdl: %s\n", SDL_GetError());
        return -1;
    }

    Uint32 flags = SDL_WINDOW_OPENGL;
    if (!width || !height) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(
            "PTinyGL_SDL2",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, flags);

    if (!window) {
        window = SDL_CreateWindow(
                "PTinyGL_SDL2",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                width, height, 0);
        if (!window) {
            printf("Couldn't create window: %s\n", SDL_GetError());
            return -1;
        }
    }

    window_surface = SDL_GetWindowSurface(window);
    if (!window_surface) {
        printf("Couldn't create surface: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateSoftwareRenderer(window_surface);
    if (!renderer) {
        printf("Couldn't create renderer: %s\n", SDL_GetError());
        return -1;
    }

    blit_surface = SDL_CreateRGBSurface(0, width, height, 16, 0, 0, 0, 0);
    frameBuffer = ZB_open(width, height, ZB_MODE_5R6G5B, 0, 0, 0, 0);
    gl_free(frameBuffer->pbuf);
    frameBuffer->pbuf = blit_surface->pixels; //surface->pixels;
    glInit(frameBuffer);

    return 0;
}

void pglSwap() {

    SDL_BlitSurface(blit_surface, NULL, window_surface, NULL);
    SDL_UpdateWindowSurface(window);
}

void pglClose() {

    frameBuffer->frame_buffer_allocated = 0;
    ZB_close(frameBuffer);
    glClose();

    if (blit_surface)
        SDL_FreeSurface(blit_surface);
    if (window_surface)
        SDL_FreeSurface(window_surface);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void *pglGetFrameBuffer() {
    return (void *) frameBuffer->pbuf;
}
