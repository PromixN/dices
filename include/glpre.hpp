#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.hpp>
#include <Shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.hpp>
#include <glvar.hpp>
#include <gltype.hpp>
#include <geom.hpp>
#include <models.hpp>
#include <mesh.hpp>




extern GLFWwindow* win_main;
extern int window_width;
extern int window_height;
extern int view_x;
extern int view_y;

extern Camera camera_main;

extern  Shader gradual;
extern  Shader diffuss;
extern  Shader point;

extern float deltaTime ;
extern float lastFrame ;




int init_glfw(int maj_version, int min_version);

int init_window(GLFWwindow** window, int width, int height, \
	const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* shareL = NULL);

int init_glad();

int init_cube(unsigned int * VAO,unsigned int * VBO);
int init_list(cube_dsc_t* dsc);
int push_list(cube_dsc_t* dsc,cube_dsc_t* new_dsc);
void step(int axis, int pos, int cw);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int botton, int action, int mods);
unsigned int bind_model(models &model ,int times);
unsigned int bind_edge(models model,unsigned int * VAO, unsigned int * VBO, unsigned int * EBO, int times);
void Bind_static_float_element_3(
	unsigned int* VAO, unsigned int* VBO, unsigned int* EBO, float* vertices, \
	unsigned long long int vertices_size, unsigned int* indices, \
	unsigned long long int indices_size, int normalize = GL_FALSE, int offset = 0, int layout = 0);
void Bind_data(int count, ...);
void Bind_dynamic_float_vertice(
	unsigned int* VAO, unsigned int* VBO, float* vertices, \
	unsigned long long int vertices_size, int layout = 0, int interval = 3, int throughout = 3, int normalize = GL_FALSE, int offset = 0);
void load_text(unsigned int* texture, int num, const char* file_name,char is_alpha=0);
void load_cube(unsigned int* texture,int num, std::vector<std::string> file_name,char is_alpha);
void window_update();
// void rend_dynamic_model(Shader shader,models model,Camera camera);
void rend_dynamic_model(Shader& shader,models *model,Camera& camera);
void rend_magic_cube(Shader& shader,models *model,Camera& camera);
void rend_magic_cube(Shader& shader,models *model,Camera& camera,int interval);
DL_Infactor getDLInfactor(int distance);
void load_img(ImgShader* shader, char * path,int isalpha);
void init_imgfunc(ImgShader *shader);
void rend_img(ImgShader* shader);

void Bezeir(GLFWwindow* window, float* points, float* dots, 
	unsigned long long dot_size, unsigned long long points_size, 
	int count, Shader line, Shader point);