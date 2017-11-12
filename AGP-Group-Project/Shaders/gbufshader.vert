#version 330 core
layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec3 oNormal;
layout (location = 2) out vec4 oAlbedo;

in vec2 textureCoord;
in vec3 fPosition;
in vec3 normal;

uniform sampler2D diffuse;

void main() {    
    oPosition = fPosition;
    oNormal = normalize(normal);
    oAlbedo = texture(texture_diffuse1, TexCoords);
} 