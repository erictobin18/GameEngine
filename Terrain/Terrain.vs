#version 410 core

layout (location = 0) in vec4 coord;
layout (location = 2) in mat4 mvp;
out vec4 texcoord;

void main(void)
{
	texcoord = coord;
	gl_Position = mvp * vec4(coord.x,coord.y,coord.z, 1);
    //gl_Position = vec4(coord.x, coord.y, -10.0,20.0);
}