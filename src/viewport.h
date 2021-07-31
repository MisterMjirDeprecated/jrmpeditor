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

struct EDTR_Viewport {
  int     width;
  int     height;
  int     tile_size;

  int64_t camera_x;
  int64_t camera_y;
  uint8_t layer;
  uint8_t flags;
};

int EDTR_viewport_draw(struct EDTR_Viewport *);

#endif
