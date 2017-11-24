#version 330

struct DirectionLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float intensity;
};

const int MAX_POINT_LIGHTS = 5;

uniform struct PointLight { 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float attenuation;
	float lightLength; 
} pointLights[MAX_POINT_LIGHTS];

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform bool pointLightsEnabled;
uniform bool castShadows;
uniform bool directionalLightEnabled;
uniform DirectionLight directionLight;

uniform int numberOfSpotLights;
uniform int numberOfPointLights;
uniform Material material;
uniform sampler2D textureUnit0;

uniform sampler2D textureUnit2;
uniform samplerCube cubeMap0;
uniform samplerCube cubeMap1;
uniform samplerCube cubeMap2;
uniform samplerCube cubeMap3;
uniform samplerCube cubeMap4;

uniform float far;
uniform vec3 pointLightPositions[MAX_POINT_LIGHTS]; 

in ShaderValues {
	vec3 position;
	vec3 normal;
	vec2 textureCoordinate;
	vec3 viewPosition;
	vec3 viewDirection;
	vec3 directionLightDirection;
	vec3 pointLightPositions[MAX_POINT_LIGHTS];
	vec4 lightSpacePosition;
} inValues;

const int samples = 20;
vec3 sampleOffsetDirections[samples] = vec3[] (
   vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
   vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
   vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
   vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

out vec4 colour;

float CalculatePointShadow(int activeLight) {
    vec3 toLight = (inValues.position - pointLightPositions[activeLight]);
    float currentDepth = length(toLight);
	float shadow = 0.0;
	float bias = 0.015;
	float viewDistance = length(inValues.viewPosition - inValues.position);
	float diskRadius = 0.005;
	for(int i = 0; i < samples; ++i) {
		float closestDepth = 0.0;
		if(activeLight == 0) closestDepth = (texture(cubeMap0, toLight + sampleOffsetDirections[i] * diskRadius).r) * far;
		else if(activeLight == 1) closestDepth = (texture(cubeMap1, toLight + sampleOffsetDirections[i] * diskRadius).r) * far;
		else if(activeLight == 2) closestDepth = (texture(cubeMap2, toLight + sampleOffsetDirections[i] * diskRadius).r) * far;
		else if(activeLight == 3) closestDepth = (texture(cubeMap3, toLight + sampleOffsetDirections[i] * diskRadius).r) * far;
		else if(activeLight == 4) closestDepth = (texture(cubeMap4, toLight + sampleOffsetDirections[i] * diskRadius).r) * far;
		shadow += (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
	}
	shadow /= float(samples);     
    return shadow;
}

vec3 CalculatePointLighting(int activeLight, vec3 norm, float dist) {
    vec3 lightDirection = normalize(inValues.pointLightPositions[activeLight] - inValues.position);
	float spec = pow(max(dot(norm, normalize(lightDirection + inValues.viewDirection)), 0.0), material.shininess);
	float attenuation = (1.0 - (dist / pointLights[activeLight].lightLength)) * pointLights[activeLight].attenuation;  
    vec3 ambient = pointLights[activeLight].ambient * material.ambient.xyz;
	float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = pointLights[activeLight].diffuse * diff * material.diffuse.xyz;
    vec3 specular = pointLights[activeLight].specular * spec * material.specular.xyz;
	float shadow = castShadows ? CalculatePointShadow(activeLight) : 0.0;
	return ambient + ((diffuse + specular) * attenuation * (1.0 - shadow));
}

float CalculateDirectionalShadow(vec3 norm) {
    vec3 projCoords = (inValues.lightSpacePosition.xyz / inValues.lightSpacePosition.w) * 0.5 + 0.5;
	float shadow = 0.0;
	if(projCoords.z < 1.0) {
		float closestDepth = texture(textureUnit2, projCoords.xy).r; 
		float currentDepth = projCoords.z;  
		float bias = max(0.0005 * (1.0 - dot(norm, inValues.directionLightDirection)), 0.00005); 
		vec2 texelSize = 1.0 / textureSize(textureUnit2, 0);
		int count = 0;
		for(int x = -2; x <= 2; ++x) {
			for(int y = -2; y <= 2; ++y) {
				float pcfDepth = texture(textureUnit2, projCoords.xy + vec2(x, y) * texelSize).r; 
				shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;   
				count++;     
			}    
		}
		shadow /= count;
	}
    return clamp(shadow, 0.0, 1.0);
}

vec3 CalculateDirectionLighting(vec3 norm, float shadow) {	
	vec3 lightDirection = inValues.directionLightDirection;
	float diff = max(dot(norm, lightDirection), 0.0);
	float spec = pow(max(dot(inValues.viewDirection, reflect(-lightDirection, norm)), 0.0), material.shininess);
	vec3 ambient = directionLight.ambient * material.ambient.xyz;
	vec3 diffuse = directionLight.diffuse * diff * material.diffuse.xyz;
	vec3 specular = directionLight.specular * spec * material.specular.xyz;      
	return ambient + (diffuse + specular) * (1.0-shadow) * directionLight.intensity;
}

void main()
{      
	vec4 tex = texture(textureUnit0, inValues.textureCoordinate);
	vec3 norm = inValues.normal;
	vec3 result = vec3(0.0);
	
	for(int i = 0; i < numberOfPointLights; i++) {
		float dist = distance(inValues.pointLightPositions[i], inValues.position);
		if(dist > pointLights[i].lightLength) continue;
		result += CalculatePointLighting(i, norm, dist);
	}
	if(directionalLightEnabled){
		float directionalShadow = castShadows ? CalculateDirectionalShadow(norm) : 0.0f;
		result += CalculateDirectionLighting(norm, directionalShadow);
	}
	colour = vec4(result, material.diffuse.w) * tex; 
}
