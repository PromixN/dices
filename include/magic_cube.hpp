#include <glpre.hpp>

extern int cube_index;
extern int direction;
extern int to_axis;
extern int move_dir;
extern int touch_face;
extern glm::uvec3 at_pos;
extern glm::vec3 hit_pos;
extern char* ax_str  [];
extern char* dir_str [];
extern char* fa_str  [];



void rend_dot(Shader& shader,unsigned int VAO,Camera& camera);
void rend_magic_cube(Shader& shader,models *model,Camera& camera);
void rend_magic_cube(Shader& shader,models *model,Camera& camera,int interval);
int init_list(cube_dsc_t* dsc);
int push_list(cube_dsc_t* dsc,cube_dsc_t* new_dsc);
void step(int axis, int pos, int cw);
int init_list(cube_dsc_t* dsc);
int push_list(cube_dsc_t* dsc,cube_dsc_t* new_dsc);
glm::uvec3 get_index(glm::vec3 pos);
int get_axis();
int get_direction();
void choose_cube(double xpos,double ypos);
