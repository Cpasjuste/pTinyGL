#include <vita2d.h>

#include "GL/pgl.h"
#include "api/zbuffer.h"
#include "api/zgl.h"

static vita2d_texture *texture;
static ZBuffer *frameBuffer;

int pglInit(int width, int height) {

    vita2d_init();

    texture = vita2d_create_empty_texture_format(
            (unsigned int) width, (unsigned int) height,
            SCE_GXM_TEXTURE_FORMAT_R5G6B5);

    frameBuffer = ZB_open(width, height, ZB_MODE_5R6G5B, 0, 0, 0, 0);
    gl_free(frameBuffer->pbuf);
    frameBuffer->pbuf = (PIXEL *) vita2d_texture_get_datap(texture);
    glInit(frameBuffer);

    return 0;
}

void pglSwap() {

    vita2d_start_drawing();
    vita2d_draw_texture(texture, 0, 0);
    vita2d_end_drawing();
    vita2d_swap_buffers();
}

void pglClose() {

    frameBuffer->frame_buffer_allocated = 0;
    ZB_close(frameBuffer);
    glClose();

    vita2d_wait_rendering_done();
    vita2d_free_texture(texture);
    vita2d_fini();
}

void *pglGetFrameBuffer() {
    return (void *) frameBuffer->pbuf;
}
