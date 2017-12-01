// based on and adapted from https://en.wikibooks.org/wiki/OpenGL_Programming/Post-Processing#A_first_effect
#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
uniform float offset;
float constantVal = 32.5;
void main(void) {
  vec2 texcoord = UV;
  // offset the UVs x component
  texcoord.x += sin(UV.y * constantVal + offset) * 0.01;
  colour = texture2D(textureUnit0, texcoord);
}