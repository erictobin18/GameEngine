#version 410 core

in vec4 testingPosition;
in vec4 instanceF;

layout(location = 0) out vec4 fColor;

uniform sampler2D tex;

void main(void)
{
    //fColor = vec4(1,0,0,1);
    //fColor = vec4(instanceF.a/48.0f,instanceF.a/48.0f,instanceF.a/48.0f,1);
    //int temp = int(instanceF.a);
    
    fColor = vec4(instanceF.a/128.0,instanceF.a/256.0,instanceF.a/512.0,1);
}








