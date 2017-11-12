#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
uniform float exposure = 0.8;

void main() {            
    const float gamma = 2.2;
    vec3 hdrColor = texture(textureUnit0, UV).rgb;
  
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
    colour = vec4(mapped, 1.0);
}