#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
uniform float offset;

void main(void) {
  vec2 texcoord = UV;
  texcoord.x += sin(UV.y * 4*2*3.14159 + offset) / 100.0;
  colour = texture2D(textureUnit0, texcoord);
}