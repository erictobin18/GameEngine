#version 410 core


in vec2 tex_coordinate_out;

layout(location = 0) out vec4 fColor;

uniform sampler2D tex;

void main(void)
{
    fColor = texture(tex, tex_coordinate_out);
    //fColor = vec4(tex_coordinate_out,0.0f,1.0f);
}








