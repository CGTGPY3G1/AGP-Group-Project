// based on and heavily adapted from tutorials found at https://learnopengl.com
#version 330

struct DirectionLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float intensity;
};

const int MAX_POINT_LIGHTS = 5;

uniform struct PointLight { 
	vec3 position;
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
uniform bool allowNormalMapping;
uniform bool pointLightsEnabled;
uniform bool castShadows;
uniform bool directionalLightEnabled;
uniform DirectionLight directionLight;

uniform int numberOfSpotLights;
uniform int numberOfPointLights;
uniform Material material;
uniform sampler2D textureUnit0;// diffuse
uniform sampler2D textureUnit1;// normal
uniform sampler2D textureUnit2;// height
uniform samplerCube cubeMap0;// point shadow maps, should be an array but the games lab wont allow it
uniform samplerCube cubeMap1;
uniform samplerCube cubeMap2;
uniform samplerCube cubeMap3;
uniform samplerCube cubeMap4;
// the far value of spot lights viewprojection shoud be an array or
// attached to point light str
uniform float far; 

in ShaderValues {
	vec3 position;
	vec3 normal;
	vec2 textureCoordinate;
	vec3 viewPosition;
	vec3 viewDirection;
	mat3 tangentSpaceMatrix;
	vec3 directionLightDirection;
	vec4 lightSpacePosition;
} inValues;


// taken directly from https://learnopengl.com/#!Advanced-Lighting/Shadows/Point-Shadows
const int samples = 20;
vec3 sampleOffsetDirections[samples] = vec3[] (
   vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
   vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
   vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
   vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

out vec4 colour;
// adapted from https://learnopengl.com/#!Advanced-Lighting/Shadows/Point-Shadows
float CalculatePointShadow(int activeLight) {
    vec3 toLight = (inValues.position - pointLights[activeLight].position);
    float currentDepth = length(toLight);
	float shadow = 0.0;
	float bias = 0.015;
	float viewDistance = length(inValues.viewPosition - inValues.position);
	float diskRadius = 0.005;
	for(int i = 0; i < samples; ++i) {
		float closestDepth = 0.0;
		if(activeLight == 0) closestDepth = (texture(cubeMap0, toLight + sampleOffsetDirections[i] * diskRadius).r) * pointLights[activeLight].lightLength;
		else if(activeLight == 1) closestDepth = (texture(cubeMap1, toLight + sampleOffsetDirections[i] * diskRadius).r) * pointLights[activeLight].lightLength;
		else if(activeLight == 2) closestDepth = (texture(cubeMap2, toLight + sampleOffsetDirections[i] * diskRadius).r) * pointLights[activeLight].lightLength;
		else if(activeLight == 3) closestDepth = (texture(cubeMap3, toLight + sampleOffsetDirections[i] * diskRadius).r) * pointLights[activeLight].lightLength;
		else if(activeLight == 4) closestDepth = (texture(cubeMap4, toLight + sampleOffsetDirections[i] * diskRadius).r) * pointLights[activeLight].lightLength;
		shadow += (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
	}
	shadow /= float(samples);     
    return shadow;
}

// adapted from https://learnopengl.com/#!Advanced-Lighting/Shadows/Point-Shadows
vec3 CalculatePointLighting(int activeLight, vec3 norm, float dist) {
    vec3 lightDirection = normalize(pointLights[activeLight].position - inValues.position);
	float spec = pow(max(dot(norm, normalize(lightDirection + inValues.viewDirection)), 0.0), material.shininess);
	float attenuation = (1.0 - (dist / pointLights[activeLight].lightLength)) * pointLights[activeLight].attenuation;  
    vec3 ambient = pointLights[activeLight].ambient * material.ambient.xyz;
	float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = pointLights[activeLight].diffuse * diff * material.diffuse.xyz;
    vec3 specular = pointLights[activeLight].specular * spec * material.specular.xyz;

	float light = castShadows ? 1.0 - CalculatePointShadow(activeLight) : 1.0;
	return ambient + ((diffuse + specular) * attenuation * light);
}

// adapted from https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
// this is almost a direct copy of joet de vries example from the above link.
// the only real changes are to the constant values, the for loop and the addition of a clamping operation
float CalculateDirectionalShadow(vec3 norm) {
    vec3 projCoords = (inValues.lightSpacePosition.xyz / inValues.lightSpacePosition.w) * 0.5 + 0.5;
	float shadow = 0.0;
	if(projCoords.z < 1.0) {
		float closestDepth = texture(textureUnit2, projCoords.xy).r; 
		float currentDepth = projCoords.z;  
		float bias = max(0.0005 * (1.0 - dot(norm, inValues.directionLightDirection)), 0.00005); 
		vec2 texelSize = 1.0 / textureSize(textureUnit2, 0);
		int count = 0;
		for(int x = 0; x <= 2; ++x) {
			for(int y = 0; y <= 2; ++y) {
				float pcfDepth = texture(textureUnit2, projCoords.xy + vec2(x, y) * texelSize).r; 
				shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;   
				count++;     
			}    
		}
		shadow /= count;
	}
    return clamp(shadow, 0.0, 1.0);
}

// adapted from https://learnopengl.com/#!Lighting/Light-casters
vec3 CalculateDirectionLighting(vec3 norm, float light) {	
	vec3 lightDirection = inValues.directionLightDirection;
	float diff = max(dot(norm, lightDirection), 0.0);
	float spec = pow(max(dot(inValues.viewDirection, reflect(-lightDirection, norm)), 0.0), material.shininess);
	vec3 ambient = directionLight.ambient * material.ambient.xyz;
	vec3 diffuse = directionLight.diffuse * diff * material.diffuse.xyz;
	vec3 specular = directionLight.specular * spec * material.specular.xyz;      
	return ambient + (diffuse + specular) * light * directionLight.intensity;
}

void main() {      
	vec4 tex = texture(textureUnit0, inValues.textureCoordinate);// the texture

	vec3 norm = normalize(allowNormalMapping && useNormalMap ? inValues.tangentSpaceMatrix * normalize(texture(textureUnit1, inValues.textureCoordinate).rgb * 2.0 - 1.0) : inValues.normal);
	vec3 result = vec3(0.0);
	// calculate point lighting
	for(int i = 0; i < numberOfPointLights; i++) {
		float dist = distance(pointLights[i].position, inValues.position);
		if(dist > pointLights[i].lightLength) continue;
		result += CalculatePointLighting(i, norm, dist);
	}
	if(directionalLightEnabled){// calculate directional lighting
		float light = castShadows ? 1.0 - CalculateDirectionalShadow(norm) : 1.0f;
		result += CalculateDirectionLighting(norm, light);
	}
	colour = vec4(result, 1.0) * tex; // combine result with texture
}
