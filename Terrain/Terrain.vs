#version 410 core

layout(location = 0) in vec4 vPosition;
layout(location = 2) in mat4x4 modelViewPerspective;
layout(location = 6) in vec4 instance;


out vec2 tex_coordinate_out;
out vec4 testingPosition;
out vec4 instanceF;

void main(void)
{
    instanceF = instance;
    //testingPosition = vPosition;
    //gl_Position = vPosition;
    gl_Position = modelViewPerspective*vec4(vPosition.xyz,16);
    
}
