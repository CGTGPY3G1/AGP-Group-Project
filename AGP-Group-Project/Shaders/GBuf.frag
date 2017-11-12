#version 330
layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 5) out vec4 diffuse;
uniform sampler2D textureUnit0;
in ShaderValues {
	vec3 nposition;
	vec3 nnormal;
	vec2 textureCoordinate;
}inData;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    position = inData.nposition;
    // also store the per-fragment normals into the gbuffer
    normal = inData.nnormal;
    // and the diffuse per-fragment color
    diffuse = vec4(texture(textureUnit0, inData.textureCoordinate).rgb, 1.0);
    // store specular intensity in gAlbedoSpec's alpha component
    //diffuse.a = texture(texture_specular1, ShaderValues.textureCoordinate).r;
}
