#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
uniform float offset_x;
uniform float offset_y;
uniform float scale_x;
uniform float scale_y;
out vec2 textPos;

void main()
{
    gl_Position=vec4(aPos.x*scale_x+offset_x,aPos.y*scale_y+offset_y,aPos.z,1.0);
    textPos = aTexture;
}
