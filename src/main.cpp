#include <glpre.h>
#include <iostream>

int main()
{
//config
    std::ofstream of("./out.txt");
    // 获取文件out.txt流缓冲区指针
     std::streambuf* fileBuf = of.rdbuf();
    // 设置cout流缓冲区指针为out.txt的流缓冲区指针
    //  std::cout.rdbuf(fileBuf);
    srand(time(NULL));
//initialization
    init_glfw(4,6);
    init_window(&win_main,WINDOW_WIDTH,WINDOW_HEIGHT,"promixs");
	glfwMakeContextCurrent(win_main);
    init_glad();
	glfwSetFramebufferSizeCallback(win_main, framebuffer_size_callback);
	glfwSetInputMode(win_main, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// glfwSetInputMode(win_main, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(win_main, mouse_callback);
    glfwSetMouseButtonCallback(win_main,mouse_button_callback);
	glfwSetScrollCallback(win_main, scroll_callback);
//shader
    Shader dice("./shaders/dice.vs", "./shaders/dice.fs");
    Shader edge("./shaders/edge.vs", "./shaders/edge.fs");
    ImgShader imgshader("./shaders/img.vs","./shaders/img.fs");
    init_imgfunc(&imgshader);
//model
    bounce cube(cube_vertices,CV_NUM,cube_faces,CF_NUM,cube_edges,CE_NUM);
    bind_model(cube, 3);
    models plane(plane_vertices,PV_NUM,plane_faces,PF_NUM,NULL,0);
    int plane_size = bind_model(plane,0);
    plane.p=glm::vec3(0,-3.9,0);
    plane.scale_t=glm::vec3(200.0,1.0, 200.0);

//texture
    std::vector<std::string> files = 
    {
        "./texture/1.png",
        "./texture/2.png",
        "./texture/3.png",
        "./texture/4.png",
        "./texture/5.png",
        "./texture/6.png"
    };
    unsigned int texture1,texture2;
    dice.use();
	load_cube(&texture1, texture_num, files,1);
    dice.setInt("Texture1", texture1);
	load_text(&texture2, texture_num, "./texture/awesomeface.png",1);
	dice.setInt("Texture2", texture2);

    load_img(&imgshader,"./texture/a.jpg",0);
//GL configuration
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glLineWidth(5);

    lastFrame = glfwGetTime();
    camera_main.MoveSpeed=10.0f;
    dice.setBool("col",true);
    while (!glfwWindowShouldClose(win_main))
    {
        // input
        // -----
        window_update();
        // render
        // ------
        rend_dynamic_model(dice,&plane,camera_main);
        rend_dynamic_model(dice,&cube,camera_main);
        rend_img(&imgshader);
        //refresh
        glfwSwapBuffers(win_main);
        glfwPollEvents();
    }
    of.flush();
    of.close();
    glfwTerminate();
    return 0;
}
