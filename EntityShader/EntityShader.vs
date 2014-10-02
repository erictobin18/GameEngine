#version 410 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 texture_coordinate;
layout(location = 2) in mat4x4 modelViewPerspective;


out vec2 tex_coordinate_out;
out vec4 testingPosition;

void main(void)
{
    tex_coordinate_out = texture_coordinate;
    //testingPosition = vPosition;
    //gl_Position = vPosition;
    gl_Position = modelViewPerspective*vec4(vPosition.xyz,1);
    
}







