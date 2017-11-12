#version 330
precision highp float;
uniform sampler2D textureUnit0;

uniform vec4 colour;
in vec2 nTextureCoordinate;
out vec4 out_color;

void main(void) {
	out_color = colour * texture(textureUnit0, nTextureCoordinate);
}