//
// Created by cpasjuste on 24/01/18.
//

#include <stdio.h>
#include <pTinyGL/pgl.h>
#include "lodepng.h"
#include "pfba.h"

#define SCREEN_W 1280
#define SCREEN_H 720

typedef struct TEXTURE {

    GLuint id;
    unsigned int width;
    unsigned int height;

} TEXTURE;

int load_texture(TEXTURE *texture) {

    unsigned int error = 0;
    unsigned char *pixels = NULL;

    error = lodepng_decode32(&pixels, &texture->width, &texture->height,
                             (const unsigned char *) pfba_png, pfba_png_length);
    if (error) {
        printf("load_texture error %u: %s\n", error, lodepng_error_text(error));
        return -1;
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, texture->width, texture->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return 0;
}

int main() {

    pglInit(SCREEN_W, SCREEN_H);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_W, SCREEN_H, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    TEXTURE tex;
    if (load_texture(&tex) != 0) {
        pglClose();
        return -1;
    }

    for (int i = 0; i < 60; i++) {

        // clear screen
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex.id);

        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(0, 0);
        glTexCoord2f(0, 1);
        glVertex2f(0, tex.height);
        glTexCoord2f(1, 1);
        glVertex2f(tex.width, tex.height);
        glTexCoord2f(1, 0);
        glVertex2f(tex.width, 0);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        pglSwap();
    }

    pglClose();

    return 0;
}
