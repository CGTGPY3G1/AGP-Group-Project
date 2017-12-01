#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 textureCoordinate;

const int MAX_POINT_LIGHTS = 5;

uniform int numberOfSpotLights;
uniform int numberOfPointLights;
uniform mat4 model;
uniform mat4 viewprojection;
uniform mat4 lightSpace;
uniform mat3 normalMatrix;

uniform vec3 viewPosition;
uniform vec3 directionLightDirection;

out ShaderValues {
	vec3 position;
	vec3 normal;
	vec2 textureCoordinate;
	vec3 viewPosition;
	vec3 viewDirection;
	mat3 tangentSpaceMatrix;
	vec3 directionLightDirection;
	vec4 lightSpacePosition;
} outValues;

void main(void) {
	
	outValues.textureCoordinate = textureCoordinate;
	vec3 n = normalize(vec3(model * vec4(normal, 0.0)));
	vec3 t = normalize(vec3(model * vec4(tangent, 0.0)));
	vec3 bt = normalize(vec3(model * vec4(biTangent, 0.0)));
	outValues.tangentSpaceMatrix = mat3(t, bt, n);
	outValues.normal = normal;
	outValues.position = vec3(model * vec4(position, 1.0));
	outValues.viewPosition = viewPosition;
	outValues.viewDirection = normalize(outValues.viewPosition - outValues.position);
	outValues.directionLightDirection = normalize(-directionLightDirection);
	outValues.lightSpacePosition = lightSpace * (model * vec4(position, 1.0));
	gl_Position = viewprojection * model * vec4(position, 1.0);
}