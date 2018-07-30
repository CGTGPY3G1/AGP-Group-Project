#version 330
layout (location = 0) in vec3 position;
layout (location = 4) in vec2 textureCoordinate;

uniform mat4 model;
out vec2 UV;

void main(void) {
	UV = textureCoordinate;
	vec4 vertexPosition = model * vec4(position, 1.0);
    gl_Position = vertexPosition;
}