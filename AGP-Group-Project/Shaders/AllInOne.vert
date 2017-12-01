#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 biTangent;
layout (location = 4) in vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 viewprojection;
// to direction lights coordinate matrix
uniform mat4 lightSpace; 
uniform mat3 normalMatrix;

uniform vec3 viewPosition;
uniform vec3 directionLightDirection;
// to be passes to fragment shader
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
	// calculate a tangent to model space matrix
	vec3 n = normalize(normalMatrix * normal);
	vec3 t = normalize(normalMatrix * tangent);
	vec3 bt = normalize(normalMatrix * biTangent);
	outValues.tangentSpaceMatrix = mat3(t, bt, n);
	// send the default normal to the fragment shader, incase normal mapping is disabled
	outValues.normal = normalize(normalMatrix *normal); 
	outValues.position = vec3(model * vec4(position, 1.0));
	outValues.viewPosition = viewPosition;
	outValues.viewDirection = normalize(viewPosition - outValues.position);
	outValues.directionLightDirection = normalize(-directionLightDirection);
	// position in direction lights coordinates
	outValues.lightSpacePosition = lightSpace * (model * vec4(position, 1.0)); 
	gl_Position = viewprojection * model * vec4(position, 1.0);
}