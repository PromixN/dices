#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexture;
uniform mat4 model;
uniform mat4 rotate;
uniform mat4 view;
uniform mat4 projection;
out vec4 temPos;
out vec3 normal;
out vec3 textPos;
out mat4 rotates;
// flat out int choice;

void main()
{

    temPos =model*vec4(aPos,1.0);
    textPos = aPos;

    rotates = rotate;

    normal = vec3(mat4(model[0],model[1],model[2],vec4(0,0,0,1.0))*vec4(aNormal,1.0));
    gl_Position = projection*view*rotate*model*vec4(aPos,1.0);


}