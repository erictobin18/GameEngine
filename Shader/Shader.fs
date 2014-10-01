#version 410 core


in vec2 tex_coordinate_out;
in vec4 testingPosition;
in vec4 instanceF;

layout(location = 0) out vec4 fColor;

uniform sampler2D tex;

void main(void)
{
    //fColor = vec4(1,0,0,1);
    //fColor = vec4(instanceF.a/48.0f,instanceF.a/48.0f,instanceF.a/48.0f,1);
    fColor = texture(tex, tex_coordinate_out);
    //fColor = vec4((int(instanceF.a)/64)/3.0f,((int(instanceF.a)/16)%4)/3.0f,((int(instanceF.a)/4)%4)/3.0f,(int(instanceF.a)%4)/3.0f);
}








