cmake_minimum_required(VERSION 3.10)

# Global Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# Binary
project(editor)

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/app.c
  ${CMAKE_CURRENT_LIST_DIR}/src/graphics.c
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
  
  ${CMAKE_CURRENT_LIST_DIR}/lodepng/lodepng.c
)

target_link_libraries(${PROJECT_NAME} PUBLIC jrmp glfw GL GLEW m z)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../jrmp/include
  ${CMAKE_CURRENT_LIST_DIR}/cglm
  ${CMAKE_CURRENT_LIST_DIR}/lodepng
)

target_link_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../jrmp/lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY bin
)
