#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
const vec3 full = vec3(0.2101070, 0.7135946, 0.07216216);

void main() {            
    vec3 col = texture(textureUnit0, UV).rgb;
	colour = vec4(col * dot(col.rgb, full), 1.0);
}