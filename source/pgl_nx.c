#include <switch.h>

#include "pTinyGL/pgl.h"
#include "api/zbuffer.h"
#include "api/zgl.h"

static ZBuffer *pglBuffer;
static u32 *nxBuffer;

int pglInit(int width, int height) {

    gfxInitDefault();
    gfxSetDoubleBuffering(false);
    gfxFlushBuffers();
    gfxWaitForVsync();

    nxBuffer = (u32 *) gfxGetFramebuffer(NULL, NULL);
    pglBuffer = ZB_open(width, height, ZB_MODE_RGBA, 0, 0, 0, 0);
    glInit(pglBuffer);

    return 0;
}

#define RGB16_TO_RGB32(p0, p1, v)\
{\
    unsigned int g,b,gb;\
    g = (v & 0x07E007E0) << 5;\
    b = (v & 0x001F001F) << 3;\
    gb = g | b;\
    p0 = (gb & 0x0000FFFF) | ((v & 0x0000F800) << 8);\
    p1 = (gb >> 16) | ((v & 0xF8000000) >> 8);\
}

static void ZB_copyFrameBufferRGB32(ZBuffer *zb, u32 *buf, int linesize) {

    /*
    u32 x, y;
    //u32 *src = (u32 *) zb->pbuf;

    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {

            //u32 *dst = (u32 *) buf[(u32) gfxGetFramebufferDisplayOffset(x, y)];
            u32 *src = (u32 *) zb->pbuf[x + y];
            buf[(u32) gfxGetFramebufferDisplayOffset(x, y)] =
                    RGBA8(src[2], src[1], src[0], 255);
            //src += 4;
            //pixel[0] = zb->pbuf[0];
            //buf[gfxGetFramebufferDisplayOffset(x, y)] = 0x01010101 * cnt * 4;
        }
    }
    */

    unsigned short *q;
    //unsigned int *p, *p1, v, w0, w1;
    unsigned int v, w0, w1;
    int y, n;

    q = zb->pbuf;
    //p1 = buf;

    //printf("n: %i\n", zb->ysize);

    for (y = 0; y < zb->ysize; y++) {

        //p = p1;
        n = zb->xsize >> 2;

        do {

            unsigned int *pixel = &buf[(u32) gfxGetFramebufferDisplayOffset((u32) n, (u32) y)];

            v = *(unsigned int *) q;
            RGB16_TO_RGB32(w0, w1, v);
            pixel[0] = w0;
            pixel[1] = w1;

            v = *(unsigned int *) (q + 2);
            RGB16_TO_RGB32(w0, w1, v);
            pixel[2] = w0;
            pixel[3] = w1;

            q += 4;
            //p += 4;

        } while (--n > 0);

        //p1 += linesize;
    }
}

void pglSwap() {

    ZB_copyFrameBufferRGB32(pglBuffer, nxBuffer, 1280 * 4);
    //u32 x, y;
    //for (x = 0; x < w; x++)
    //    for (y = 0; y < h; y++)
    //        nxBuffer[gfxGetFramebufferDisplayOffset(x, y)] = 0x01010101 * cnt * 4;

    gfxFlushBuffers();
    gfxSwapBuffers();
    gfxWaitForVsync();
}

void pglClose() {

    glClose();
    ZB_close(pglBuffer);
    gfxExit();
}

void *pglGetFrameBuffer() {
    return (void *) pglBuffer->pbuf;
}
