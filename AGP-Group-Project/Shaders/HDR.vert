#version 330
layout (location = 0) in vec3 position;
layout (location = 4) in vec2 textureCoordinate;

uniform mat4 modelview;
out vec2 UV;

void main(void) {
	UV = textureCoordinate;
	vec4 vertexPosition = modelview * vec4(position, 1.0);
    gl_Position = vertexPosition;
}