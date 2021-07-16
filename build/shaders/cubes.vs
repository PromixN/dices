#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexture;
uniform mat4 model;
uniform mat4 rotate;
uniform mat4 view;
uniform mat4 projection;
uniform bool col;
out vec3 ourColor;
out vec3 temPos;
out vec3 normal;
out vec3 textPos;
// flat out int choice;

void main()
{
    vec3 Pos;
    if(col) Pos = aPos;
    else Pos = 0.98*aPos;

    temPos =vec3(model*vec4(Pos,1.0));
    textPos = aPos;

    if (col)ourColor = vec3(aPos.x+1,aPos.y+1.0,aPos.z+1.0)*0.5;
    else ourColor = vec3(0.5,0.5,0.5);

    normal = vec3(mat4(model[0],model[1],model[2],vec4(0,0,0,1.0))*vec4(aNormal,1.0));
    gl_Position = projection*view*rotate*model*vec4(Pos,1.0);


}