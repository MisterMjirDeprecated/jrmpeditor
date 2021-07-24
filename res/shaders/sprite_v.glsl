#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex_pos_in;

out vec2 tex_pos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
  //gl_Position = projection * model * vec4(pos.x, pos.y, 0.0, 1.0);
  gl_Position = projection * model * vec4(pos.x, pos.y, 0.0, 1.0);
  tex_pos = tex_pos_in;
}
