#ifndef GLTYPES
    #define GLTYPES
    #include <glm/glm.hpp>
    #include <string>
    #include <vector>
    typedef struct graph_buffers
    {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int size;
    } graphBuffers;

    typedef struct dot_light_infactor
    {
        float cons;
        float linear;
        float quad;
    } DL_Infactor;
    typedef struct img_desc
    {
        unsigned texture;
        float offset_x;
        float offset_y;
        float scale_x;
        float scale_y;
    }imgDesc;
    typedef struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    } vertex_t;
    typedef struct Texture {
        unsigned int id;
        std::string type;
    }texture_t;
    typedef struct cube_dsct
    {
        std::vector<glm::vec3> pos;
        std::vector<glm::mat4> rotate;
    }cube_dsc_t;
#endif