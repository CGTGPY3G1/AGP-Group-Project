#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
// weights obtained from http://dev.theomader.com/gaussian-kernel-calculator/
uniform float weight[25] = float[] (0.003765,	0.015019,	0.023792,	0.015019,	0.003765,
									0.015019,	0.059912,	0.094907,	0.059912,	0.015019,
									0.023792,	0.094907,	0.150342,	0.094907,	0.023792,
									0.015019,	0.059912,	0.094907,	0.059912,	0.015019,
									0.003765,	0.015019,	0.023792,	0.015019,	0.003765);
void main() {            
    vec4 col = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 offset = (1.0 / textureSize(textureUnit0, 0));
	int count = 0;
	for(int i = -2; i <= 2; ++i) {
		for(int j = -2; j <= 2; ++j) {
			// calculate the blur, converting the 2D loop indices into 1D array indices
			col += texture(textureUnit0, UV + vec2((offset.x * i) * weight[i+2], (offset.y * j) * weight[((i+2) * 5) + j]));
			count += 1;
		}
	}
	// devide the result by the number of blur itterations
    colour = col / count;
}