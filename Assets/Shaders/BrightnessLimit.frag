#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
// colour limit			Red		   Green	  Blue
const vec3 limit = vec3(0.401070, 0.7135946, 0.3216216);

void main() {       
	// get the textures x, y and z components
    vec3 col = texture(textureUnit0, UV).rgb;
	// project the colour onto the limit (clamping the result between 0 and 1.0), 
	// then scale the colour by this value, this will darken colours below the limit
	colour = vec4(col * clamp(dot(col.rgb, limit), 0.0, 1.0), 1.0);
}