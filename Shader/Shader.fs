#version 410 core


in vec2 tex_coordinate_out;
in vec4 testingPosition;

layout(location = 0) out vec4 fColor;

uniform sampler2D tex;

void main(void)
{
    //fColor = vec4(testingPosition.w/20,0,0,1);
    //fColor = vec4(1,0,0,1);
    fColor = texture(tex, tex_coordinate_out);
}








