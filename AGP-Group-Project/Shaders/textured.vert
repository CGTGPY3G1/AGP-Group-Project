#version 330
layout(location = 0) in vec3 in_Position;
layout(location = 4) in vec2 in_TexCoord;
uniform mat4 model;
out vec2 ex_TexCoord;
void main(void) {
	vec4 vertexPosition = model * vec4(in_Position,1.0);
    gl_Position = vertexPosition;

	ex_TexCoord = in_TexCoord;
}
