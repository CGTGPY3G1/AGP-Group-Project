// renders quads
#version 330
layout (location = 0) in vec3 position;
layout (location = 4) in vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 viewviewprojection;

out vec2 nTextureCoordinate;

void main(void) {
	nTextureCoordinate = textureCoordinate;
    gl_Position = viewviewprojection * model * vec4(position, 1.0);
}
