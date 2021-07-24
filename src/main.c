#include "app.h"

int main(int argc, char *args[])
{
  struct EDTR_App app;

  EDTR_app_create(&app);
  EDTR_app_run(&app);
  EDTR_app_destroy(&app);

  return 0;
}
