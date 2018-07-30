#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;
const int NUMBER_OF_SIDES = 6;
uniform mat4 lightMatrices[NUMBER_OF_SIDES];

out vec4 fragmentPosition;

void main()
{
    for(int i = 0; i < NUMBER_OF_SIDES; ++i)
    {
        gl_Layer = i;
        for(int j = 0; j < 3; ++j)
        {
            fragmentPosition = gl_in[j].gl_Position;
            gl_Position = lightMatrices[i] * fragmentPosition;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  