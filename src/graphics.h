#ifndef EDTR_GRAPHICS_H
#define EDTR_GRAPHICS_H

/* OpenGL stuff */
int  EDTR_file_to_str(char *buffer, long long int buffer_size, const char *fpath); /* Converts a text file into a string */
int  EDTR_create_program(unsigned int *id, const char *vshader, const char *fshader); /* Creates an OpenGL program */
void EDTR_create_texture(unsigned int *id, const char *fpath); /* Creates an OpenGL texture */

/* Renderer */
struct EDTR_Renderer {
  unsigned int program;
  unsigned int vao;
};

struct EDTR_Renderer * EDTR_renderer_create(unsigned int program);
void                   EDTR_renderer_destroy(struct EDTR_Renderer *renderer);

void EDTR_renderer_draw(struct EDTR_Renderer *renderer, unsigned int texture, int x, int y, int w, int h, float t_x, float t_y, float t_w, float t_h);

#endif
