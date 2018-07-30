#version 330 core
in vec4 fragmentPosition;

uniform vec3 lightPosition;
uniform float far;

void main(){
    gl_FragDepth = length(fragmentPosition.xyz - lightPosition) / far;
} 