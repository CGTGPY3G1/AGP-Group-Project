#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 textureCoordinate;

const int MAX_POINT_LIGHTS = 5;

uniform int numberOfSpotLights;
uniform int numberOfPointLights;
uniform sampler2D textureUnit1;
uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 lightSpace;

uniform mat3 normalMatrix;

uniform vec3 viewPosition;
uniform vec3 directionLightDirection;
uniform vec3 pointLightPositions[MAX_POINT_LIGHTS]; 
uniform bool useNormalMap;

out ShaderValues {
	vec3 position;
	vec3 normal;
	vec2 textureCoordinate;
	vec3 viewPosition;
	vec3 viewDirection;
	vec3 directionLightDirection;
	vec3 pointLightPositions[MAX_POINT_LIGHTS];
	vec4 lightSpacePosition;
} outValues;

void main(void) {
	vec3 n = normalize(normalMatrix * normal);
	outValues.textureCoordinate = textureCoordinate;

	if(useNormalMap) {
		vec3 t = normalize(normalMatrix * tangent);
		vec3 bt = normalize(normalMatrix * biTangent);
		mat3 tangentSpaceMatrix = mat3(t, bt, n);
		outValues.normal = tangentSpaceMatrix * normalize(texture(textureUnit1, textureCoordinate).rgb * 2.0 - 1.0);
	}
	else {
		outValues.normal = n;
	}
	outValues.position = vec3(modelview * vec4(position, 1.0));
	outValues.viewPosition = viewPosition;
	outValues.viewDirection = normalize(outValues.viewPosition - outValues.position);
	outValues.directionLightDirection = normalize(-directionLightDirection);
	outValues.lightSpacePosition = lightSpace * (modelview * vec4(position, 1.0));
	for(int i = 0; i < numberOfPointLights; i++)
		outValues.pointLightPositions[i] = pointLightPositions[i];
	gl_Position = projection * modelview * vec4(position, 1.0);
}