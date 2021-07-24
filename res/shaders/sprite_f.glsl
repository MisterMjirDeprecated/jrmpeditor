#version 330 core
in vec2 tex_pos;
out vec4 color;

uniform sampler2DRect image;
uniform vec2 tex_scale;
uniform vec2 tex_translate;

void main()
{
  vec2 real_tex_pos = tex_pos;

  real_tex_pos *= tex_scale;
  real_tex_pos += tex_translate;

  vec4 tex_color = texture(image, real_tex_pos);
  if (tex_color.a < 1.0) discard;
  color = tex_color;
}
