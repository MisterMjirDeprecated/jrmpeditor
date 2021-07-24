#ifndef EDTR_APP
#define EDTR_APP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "viewport.h"
#include "jrmp.h"

struct EDTR_App {
  /* Library stuff */
  GLFWwindow *window;

  /* Global app data */
  char             *file_name;
  struct JRMP_Data *data;

  /* Things */
  struct EDTR_Viewport viewport;
};

int EDTR_app_create(struct EDTR_App *);
int EDTR_app_destroy(struct EDTR_App *);
int EDTR_app_run(struct EDTR_App *);

#endif
