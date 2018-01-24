#ifndef PTINYGL_H
#define PTINYGL_H

#include "gl.h"

#ifdef __cplusplus
extern "C" {
#endif

int pglInit(int width, int height);

void pglSwap();

void pglClose();

void *pglGetFrameBuffer();

#ifdef __cplusplus
}
#endif

#endif // PTINYGL_H
