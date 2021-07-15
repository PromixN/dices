#version 460 core
out vec4 FragColor;
uniform vec3 viewPos;
in vec3 ourColor;
in vec3 temPos;
in vec3 normal;
in vec3 textPos;
// flat in int choice;
uniform samplerCube texture1;
uniform samplerCube texture2;

void main()
{
    vec3 lightPos = vec3(100, 100, 100);


    // vec3 cent;
    // if(abs(temPos.x)>=abs(temPos.y)&&abs(temPos.x)>=abs(temPos.z))
    // {
    //     if(temPos.x>0) 
    //         cent = vec3(1,0,0);
    //     else
    //         cent = vec3(-1,0,0);
    // }
    // if(abs(temPos.y)>=abs(temPos.x)&&abs(temPos.y)>=abs(temPos.z))
    // {
    //     if(temPos.y>0) 
    //         cent = vec3(0,1,0);
    //     else
    //         cent = vec3(0,-1,0);
    // }
    // if(abs(temPos.z)>=abs(temPos.y)&&abs(temPos.z)>=abs(temPos.x))
    // {
    //     if(temPos.z>0) 
    //         cent = vec3(0,0,1);
    //     else
    //         cent = vec3(0,0,-1);
    // }
    // vec3 t = cent-temPos;
    // t = normalize(t + normal*dot(normal,t));
    // vec3 b = cross(normal,t);
    // vec3 v =vec3(texture(texture1,1.05*temPos-0.05*cent)-texture(texture1,temPos));
    // float rate =sqrt(dot(v,v));
    // vec3 normal_tem = normalize(-rate*t+normal);
    vec3 normal_tem = normalize(normal);
    // vec3 normal_tem = normalize(normal);
    vec3 light = vec3(0.0);
    vec3 light_color = vec3(1.0);
    vec3 diff,spec,ambient;
    // vec3 fragPos = temPos - rate*normal;
    vec3 fragPos = temPos ;
    ambient = light_color;
    vec3 light_dir = normalize(fragPos-lightPos);
    vec3 light_ref =-dot(light_dir,normal_tem)*normal_tem*2+light_dir;

    if(dot(light_dir,normal_tem)>0.0001) spec =vec3(0);
    else spec = light_color*pow(max(0.0,dot(light_ref,normalize(viewPos-fragPos))),16);
    diff = light_color*max(0.0,dot(light_ref,normal_tem));
    // diff = vec3(0);
    light = diff*0.2+spec*0.4+ambient*0.4;

    vec3 temtextPos =textPos;
    if(abs(fragPos.x)>2.95||abs(fragPos.z)>2.95||abs(fragPos.y)>2.95)
    FragColor =(texture(texture1, temtextPos)*(vec4(0.7)+0.5*texture(texture2,temtextPos))+texture(texture2,temtextPos)*texture(texture2,temtextPos))*vec4(light,1.0);
    else FragColor = vec4(0.5,0.5,0.5,1.0);
    // FragColor =(texture(texture1, temtextPos)*vec4(light,1.0)+0.5*texture(texture2,temtextPos))*(vec4(0.7)+0.5*texture(texture2,temtextPos));
    // FragColor = mix(texture(texture1, temtextPos), texture(texture2, temtextPos), 0.2);
    // FragColor =texture(texture1,temtextPos);
    // FragColor = vec4(normal,1.0);


}