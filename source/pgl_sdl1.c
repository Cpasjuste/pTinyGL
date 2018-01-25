#include <SDL/SDL.h>

#include "GL/pgl.h"
#include "api/zbuffer.h"
#include "api/zgl.h"

static SDL_Surface *surface;
static ZBuffer *frameBuffer;

int pglInit(int width, int height) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("could not init sdl: %s\n", SDL_GetError());
        return -1;
    }

    surface = SDL_SetVideoMode(
            width, height, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!surface) {
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return -1;
    }

    frameBuffer = ZB_open(width, height, ZB_MODE_5R6G5B, 0, 0, 0, 0);
    gl_free(frameBuffer->pbuf);
    frameBuffer->pbuf = surface->pixels;
    glInit(frameBuffer);

    return 0;
}

void pglSwap() {

    SDL_Flip(surface);
}

void pglClose() {

    frameBuffer->frame_buffer_allocated = 0;
    ZB_close(frameBuffer);
    glClose();

    if (surface)
        SDL_FreeSurface(surface);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void *pglGetFrameBuffer() {
    return (void *) frameBuffer->pbuf;
}
