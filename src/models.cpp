#include <models.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
float r =15;
glm::vec3 rate((r*r)/((1+r)*(1+r)),1.0*r/((1+r)*(1+r)),1/((1+r)*(1+r)));

models::models(float* vertexs,int vsize,unsigned int * faces,int fsize,unsigned int * edges, int esize)
{
    if(vsize%(3)||fsize%(4)||esize%(4)) 
    {
        std::cout<<"error input"<<std::endl;
    }
    for(int i = 0;i<vsize/(3);i++)
    {
        vertex.push_back(glm::vec3(vertexs[3*i],vertexs[3*i+1],vertexs[3*i+2]));
    }
    for(int i = 0;i<fsize/(4);i++)
    {
        face.push_back(glm::vec<4,unsigned int,glm::packed_highp>(faces[4*i],faces[4*i+1],faces[4*i+2],faces[4*i+3]));
    }
    for(int i = 0;i<esize/(4);i++)
    {
        edge.push_back(glm::vec<4,unsigned int,glm::packed_highp>(edges[4*i],edges[4*i+1],edges[4*i+2],edges[4*i+3]));
    }   
}
models::models(std::vector<glm::vec3> vers ,unsigned int * faces,int fsize,unsigned int * edges, int esize)
{

    if(fsize%(4)||esize%(4)) 
    {
        std::cout<<"error input"<<std::endl;
    }
    vertex=vers;
    for(int i = 0;i<fsize/(4);i++)
    {
        face.push_back(glm::vec<4,unsigned int,glm::packed_highp>(faces[4*i],faces[4*i+1],faces[4*i+2],faces[4*i+3]));
    }
    for(int i = 0;i<esize/(4);i++)
    {
        edge.push_back(glm::vec<4,unsigned int,glm::packed_highp>(edges[4*i],edges[4*i+1],edges[4*i+2],edges[4*i+3]));
    }   

}
models::~models()
{
}
models::models()
{}
std::vector<unsigned int> models::get_indicies()
{
    std::vector<unsigned int> vertices;
    for (auto face : this->face)
    {
        vertices.push_back(face[0]);
        vertices.push_back(face[1]);
        vertices.push_back(face[2]);
        if(face[2]==face[3]) continue;
        vertices.push_back(face[0]);
        vertices.push_back(face[2]);
        vertices.push_back(face[3]);
    }
    return vertices;
}

glm::vec3 models::find_vertex(glm::vec<4,int,glm::packed_highp> face,int num,int limit)
{
    if(limit == 3) return (glm::vec3(0.5833)*this->vertex[face[num]]+\
            glm::vec3(0.2083)*this->vertex[face[(num+1)%3]]+\
            glm::vec3(0.2083)*this->vertex[face[(num+2)%3]]);
    else    return (glm::vec3(rate[0])*this->vertex[face[num]]+\
            glm::vec3(rate[1])*this->vertex[face[(num+1)%4]]+\
            glm::vec3(rate[1])*this->vertex[face[(num+3)%4]]+\
            glm::vec3(rate[2])*this->vertex[face[(num+2)%4]]);
}
void models::devide()
{

    std::vector<new_type> vertexs;
    int vcount = 0;
    int fcount = 0;
    std::vector<glm::vec3> tvertex;
    std::vector<glm::vec<4,unsigned int,glm::packed_highp>> tface;
    std::vector<glm::vec<4,unsigned int,glm::packed_highp>> tedge;
    for(int i = 0 ; i < this->face.size();i++)
    {
        glm::vec<4,int,glm::packed_highp> face = this->face[i];
        int limit = face[2]!=face[3]? 4: 3;
        tface.push_back(glm::uvec4(vcount,vcount+1,vcount+2,vcount+(limit == 4 ?3:2)));
        tedge.push_back(glm::uvec4(vcount,vcount+1,fcount,-1));
        tedge.push_back(glm::uvec4(vcount+1,vcount+2,fcount,-1));
        tedge.push_back(glm::uvec4(vcount+2,vcount+(3%limit),fcount,-1));
        if(limit == 4) 
            tedge.push_back(glm::uvec4(vcount+3,vcount,fcount,-1));

        for(int j = 0 ; j < limit; j++)
        {
            glm::vec3 tem = this->find_vertex(face,j,limit);
            
            vertexs.push_back(new_type({tem ,face[j],face[(j+1)%limit],vcount++,i}));
            tvertex.push_back(tem);
        }

        
        fcount ++ ;
    }
    for(auto edge : this->edge)
    {
        
        glm::uvec4 tem;
        int f_1, f_2;

        for(auto vert : vertexs)
        {
            if(vert.from == edge[0]&&vert.to==edge[1])
            {
                tem[0]=vert.vertex;
                f_1 = vert.face;


            }
            if(vert.from == edge[1]&&vert.to==edge[0])
            {
                tem[2]=vert.vertex;
                f_2 = vert.face;
            }
        }

        for(auto vert: vertexs)
        {
            if(vert.from == edge[1]&&vert.face==f_1)
            {
                tem[3]=vert.vertex;
            }
            if(vert.from == edge[0]&&vert.face==f_2)
            {
                tem[1]=vert.vertex;
            }
        }

        tface.push_back(tem);
        tedge[tem[0]][3]=fcount;
        tedge[tem[3]][3]=fcount;
        tedge.push_back(glm::uvec4(tem[0],tem[1],fcount,-1));
        tedge.push_back(glm::uvec4(tem[2],tem[3],fcount,-1));

        fcount++;
    }
    for(int i = 0 ; i < this->vertex.size(); i ++)
    {
        glm::uvec4 tem;
        glm::uvec4 tem_face;
        int c = 0;
        for(auto vert : vertexs)
        {

            if(vert.from == i) 
            {

                tem[c++] = vert.vertex;
            }
        }
        for(int j = vcount; j < tedge.size(); j++)
        {
            for( int k = 0 ; k < c; k++)
            {
                if(tedge[j][0]==tem[k])
                    tedge[j][3]=fcount;
            }
        }
        
        tem_face[0]=tem[0];
        for(int k = 1; k<c ; k++)
        {
            for(int j = vcount; j < tedge.size(); j++)
            {
                if(tedge[j][1]==tem_face[k-1])
                {
                    tem_face[k]=tedge[j][0];
                    break;
                }
            }
            
        }
        if(c==3)
        tem_face[3]=tem_face[2];
        tface.push_back(tem_face);

        fcount ++;
    }
    
    this->edge= tedge;
    this->face= tface;
    this->vertex= tvertex;
    std::cout<<"DEVIDE DONE"<<std::endl;

    
}
std::vector<glm::vec3> models::get_vertices()
{
    return this->vertex;
}
std::vector<glm::vec3> models::get_normal()
{
    std::vector<glm::vec3> normal{this->vertex.size()};
    for(auto face : this->face)
    {
        bool tri = face[3]==face[2];
        glm::vec3& v0 = this->vertex[face[0]];
        glm::vec3& v1 = this->vertex[face[1]];
        glm::vec3& v2 = this->vertex[face[2]];
        glm::vec3& v3 = this->vertex[face[3]];
        glm::vec3 facet = glm::normalize(\
                          glm::cross(v2-v1,v1-v0));
        glm::vec4 angles (
            acos(glm::dot(glm::normalize(v3),glm::normalize(v0))),
            acos(glm::dot(glm::normalize(v0),glm::normalize(v1))),
            acos(glm::dot(glm::normalize(v1),glm::normalize(v2))),
            tri? 0:acos(glm::dot(glm::normalize(v2),glm::normalize(v3)))
        );

        glm::vec3 centrio = (v0+v1+v2+(tri? glm::vec3(0): v3))*glm::vec3(tri ? 0.33:0.25);


        normal[face[0]]+=(facet)*glm::vec3(angles[0]/glm::dot(centrio-v0,centrio-v0));
        normal[face[1]]+=(facet)*glm::vec3(angles[1]/glm::dot(centrio-v1,centrio-v1));
        normal[face[2]]+=(facet)*glm::vec3(angles[2]/glm::dot(centrio-v2,centrio-v2));
        if(!tri)
        normal[face[3]]+=(facet)*glm::vec3(angles[3]/glm::dot(centrio-v3,centrio-v3));
    }
    for(auto& vec:normal)
    {
        vec = glm::normalize(vec);
    }
    return normal;
}
std::vector<glm::vec3> models::get_sharp_normal()
{
    std::vector<glm::vec3> normal{this->vertex.size()};
    for(auto face : this->face)
    {
        bool tri = face[3]==face[2];
        glm::vec3& v0 = this->vertex[face[0]];
        glm::vec3& v1 = this->vertex[face[1]];

        glm::vec3 norm = glm::cross(v1,v0);

        normal[face[0]]+=norm;
        normal[face[1]]+=norm;
        normal[face[2]]+=norm;
        if(!tri)
        normal[face[3]]+=norm;
    }
    for(auto& vec:normal)
    {
        vec = glm::normalize(vec);
    }
    return normal;
}
glm::vec2 get_offset(int i,bool sig)
{
    switch (i)
    {
    case 0:
        return sig? glm::vec2(0,0.5):glm::vec2(2/3.0,0.5);
    case 2:
        return sig? glm::vec2(1/3.0,0):glm::vec2(1/3.0,0.5);
    case 1:
        return sig? glm::vec2(1/3.0,0.25):glm::vec2(1/3.0,0.75);
    default:
        break;
    }
}

glm::vec2 get_texture_pos(glm::vec3 vertext)
{

    for(int i = 0 ; i < 3 ; i ++){
        float x = fabs(vertext[(0+i)%3]);
        float y = fabs(vertext[(1+i)%3]);
        float z = fabs(vertext[(2+i)%3]);
        // std::cout<<x<<":"<<y<<":"<<z<<":"<<std::endl;
        if(x>=y&&x>=z)
        {
            return glm::vec2((vertext[(2+i)%3]/x+1.0)/6,(vertext[(1+i)%3]/x+1.0)/8)+get_offset(i,vertext[i]>0);
        }
    }
    // return glm::vec2(vertext[0],vertext[2]);
    return glm::vec2(-0.1,-0.1);

}
std::vector<glm::vec2> models::get_texture()
{
    std::vector<glm::vec2> texture;
    for(auto vert : this->vertex)
    {
        glm::vec2 tem = get_texture_pos(vert);
        // if(tem[0]<0.0001&&tem[1]<0.0001 && texture.size()>10)
        // {
        //     tem = *(texture.end()-1)+*(texture.end()-3)+*(texture.end()-5)+*(texture.end()-7);
        //     tem = tem * glm::vec2(0.25);
        // }
        texture.push_back(tem);
    }
    // for(int i = 0; i <10 && texture.size()>10 ; i ++)
    // {
    //     glm::vec2 tem = texture[i];
    //     if(tem[0]<0.0001&&tem[1]<0.0001)
    //     {
    //         tem = texture[i+3]+texture[i+5]+texture[i+7]+texture[i+9];
    //         tem = tem * glm::vec2(0.25);
    //         texture[i]=tem;
    //     }
        
    // }
    return texture;
}
std::vector<unsigned int> models::get_edges()
{
    std::vector<unsigned int> tem;
    for(auto edge :this->edge)
    {
        tem.push_back(edge[0]);
        tem.push_back(edge[1]);
    }
    return tem;
}

/*-------------------*/


bounce::bounce(std::vector<glm::vec3>  vertice, std::vector<unsigned int> modeledge)
{
    glm::mat4 trans = glm::mat4(1.0);
    trans = glm::rotate(trans,glm::radians(75.0f),glm::vec3(1.0,1.0,1.0));
    this->model_t = glm::rotate(this->model_t,glm::radians(75.0f),glm::vec3(1.0,1.0,1.0));
    this->x=trans*glm::vec4(this->x[0],this->x[1],this->x[2],1.0);
    this->y=trans*glm::vec4(this->y[0],this->y[1],this->y[2],1.0);
    this->z=trans*glm::vec4(this->z[0],this->z[1],this->z[2],1.0);
    for(auto ver : vertice)
    {
        // ver = trans*glm::vec4(ver[0],ver[1],ver[2],1.0); 
        this->vertices.push_back({ver,glm::vec3(0),new std::vector<unsigned int>,new std::vector<float>});
    }
    for(int i = 0 ; i < modeledge.size(); i += 2 )
    {
        this->vertices[modeledge[i]].edges->push_back(modeledge[i+1]);
        auto tem =this->vertices[modeledge[i]].pos-this->vertices[modeledge[i+1]].pos;
        float len = glm::dot(tem,tem);
        this->vertices[modeledge[i]].len->push_back(len);
        this->vertices[modeledge[i+1]].len->push_back(len);
        this->vertices[modeledge[i+1]].edges->push_back(modeledge[i]);
    }
}
bounce::bounce(float* vertexs,int vsize,unsigned int * faces,int fsize,unsigned int * edges, int esize)
{

    if(vsize%(3)||fsize%(4)||esize%(4)) 
    {
        std::cout<<"error input"<<std::endl;
    }
    for(int i = 0;i<vsize/(3);i++)
    {
        vertex.push_back(glm::vec3(vertexs[3*i],vertexs[3*i+1],vertexs[3*i+2]));
    }
    for(int i = 0;i<fsize/(4);i++)
    {
        face.push_back(glm::vec<4,unsigned int,glm::packed_highp>(faces[4*i],faces[4*i+1],faces[4*i+2],faces[4*i+3]));
    }
    for(int i = 0;i<esize/(4);i++)
    {
        edge.push_back(glm::vec<4,unsigned int,glm::packed_highp>(edges[4*i],edges[4*i+1],edges[4*i+2],edges[4*i+3]));
    }


}
void bounce::rotate(float angle, glm::vec3 axis)
{
    glm::mat4 trans = glm::mat4(1.0);

    trans = glm::rotate(trans,glm::radians(angle),axis);
    this->model_t = glm::rotate(this->model_t,glm::radians(angle),axis);
    this->x=trans*glm::vec4(this->x[0],this->x[1],this->x[2],1.0);
    this->y=trans*glm::vec4(this->y[0],this->y[1],this->y[2],1.0);
    this->z=trans*glm::vec4(this->z[0],this->z[1],this->z[2],1.0);
}
bounce::~bounce()
{
}
float interval= 0.016;
float k = 1000;
float ave = 0.125;
glm::vec3 g = glm::vec3(0,-9.8,0);
std::vector<glm::vec3> vlist =
{
    glm::vec3(-3.0,6.0,-3.0),
    glm::vec3(5.0,6.0,1.0),
    glm::vec3(-3.0,9.0,7.0),
    glm::vec3(3.0,3.0,4.0),
    glm::vec3(5.0,6.0,-3.0),
    glm::vec3(-3.0,8.0,4.0),
    glm::vec3(-5.0,6.0,-3.0),
    glm::vec3(-3.0,6.0,3.0)
};
std::vector<glm::vec3> bounce::get_vertices()
{
}
std::vector<glm::vec3> bounce::update()
{
    std::vector<glm::vec3> tem;
    this->p = glm::vec3(0);
    float hit_count = 0;
    glm::vec3 hp = glm::vec3(0);
    glm::vec3 tem_v = glm::vec3(0);
    for(auto& ver : this->vertices)
        this->p = this->p + ver.pos;
    this->p = this->p*glm::vec3(ave,ave,ave);

        // printf("%f:%f:%f\n",this->p[0],this->p[1],this->p[2]);
    for(auto& ver : this->vertices)
    {

        hp = glm::cross(this->p-ver.pos,this->l);
        if(glm::dot(hp,hp)>0.01)
        {
            float r = glm::dot(this->p-ver.pos,this->l);
            r = sqrt(glm::dot(this->p-ver.pos,this->p-ver.pos)-r*r);
        // printf("%f\n",r);
            hp =this->av*glm::normalize(hp)*glm::vec3(r,r,r);
            // printf("hp:%f:%f:%f\n",hp[0],hp[1],hp[2]);
        // printf("%f:%f:%f\n",ver.vel[0],ver.vel[1],ver.vel[2]);
            ver.vel=this->v+ hp ;
        // printf("%f:%f:%f\n",ver.vel[0],ver.vel[1],ver.vel[2]);
        }
        else
            ver.vel=this->v;
        ver.pos = ver.pos + ver.vel*glm::vec3(interval,interval,interval);
        tem.push_back(ver.pos);
    }
    for(auto& ver : this->vertices)
    {
        if(ver.pos[1]+4.9<0.0001)
        {
            this->hit_count+=1;
            ver.vel = ver.vel*glm::vec3(0.95,-0.95,0.95);
            for(auto e:*(ver.edges))
            {
                this->vertices[e].vel*=glm::vec3(0.95,-0.95,0.95);
            }
            ver.pos[1]=-4.899;
            this->hit = 1;
        }
        tem_v = tem_v + ver.vel;

    }
    if(this->hit){
        this->v= vlist[this->hit_count%8];
        }
    else this->v= tem_v*glm::vec3(ave,ave,ave);

    this->v= this->v+g*glm::vec3(interval,interval,interval);
    // else this->v=glm::vec3(0);
    // printf("%f:%f:%f\n",hp[0],hp[1],hp[2]);
    // hp =  hp*glm::vec3(1.0/hit_count,1.0/hit_count,1.0/hit_count);
    // printf("%f:%f:%f\n",hp[0],hp[1],hp[2]);

    // this->l= glm::vec3(0);
    float sum_r = 0;
    for(auto& ver : this->vertices)
    {
        float r = glm::dot(this->p-ver.pos,this->l);
        r = glm::dot(this->p-ver.pos,this->p-ver.pos)-r*r;
        sum_r += r;
        // this->l=this->l+glm::cross(ver.pos-this->p,ver.vel);
    }
    if(this->hit)
    {
        this->l= glm::vec3(fmod(rand(),8)- 4,fmod(rand(),8)- 4,fmod(rand(),8)- 4);
        // printf("%f:%f:%f\n",this->l[0],this->l[1],this->l[2]);
    }
    
    r = sqrt(glm::dot(this->l,this->l))/sum_r;
    this->av=glm::vec3(r*100,r*100,r*100);
    // this->av=glm::vec3(r,r,r);
    if(glm::dot(this->l,this->l)>0.01)
        this->l= glm::normalize(this->l);
    // this->l=glm::vec3(1.0,0.0,0);
        // printf("%f:%f:%f\n",this->l[0],this->l[1],this->l[2]);
        // printf("%f:%f:%f\n",this->av[0],this->av[1],this->av[2]);
        // printf("%f:%f:%f\n",this->d[0],this->d[1],this->d[2]);
    this->hit=0;
    return tem;
}


void bounce::updatet()
{
    std::vector<glm::vec3> tem;
    std::vector<glm::vec3> hitter;
    tem.push_back(this->p-this->x-this->y+this->z);
    tem.push_back(this->p-this->x-this->y-this->z);
    tem.push_back(this->p-this->x+this->y+this->z);
    tem.push_back(this->p+this->x-this->y+this->z);
    tem.push_back(this->p-this->x+this->y-this->z);
    tem.push_back(this->p+this->x-this->y-this->z);
    tem.push_back(this->p+this->x+this->y+this->z);
    tem.push_back(this->p+this->x+this->y-this->z);

    if(glm::dot(this->l,this->l)>0.01)
    {
        auto trans =glm::rotate(glm::mat4(1.0),this->av[0]*interval,this->l);
        this->model_t = trans*this->model_t;
        this->x=trans*glm::vec4(this->x[0],this->x[1],this->x[2],1.0);
        this->y=trans*glm::vec4(this->y[0],this->y[1],this->y[2],1.0);
        this->z=trans*glm::vec4(this->z[0],this->z[1],this->z[2],1.0);
    }


    for(auto ver : tem)
    {
        if(ver[1]+4.9<0.0001)
        {
            this->hit_count+=1;
            this->hit = 1;
            hitter.push_back(ver);
        }
    }
    glm::vec3 dir;
    float lowest = 0;
    // printf("%d::%d\n",this->stay,hitter.size());
    if(this->v[1]*this->v[1]<0.01||this->stay)
    {
        this->stay+=1;
        // this->v[1]=-this->v[1];
        if(this->stay==1)
        {
            // this->v[0]=this->v[2]=0;
        }
        switch (hitter.size())
        {
        case 0:
            // this->v[1]=-this->v[1];
            this->v= this->v+g*glm::vec3(interval);
            this->stay-=1;
            break;
        case 1:
            this->stay+=1;
            // this->p[1]=this->p[1]-4.9-hitter[0][1];
            dir = glm::cross(glm::vec3(0,1,0),hitter[0]);
            if(glm::dot(dir,dir)>0.0001)
            {
                this->l=glm::normalize(dir)*glm::vec3(-1,-1,-1);
                dir = glm::normalize(glm::cross(glm::normalize(dir),hitter[0]));
                float cosa = glm::dot(glm::normalize(hitter[0]),glm::vec3(0,-1,0));
                float g_val = 9.8*(sqrt(1-cosa*cosa));
                // glm::vec3 rot = glm::vec3
                auto g_vec =glm::vec3(g_val)*dir;
                this->sv=this->sv+g_vec*glm::vec3(interval);
                this->sv=glm::vec3(sqrt(glm::dot(this->sv,this->sv)))*dir;
                float b_val = sqrt(glm::dot(this->sv,this->sv));
                this->av[0]=(b_val)/sqrt(3);
                // float n = glm::dot(glm::normalize(hitter[0]),)   
            // printf("%f:%f:%f:\n",g_vec[0],g_vec[1],g_vec[2]);
            // printf("%f:%f:%f:\n",this->v[0],this->v[1],this->v[2]);
            }
            break;
        case 2:
            this->stay+=1;
            // this->p[1]=this->p[1]-4.9-(hitter[0][1]>hitter[1][1] ? hitter[1][1]:hitter[0][1]);
            dir = glm::cross(glm::vec3(0,1,0),hitter[0]+hitter[1]);
            if(glm::dot(dir,dir)>0.0001)
            {
                this->l=glm::normalize(dir)*glm::vec3(-1,-1,-1);
                dir = glm::normalize(glm::cross(glm::normalize(dir),hitter[0]+hitter[1]));
                float cosa = glm::dot(glm::normalize(hitter[0]+hitter[1]),glm::vec3(0,-1,0));
                float g_val = 9.8*(sqrt(1-cosa*cosa));
                // glm::vec3 rot = glm::vec3
                auto g_vec =glm::vec3(g_val)*dir;
                // auto g_vec =glm::vec3(g_val,g_val,g_val)*dir*glm::vec3(-1,-1,-1);
                this->sv=this->sv+g_vec*glm::vec3(interval);
                this->sv=glm::vec3(sqrt(glm::dot(this->sv,this->sv)))*dir;
                float b_val = sqrt(glm::dot(this->sv,this->sv));
                this->av[0]=(b_val)/sqrt(2);
                // float n = glm::dot(glm::normalize(hitter[0]),)   
            // printf("%f:%f:%f:\n",g_vec[0],g_vec[1],g_vec[2]);
            // printf("%f:%f:%f:\n",this->v[0],this->v[1],this->v[2]);
            }
            break;
        case 3:
        case 4:
            // for(auto ver : hitter)
            // {
            //     if(ver[1]<lowest)
            //         lowest=ver[1];
            // }
            // this->p[1]+=-4.9-lowest;
            this->stay+=1;
            this->l=glm::vec3(0);
            this->av[0]=0;
            this->v=glm::vec3(0);
            this->sv=glm::vec3(0);
        default:
            break;
        }
        if(this->stay>4) this->stay=4;

            // printf("%f:%f:%f:\n",this->v[0],this->v[1],this->v[2]);
    }
    else
    {
        this->sv=glm::vec3(0);
        switch (hitter.size())
        {
        case 0:
            break;
        case 1:
            this->p[1]=this->p[1]-4.9-hitter[0][1];
            dir = glm::cross(hitter[0],glm::vec3(0,1,0));
            if(glm::dot(dir,dir)>0.01)
            {
                float cosa = glm::dot(glm::normalize(hitter[0]),glm::vec3(0,-1,0));
                // glm::vec3 rot = glm::vec3
                float offset = glm::dot(this->l,hitter[0]);
                auto v_r = hitter[0]-glm::vec3(offset)*this->l;
                float cosb = glm::dot(glm::normalize(v_r),glm::vec3(0,-1,0));

                float rot = this->av[0]*sqrt(glm::dot(v_r,v_r));
                float y_v = rot*sqrt(1-cosb*cosb);
                // printf("%f\n",y_v);
                this->l=glm::normalize(dir);
                this->l=this->l*this->av;
                offset = glm::dot(this->l,hitter[0]);
                v_r = hitter[0]-glm::vec3(offset)*this->l;
                this->av[0]=(-this->v[1]+y_v)*sqrt(1-cosa*cosa)/sqrt(glm::dot(v_r,v_r));
                // this->l=glm::vec3(this->av[0],this->av[0],this->av[0])*glm::normalize(dir)+this->l;
                // this->av[0] =sqrt(glm::dot(this->l,this->l));
                // float n = glm::dot(glm::normalize(hitter[0]),)   
            }
            this->v[1]=-0.5*this->v[1];
            // printf("%f:%f:%f::::\n",hitter[0][0],hitter[0][2],hitter[0][2]);
            // printf("%f:%f:%f:\n",this->l[0],this->l[1],this->l[2]);
            break;
        case 2:
            this->p[1]=this->p[1]-4.9-(hitter[0][1]>hitter[1][1] ? hitter[1][1]:hitter[0][1]);
            dir = glm::cross(hitter[0]+hitter[1],glm::vec3(0,1,0));
            if(glm::dot(dir,dir)>0.01)
            {
                float cosa = glm::dot(glm::normalize(hitter[0]+hitter[1]),glm::vec3(0,-1,0));
                // glm::vec3 rot = glm::vec3
                float offset = glm::dot(this->l,hitter[0]);
                auto v_r = (hitter[0]+hitter[1])*glm::vec3(0.5,0.5,0.5)-glm::vec3(offset)*this->l;
                float cosb = glm::dot(glm::normalize(v_r),glm::vec3(0,-1,0));

                float rot = this->av[0]*sqrt(glm::dot(v_r,v_r));
                float y_v = rot*sqrt(1-cosb*cosb);
                offset = glm::dot(this->l,hitter[1]);
                v_r = hitter[1]-glm::vec3(offset)*this->l;
                cosb = glm::dot(glm::normalize(v_r),glm::vec3(0,-1,0));
                rot = this->av[0]*sqrt(glm::dot(v_r,v_r));
                y_v += rot*sqrt(1-cosb*cosb);
                this->l=glm::normalize(dir);
                offset = glm::dot(this->l,(hitter[0]+hitter[1])*glm::vec3(0.5));
                v_r = (hitter[0]+hitter[1])*glm::vec3(0.5,0.5,0.5)-glm::vec3(offset)*this->l;
                this->av[0]=(-this->v[1]+y_v*0.5)*sqrt(1-cosa*cosa)/sqrt(glm::dot(v_r,v_r));
                // float n = glm::dot(glm::normalize(hitter[0]),)   
            }
            this->v[1]=-0.7*this->v[1];
            break;
        case 3:
        case 4:
            for(auto ver : hitter)
            {
                if(ver[1]<lowest)
                    lowest=ver[1];
            }
            this->p[1]+=-4.9-lowest;
            this->l=glm::vec3(0);
            this->av[0]=0;
            this->v=glm::vec3(0.95,-0.7,0.95)*this->v;
        default:
            break;
        }
        this->v= this->v+g*glm::vec3(interval);
    }
    // if(this->hit)
    // {
    // this->v= vlist[this->hit_count%8];
    // }

    this->p=this->p+(this->v+this->sv)*glm::vec3(interval);
    // if(this->hit)
    // {
    //     this->l= glm::vec3(fmod(rand(),8)- 4,fmod(rand(),8)- 4,fmod(rand(),8)- 4);
    //     // printf("%f:%f:%f\n",this->l[0],this->l[1],this->l[2]);
    //       float sum_r = 0;
    //     for(auto ver : tem)
    //     {
    //         float r = glm::dot(this->p-ver,this->l);
    //         r = glm::dot(this->p-ver,this->p-ver)-r*r;
    //         sum_r += r;
    //     }
    //     r = sqrt(glm::dot(this->l,this->l))/sum_r;
    //     // this->av=glm::vec3(r*100,r*100,r*100);
    //     this->av=glm::vec3(r,r,r);
    // }
    // if(glm::dot(this->l,this->l)>0.01)
    // this->l= glm::normalize(this->l);
    // this->hit=0;
    this->av=glm::vec3(this->av[0]);
    return ;
}
