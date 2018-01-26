#include <switch.h>

#include "pTinyGL/pgl.h"
#include "api/zbuffer.h"
#include "api/zgl.h"

static ZBuffer *pglBuffer;
static u8 *nxBuffer;

int pglInit(int width, int height) {

    gfxInitDefault();

    pglBuffer = ZB_open(width, height, ZB_MODE_RGBA, 0, 0, 0, 0);
    glInit(pglBuffer);

    return 0;
}

void pglSwap() {

    // clear
    int w, h;
    nxBuffer = gfxGetFramebuffer((u32 *) &w, (u32 *) &h);

    ZB_copyFrameBuffer(pglBuffer, nxBuffer, w * 4);

    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();
}

void pglClose() {

    ZB_close(pglBuffer);
    glClose();
    gfxExit();
}

void *pglGetFrameBuffer() {
    return (void *) pglBuffer->pbuf;
}
