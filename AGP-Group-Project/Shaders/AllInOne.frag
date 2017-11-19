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

uniform bool useNormalMap;
uniform bool useHeightMap;
uniform bool pointLightsEnabled;
uniform bool directionalLightEnabled;
uniform DirectionLight directionLight;

uniform int numberOfSpotLights;
uniform int numberOfPointLights;
uniform Material material;
uniform sampler2D textureUnit0;
uniform sampler2D textureUnit1;
uniform sampler2D textureUnit2;
uniform samplerCube cubeMaps[MAX_POINT_LIGHTS];
uniform float far;
uniform vec3 pointLightPositions[MAX_POINT_LIGHTS]; 

in ShaderValues {
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
		float closestDepth = (texture(cubeMaps[activeLight], toLight + sampleOffsetDirections[i] * diskRadius).r) * far;
		shadow += (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
	}
	shadow /= float(samples);     
    return shadow;
}

vec3 CalculatePointLighting(int activeLight, vec3 norm, float dist) {
    vec3 lightDirection = normalize(inValues.pointLightPositions[activeLight] - inValues.position);
	float diff = max(dot(norm, lightDirection), 0.0);
	float spec = pow(max(dot(inValues.viewDirection, normalize(lightDirection + inValues.viewDirection)), 0.0), material.shininess);
	float attenuation = clamp(1.0 - dist * dist / (pointLights[activeLight].lightLength * pointLights[activeLight].lightLength), 0.0, 1.0) * pointLights[activeLight].attenuation;  
    vec3 ambient = pointLights[activeLight].ambient * attenuation * material.ambient.xyz;
    vec3 diffuse = pointLights[activeLight].diffuse * attenuation * diff * material.diffuse.xyz;
    vec3 specular = pointLights[activeLight].specular * spec * attenuation * material.specular.xyz;
	return ambient + ((diffuse + specular) * (1.0 - CalculatePointShadow(activeLight)));
}

float CalculateDirectionalShadow(vec3 norm) {
    vec3 projCoords = (inValues.lightSpacePosition.xyz / inValues.lightSpacePosition.w) * 0.5 + 0.5;
	float shadow = 0.0;
	if(projCoords.z < 1.0) {
		float closestDepth = texture(textureUnit2, projCoords.xy).r; 
		float currentDepth = projCoords.z;  
		float bias = max(0.0005 * (1.0 - dot(norm, inValues.directionLightDirection)), 0.00005); 
		vec2 strength = 0.5 / textureSize(textureUnit2, 0);
		int count = 0;
		for(int x = -2; x <= 2; ++x) {
			for(int y = -2; y <= 2; ++y) {
				float pcfDepth = texture(textureUnit2, projCoords.xy + vec2(x, y) * strength).r; 
				shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;   
				count++;     
			}    
		}
		shadow /= count;
	}
    return shadow;
}

vec3 CalculateDirectionLighting(vec3 norm, float shadow) {	
	vec3 lightDirection = inValues.directionLightDirection;
	float diff = max(dot(norm, lightDirection), 0.0);
	float spec = pow(max(dot(inValues.viewDirection, reflect(-lightDirection, norm)), 0.0), material.shininess);
	vec3 ambient = directionLight.ambient * material.ambient.xyz;
	vec3 diffuse = directionLight.ambient * diff * material.diffuse.xyz;
	vec3 specular = directionLight.ambient * spec * material.specular.xyz;      
	return ambient + (diffuse + specular) * directionLight.intensity * (1.0-shadow);
}

void main()
{      
	vec4 tex = texture(textureUnit0, inValues.textureCoordinate);
	vec3 norm = useNormalMap ? inValues.tangentSpaceMatrix * normalize(texture(textureUnit1, inValues.textureCoordinate).rgb * 2.0 - 1.0) : inValues.normal;
	vec3 result = vec3(0.0);
	
	for(int i = 0; i < numberOfPointLights; i++) {
		float dist = length(inValues.pointLightPositions[i] - inValues.position);
		if(dist > pointLights[i].lightLength) continue;
		result += CalculatePointLighting(i, norm, dist);
	}
	if(directionalLightEnabled){
		float directionalShadow = CalculateDirectionalShadow(norm);
		result += CalculateDirectionLighting(norm, directionalShadow);
	}
	colour = vec4(result, material.diffuse.w) * tex; 
}
