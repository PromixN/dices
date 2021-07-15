#include <glpre.h>
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
Bind_data(9,2,&(this->buffer.VAO),&(this->buffer.VBO),&(this->vertices[0]),this->vertices.size()*sizeof(Vertex),(int)sizeof(Vertex),(int)3,(int)3,(int) 2);
Bind_data(5,0,&(this->buffer.EBO),&(this->indices[0]),sizeof(int)*indices.size());
this->buffer.size=this->vertices.size();
}
void Mesh::Draw(Shader shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); 
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setFloat((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // 绘制网格
    glBindVertexArray(buffer.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}