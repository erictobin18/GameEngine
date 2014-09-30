#version 410 core

in vec4 texcoord;

layout(location = 0) out vec4 fColor;

void main(void)
{
    //fColor = vec4(texcoord.w,0,0,1);
    fColor = vec4((int(texcoord.w) << 4) % 4, (int(texcoord.w) << 2) % 4, int(texcoord.w) % 4 ,1.0);
}