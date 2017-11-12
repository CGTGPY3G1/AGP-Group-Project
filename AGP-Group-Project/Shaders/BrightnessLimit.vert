#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 textureCoordinate;
layout (location = 5) in vec4 colour;

uniform mat4 modelview;
out vec2 UV;

void main(void) {
	UV = textureCoordinate;
	vec4 vertexPosition = modelview * vec4(position, 1.0);
    gl_Position = vertexPosition;
}