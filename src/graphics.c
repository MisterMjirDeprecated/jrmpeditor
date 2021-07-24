#include "graphics.h"
#include <GL/glew.h>
#include "cglm.h"
#include "lodepng.h"

/* OPENGL STUFF */
int EDTR_file_to_str(char *buffer, long long int buffer_size, const char *fpath)
{
  FILE         *file; /* File to convert */
  long long int size; /* Size of the file */

  file = fopen(fpath, "r");
  if (!file) return -1;

  /* Get the size of the file */
  if (fseeko(file, 0, SEEK_END)) { fclose(file); return -1; }
  size = ftello(file);
  if (size >= buffer_size) { fclose(file); return -1; }
  if (fseeko(file, 0, SEEK_SET) != 0) { fclose(file); return -1; }

  /* Read the file into the buffer, and add a null character */
  if (fread(buffer, sizeof(char), size, file) != size) { fclose(file); return -1; }
  buffer[size] = '\0';

  fclose(file);
  return 0;
}

#include <stdio.h>
/* TODO: These bottom 2 functions are just copy pasted from another project, can be made better */
int EDTR_create_program(unsigned int *id, const char *vshader, const char *fshader)
{
  int success;
  char info[512];
  
  // Make the vertex shader
  char v_src[1024];
  EDTR_file_to_str(v_src, 1024, vshader);
  const char *vp = v_src;
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vp, NULL);
  glCompileShader(vertex_shader);
  
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, &info[0]);
    printf("Error: %s\n", info);
  }

  // Make the fragment shader
  char f_src[1024];
  EDTR_file_to_str(f_src, 1024, fshader);
  const char *fp = f_src;
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fp, NULL);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, &info[0]);
    printf("Error: %s\n", info);
  }

  // Actually make the shader
  *id = glCreateProgram();
  glAttachShader(*id, vertex_shader);
  glAttachShader(*id, fragment_shader);
  glLinkProgram(*id);

  glGetProgramiv(*id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(*id, 512, NULL, &info[0]);
    printf("Error: %s\n", info);
  }

  // Clean up
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return 0;
}

void EDTR_create_texture(unsigned int *id, const char *fpath)
{
  unsigned int error;
  unsigned char *image;
  unsigned int width, height;

  error = lodepng_decode32_file(&image, &width, &height, fpath);
  if (error) return;

  glGenTextures(1, id);
  glBindTexture(GL_TEXTURE_RECTANGLE, *id);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  glBindTexture(GL_TEXTURE_RECTANGLE, *id);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

  free(image);
}

/*
void EDTR_create_texture(spng_ctx *ctx, unsigned int *id, const char *fpath)
{
  FILE *png;

  png = fopen(fpath, "rb");

  spng_set_png_file(ctx, png);

  struct spng_ihdr ihdr;
  size_t out_size;
  void *out;
  int fmt;

  spng_get_ihdr(ctx, &ihdr);
  fmt = SPNG_FMT_RGBA16;
  spng_decoded_image_size(ctx, fmt, &out_size);
  out = malloc(out_size);
  spng_decode_image(ctx, out, out_size, fmt, 0);

  glBindTexture(GL_TEXTURE_2D, *id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ihdr.width, ihdr.height, 0, GL_RGBA, GL_UNSIGNED_SHORT, out);

  fclose(png);
}
*/

/* RENDERER STUFF */
/* TODO: The render functions are just copy paste, they could be made better */
struct EDTR_Renderer * EDTR_renderer_create(unsigned int program)
{
  struct EDTR_Renderer *renderer;

  renderer = malloc(sizeof(struct EDTR_Renderer));
  renderer->program = program;

  /* Set up the VAO and VBO */
  unsigned int vbo;
  float vertices[] = {
    /* Pos        Tex pos  */
    0.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 0.0f,   0.0f, 0.0f,

    0.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 1.0f,   1.0f, 1.0f,
    1.0f, 0.0f,   1.0f, 0.0f
  };

  glGenVertexArrays(1, &renderer->vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(renderer->vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* Pos */
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  /* Text pos */
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /* Un-bind everything */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return renderer;
}

void EDTR_renderer_destroy(struct EDTR_Renderer *renderer)
{
  free(renderer);
}

void EDTR_renderer_draw(struct EDTR_Renderer *renderer, unsigned int texture, int x, int y, int w, int h, float t_x, float t_y, float t_w, float t_h)
{
  glUseProgram(renderer->program);
  
  mat4 model = GLM_MAT4_IDENTITY_INIT;

  glm_translate(model, (vec3) {x, y, 0.0f});

  glm_scale(model, (vec3) {w, h, 1.0f});

  glUniformMatrix4fv(glGetUniformLocation(renderer->program, "model"), 1, GL_FALSE, (float *) model);

  glUniform2f(glGetUniformLocation(renderer->program, "tex_scale"), t_w, t_h);
  glUniform2f(glGetUniformLocation(renderer->program, "tex_translate"), t_x, t_y);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_RECTANGLE, texture);

  glBindVertexArray(renderer->vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

