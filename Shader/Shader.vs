#version 410 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 texture_coordinate;
layout(location = 2) in mat4x4 modelViewPerspective;


out vec2 tex_coordinate_out;


void main(void)
{
    tex_coordinate_out = texture_coordinate;
    gl_Position = modelViewPerspective*vPosition;
    
}







