#version 460 core
out vec4 FragColor;
uniform vec3 viewPos;
in vec4 temPos;
in vec3 normal;
in vec3 textPos;
in mat4 rotates;
// flat in int choice;
uniform samplerCube texture1;
uniform samplerCube texture2;
uniform vec3 highlight;

void main()
{
    vec3 lightPos = vec3(100, 100, 100);

    // vec3 t = cent-temPos;
    // t = normalize(t + normal*dot(normal,t));
    // vec3 b = cross(normal,t);
    // vec3 v =vec3(texture(texture1,1.05*temPos-0.05*cent)-texture(texture1,temPos));
    // float rate =sqrt(dot(v,v));
    // vec3 normal_tem = normalize(-rate*t+normal);
    vec3 normal_tem = (rotates*vec4(normalize(normal),1.0)).xyz;
    // vec3 fragPos = temPos - rate*normal;
    vec3 fragPos = vec3((rotates*temPos).xyz) ;

    vec3 diff,spec,ambient;
    vec3 light = vec3(0.0);
    vec3 light_color = vec3(1.0,0.7,1.0);
    // ambient = light_color;
    ambient = vec3(1.0);
    
    vec3 light_dir = normalize(fragPos-lightPos);
    vec3 light_ref =-dot(light_dir,normal_tem)*normal_tem*2+light_dir;

    if(dot(light_dir,normal_tem)>0.0001) spec =vec3(0);
    else spec = light_color*pow(max(0.0,dot(light_ref,normalize(viewPos-fragPos))),16);
    diff = light_color*max(0.0,dot(light_ref,normal_tem));
    // diff = vec3(0);
    light = diff*0.2+spec*0.3+ambient*0.5;

    vec3 temtextPos =textPos;
    float x =abs(temPos.x);
    float y =abs(temPos.y);
    float z =abs(temPos.z);
    vec4 col_1 = texture(texture1, temtextPos);
    vec4 col_2 = texture(texture2, temtextPos);
    if(x>2.95||z>2.95||y>2.95)
    FragColor =(col_1*(vec4(0.8)+0.5*col_2)+col_2*col_2)*vec4(light+highlight,1.0);
    else 
        if(x<2.8&&z<2.8&&y<2.8)
        {
            if(dot(fragPos,fragPos)<12)
                FragColor = vec4(0.6,0.6,0.6,1.0)*vec4(light,1.0);
            else FragColor = vec4(0.8,0.8,0.8,1.0)*vec4(light,1.0);

        }
        else FragColor = vec4(0.8,0.8,0.8,1.0)*vec4(light,1.0);

    // FragColor =(texture(texture1, temtextPos)*vec4(light,1.0)+0.5*texture(texture2,temtextPos))*(vec4(0.7)+0.5*texture(texture2,temtextPos));
    // FragColor = mix(texture(texture1, temtextPos), texture(texture2, temtextPos), 0.2);
    // FragColor =texture(texture1,temtextPos);
    // FragColor = vec4(normal,1.0);

    // FragColor

}