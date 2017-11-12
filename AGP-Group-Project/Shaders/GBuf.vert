#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 4) in vec2 texCoord;

uniform mat4 modelview;
uniform mat4 projection;

out ShaderValues {
	vec3 nposition;
	vec3 nnormal;
	vec2 textureCoordinate;
} outData;

void main() {
    vec4 pos = modelview * vec4(position, 1.0);
    outData.nposition = pos.xyz; 
    outData.textureCoordinate = texCoord;
    outData.nnormal = normalize(transpose(inverse(mat3(modelview))) * normal);
    gl_Position = projection * pos;
}