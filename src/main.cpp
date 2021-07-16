#include <glpre.hpp>
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
    Shader cube_shader("./shaders/cubes.vs", "./shaders/cubes.fs");
    Shader edge("./shaders/edge.vs", "./shaders/edge.fs");
    ImgShader imgshader("./shaders/img.vs","./shaders/img.fs");
    init_imgfunc(&imgshader);
//model
    bounce cube(cube_vertices,CV_NUM,cube_faces,CF_NUM,cube_edges,CE_NUM);
    bind_model(cube, 4);
    models plane(plane_vertices,PV_NUM,plane_faces,PF_NUM,NULL,0);
    int plane_size = bind_model(plane,0);
    plane.p=glm::vec3(0,-3.9,0);
    plane.scale_t=glm::vec3(200.0,1.0, 200.0);
    init_list(cube_dsc_head);
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
    std::vector<std::string> noise_files = 
    {
        "./texture/noise_1.jpg",
        "./texture/noise_1.jpg",
        "./texture/noise_1.jpg",
        "./texture/noise_1.jpg",
        "./texture/noise_1.jpg",
        "./texture/noise_1.jpg"
    };
    std::vector<std::string> magic_cube_files = 
    {
        "./texture/red.png",
        "./texture/orange.png",
        "./texture/green.png",
        "./texture/blue.png",
        "./texture/yellow.png",
        "./texture/white.png"
    };
    unsigned int texture1,texture2,texture3;
    dice.use();
	load_text(&texture2, 2, "./texture/awesomeface.png",1);
    load_cube(&texture1, 3, files,1);
    load_cube(&texture3, 0, magic_cube_files,1);
    load_cube(&texture1, 1, noise_files,0);

    // load_img(&imgshader,"./texture/a.jpg",0);
    // load_img(&imgshader,"./texture/blue.png",1);
    // load_img(&imgshader,"./texture/noise_1.jpg",0);
//GL configuration
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_CULL_FACE);
    glLineWidth(5);
    // glFrontFace(GL_CW);
    glCullFace(GL_FRONT);

    lastFrame = glfwGetTime();
    camera_main.MoveSpeed=10.0f;
    dice.setBool("col",true);
    int count = 0;
    while (!glfwWindowShouldClose(win_main))
    {
        // input
        // -----
        window_update();
        if(!(count%INTERVAL_RTT))
        step(0,2,0);
        // render
        // ------
        rend_dynamic_model(dice,&plane,camera_main);
        // rend_dynamic_model(dice,&cube,camera_main);
        rend_magic_cube(cube_shader,&cube,camera_main,(count++%INTERVAL_RTT));
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
