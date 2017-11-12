#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
uniform float time; 

uniform vec2 centre = vec2(640.0, 360.0);

void main() {            
    vec2 texSize = textureSize(textureUnit0, 0);
	vec2 tc = UV * texSize;
	tc -= centre;
	float d = length(tc);
	if (d < centre.x) {
		float percent = (centre.x - d) / centre.x, theta = percent * percent * time, s = sin(theta), c = cos(theta);
		tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
	}
	tc += centre;
	vec3 col = texture2D(textureUnit0, tc / texSize).rgb;
    colour = vec4(col, 1.0);
}