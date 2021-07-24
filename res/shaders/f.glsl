#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 tex_pos;

uniform float time;
uniform sampler2D texture_a;

void main()
{
  // FragColor = vec4(0.2f, 0.5 * sin(time) + 0.5, 0.8f, 1.0f);
  FragColor = texture(texture_a, tex_pos);
}
