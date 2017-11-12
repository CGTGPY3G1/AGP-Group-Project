#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 textureCoordinate;
layout (location = 5) in vec4 colour;

const int MAX_POINT_LIGHTS = 11;

uniform int numberOfSpotLights;
uniform int numberOfPointLights;

uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 lightSpace;

uniform mat3 normalMatrix;

uniform vec3 viewPosition;
uniform vec3 directionLightDirection;
uniform vec3 pointLightPositions[MAX_POINT_LIGHTS]; 

out ShaderValues {
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec3 biTangent;
	vec2 textureCoordinate;
	mat3 tangentSpaceMatrix;
	vec3 viewPosition;
	vec3 viewDirection;
	vec3 directionLightDirection;
	vec3 pointLightPositions[MAX_POINT_LIGHTS];
	vec4 lightSpacePosition;
} outValues;

void main(void) {
	outValues.normal = normalize(normalMatrix * normal);
	outValues.tangent = normalize(normalMatrix * tangent);
	outValues.biTangent = normalize(normalMatrix * biTangent);
	outValues.textureCoordinate = textureCoordinate;
	outValues.tangentSpaceMatrix = mat3(outValues.tangent, outValues.biTangent, outValues.normal);
	outValues.position = vec3(modelview * vec4(position, 1.0));
	outValues.viewPosition = viewPosition;
	outValues.viewDirection = normalize(outValues.viewPosition - outValues.position);
	outValues.directionLightDirection = normalize(-directionLightDirection);
	outValues.lightSpacePosition = lightSpace * (modelview * vec4(position, 1.0));
	for(int i = 0; i < numberOfPointLights; i++)
		outValues.pointLightPositions[i] = pointLightPositions[i];
	gl_Position = projection * modelview * vec4(position, 1.0);
}