#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <gltype.hpp>

typedef struct new_vertex
{
    glm::vec3 pos;
    int from;
    int to  ;
    int vertex;
    int face;
} new_type;



class models
{
protected:
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec<4,unsigned int,glm::packed_highp>> face;
    std::vector<glm::vec<4,unsigned int,glm::packed_highp>> edge;
public:
    models(std::vector<glm::vec3> vers ,unsigned int * faces,int fsize,unsigned int * edges, int esize);
    models(float* vertexs,int vsize,unsigned int  * faces,int fsize,unsigned int * edges, int esize);
    models();
    ~models();
    std::vector<unsigned int> get_indicies();
    glm::vec3 find_vertex(glm::vec<4,int,glm::packed_highp> face,int num,int limit);
    void devide();
    std::vector<glm::vec3> get_vertices();
    std::vector<glm::vec3> get_normal();
    std::vector<glm::vec2> get_texture();
    std::vector<unsigned int> get_edges();
    virtual void updatet()
    {
    }
    graphBuffers buffer;
    glm::vec3 p = glm::vec3(0);
    glm::mat4 model_t = glm::mat4(1.0);
    glm::vec3 scale_t = glm::vec3(1.0);
};

typedef struct vertex_mt
{
    glm::vec3 pos;
    glm::vec3 vel;
    std::vector<unsigned int> *edges;
    std::vector<float> *len;

} Vertexm;

typedef struct edge_t
{
    unsigned int e1;
    unsigned int e2;
} Edge;

class bounce : public  models
{
private:
    std::vector<Vertexm> vertices;
    std::vector<Edge> edges;
    glm::vec3 l = glm::vec3(0.0,0.5,1);
    // glm::vec3 l = glm::vec3(0.0,0.0,0.0);
    glm::vec3 d = glm::vec3(0);
    glm::vec3 v = glm::vec3(0.0);
    glm::vec3 av = glm::vec3(5.0,1.0,1.0);
    glm::vec3 x = glm::vec3(1.0,0,0);
    glm::vec3 y = glm::vec3(0,1.0,0);
    glm::vec3 z = glm::vec3(0,0,-1.0);
    glm::vec3 sv = glm::vec3(0,0,0);
    int hit = 0;
    int stay = 0;
    int hit_count = 0;

public:
    bounce(std::vector<glm::vec3>  vertices, std::vector<unsigned int> modeledge);
    bounce(float* vertexs,int vsize,unsigned int * faces,int fsize,unsigned int * edges, int esize);
    ~bounce();
    std::vector<glm::vec3>get_vertices();
    std::vector<glm::vec3>update();
    void updatet() override;
};
