#ifndef EDTR_VIEWPORT_H
#define EDTR_VIEWPORT_H

#include <stdint.h>

/*
 * VIEWPORT
 *
 * Contains the view of the game, really just the tiles
 *
 * Needs OpenGL stuff set up
 */

enum EDTR_ViewportFlags {
  EDTR_VIEWPORT_DRAW_ALL = 0
};

struct EDTR_Camera {
  int64_t   x;
  int64_t   y;

  uint64_t *offsets; /* File offsets */
};

struct EDTR_Viewport {
  int     width;
  int     height;
  int     tile_size;

  struct EDTR_Camera camera;
  uint8_t layer;
  uint32_t tiles_x;
  uint32_t tiles_y;
};

int EDTR_viewport_draw(struct EDTR_Viewport *);

#endif
