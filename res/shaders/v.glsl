#version 330 core
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 color_in;
layout (location = 2) in vec2 tex_pos_in;

out vec3 color;
out vec2 tex_pos;

void main()
{
  gl_Position = vec4(pos_in.x, pos_in.y, pos_in.z, 1.0);
  color = color_in;
  tex_pos = tex_pos_in;
}
