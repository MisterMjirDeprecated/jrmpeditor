#include "app.h"
#include "graphics.h"
#include "cglm.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/*
 * @desc
 *   Initialize app and all libraries
 */
int EDTR_app_create(struct EDTR_App *app)
{
  /* GLFW, GLEW, GL */
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  if (!(app->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "JRMP Editor", NULL, NULL))) { glfwTerminate(); return -1; }
  glfwMakeContextCurrent(app->window);
  
  if (glewInit() != GLEW_OK) { glfwTerminate(); return -1; }

  /* APP STATE */
  app->file_name = "map.jrmp";

  /* VIEWPORT */
  app->viewport.width = 640;
  app->viewport.height = 320;

  return 0;
}

int EDTR_app_destroy(struct EDTR_App *app)
{
  glfwTerminate();
  
  return 0;
}

int EDTR_app_run(struct EDTR_App *app)
{
  /* GL and Graphics */
  unsigned int sprite_program;
  EDTR_create_program(&sprite_program, "res/shaders/sprite_v.glsl", "res/shaders/sprite_f.glsl");

  unsigned int texture;
  EDTR_create_texture(&texture, "res/tiles.png");
  
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  //glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1.0f, 1.0f);

  glUseProgram(sprite_program);

  mat4 projection;
  glm_ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f, projection);
  glUniformMatrix4fv(glGetUniformLocation(sprite_program, "projection"), 1, GL_FALSE, (float *) projection);

  struct EDTR_Renderer *renderer = EDTR_renderer_create(sprite_program);
 
  /* Data */
  app->data = malloc(sizeof(struct JRMP_Data));
  JRMP_data_create(app->data, "res/default.jrmp");
  JRMP_data_to_files(app->data);
  /* Set up viewport */
  app->viewport.camera.x = 0x8000000000000000;
  app->viewport.camera.y = 0x8000000000000000;
  uint8_t layers;
  JRMP_data_block_seek(app->data, ".MAPINFO");
  
  /* Not temporary: Main loop */
  while (!glfwWindowShouldClose(app->window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw the viewport */
    EDTR_viewport_draw(&app->viewport);

    /* Test the renderer */
    EDTR_renderer_draw(renderer, texture, 0, 0, 32, 32, 32, 0, 32, 32);

    glfwSwapBuffers(app->window);
    glfwPollEvents();
  }

  JRMP_data_destory(app->data);
  free(app->data);

  glDeleteProgram(sprite_program);

  EDTR_renderer_destroy(renderer);

  return 0;
}
