#ifndef GLVAR
#define GLVAR
    #include <map>
    #include <gltype.hpp>


    #define WINDOW_WIDTH	1800
    #define WINDOW_HEIGHT	1000
    #define VIEW_WIDTH		1000
    #define VIEW_HEIGHT		1000
    #define PI				3.1415627

    #define N				 0.45
    #define F				 3.0
    #define L				-0.8
    #define R				 0.8
    // #define T				 0.8
    #define B				-0.8

    #define CUBE_NUM		216
    #define CV_NUM		    24
    #define CF_NUM		    24
    #define CE_NUM		    48
    #define PV_NUM		    12
    #define PF_NUM		    4

    extern int texture_num;
    extern float cube_box[CUBE_NUM] ;

    extern std::map<int, DL_Infactor> DLI_map;
    extern float cube_vertices [CV_NUM];
    extern unsigned int cube_faces[CF_NUM]; 
    extern unsigned int cube_edges [CE_NUM];
    extern float plane_vertices [PV_NUM];
    extern unsigned int plane_faces[PF_NUM];
#endif