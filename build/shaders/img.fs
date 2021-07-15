#version 460 core
uniform sampler2D Texture;
out vec4 FragColor;
in vec2 textPos;

void main()
{
    FragColor =texture(Texture, textPos);

}