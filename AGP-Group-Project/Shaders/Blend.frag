#version 330 core
out vec4 colour;
in vec2 UV;
uniform sampler2D textureUnit0;
uniform sampler2D textureUnit1;


void main() {             
    colour = vec4(texture(textureUnit0, UV).rgb + texture(textureUnit1, UV).rgb, 1.0);
}