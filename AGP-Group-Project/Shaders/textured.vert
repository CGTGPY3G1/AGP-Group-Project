// textured.vert
// use textures, but no lighting
#version 330
layout(location = 0) in vec3 in_Position;
layout(location = 4) in vec2 in_TexCoord;

uniform mat4 modelview;



out vec2 ex_TexCoord;

// multiply each vertex position by the MVP matrix
void main(void) {

	// vertex into eye coordinates
	vec4 vertexPosition = modelview * vec4(in_Position,1.0);
    gl_Position = vertexPosition;

	ex_TexCoord = in_TexCoord;
}
