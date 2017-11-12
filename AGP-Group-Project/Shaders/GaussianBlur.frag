#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
uniform float weight[3] = float[] (0.2270070, 0.7845946, 0.07216216);
void main()
{            
    vec4 col = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 offset = (1.0 / textureSize(textureUnit0, 0));
	int count = 0;
	for(int i = -2; i <= 2; ++i) {
		for(int j = -2; j <= 2; ++j) {
			col += texture(textureUnit0, UV + vec2(offset.x * i, offset.y * j));
			count += 1;
		}
	}
    colour = col / count;
}