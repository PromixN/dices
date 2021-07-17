#include <ctime>
#include <glpre.hpp>
#include <magic_cube.hpp>
#include <iostream>

int main()
{
//config
	std::ofstream of("./out.txt");
    std::streambuf* fileBuf = of.rdbuf();
    //  std::cout.rdbuf(fileBuf);
    srand(time(NULL));
//initialization
    init_glfw(4,6);
    init_window(&win_main,WINDOW_WIDTH,WINDOW_HEIGHT,"promixs");
	glfwMakeContextCurrent(win_main);
    init_glad();
	glfwSetFramebufferSizeCallback(win_main, framebuffer_size_callback);
	framebuffer_size_callback(win_main,WINDOW_WIDTH,WINDOW_HEIGHT);
	glfwSetInputMode(win_main, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// glfwSetInputMode(win_main, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(win_main, mouse_callback);
    glfwSetMouseButtonCallback(win_main,mouse_button_callback);
	glfwSetScrollCallback(win_main, scroll_callback);

	init_dir(shader_dir,texture_dir);
//shader
    Shader dice(std::string("dice.vs"), std::string("dice.fs"));
    Shader cube_shader(std::string("cubes.vs"), std::string("cubes.fs"));
    Shader edge(std::string("edge.vs"), std::string("edge.fs"));
    ImgShader imgshader(std::string("img.vs"),std::string("img.fs"));
    init_imgfunc(&imgshader);
//model
    unsigned int VAO,VBO;//REND DOT
    float vertices []=
	{
	1,0,0,
    0,1,0,
    0,0,1,
	};
	Bind_data((int)7,(char)2,&(VAO),&(VBO),&(vertices[0]),sizeof(vertices),(int)3, (int)3);
	
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
        std::string("1.png"),
        std::string("2.png"),
        std::string("3.png"),
        std::string("4.png"),
        std::string("5.png"),
        std::string("6.png")
    };
    std::vector<std::string> noise_files = 
    {
        std::string("noise_1.jpg"),
        std::string("noise_1.jpg"),
        std::string("noise_1.jpg"),
        std::string("noise_1.jpg"),
        std::string("noise_1.jpg"),
        std::string("noise_1.jpg")
    };
    std::vector<std::string> magic_cube_files = 
    {
        std::string("red.png"),
        std::string("orange.png"),
        std::string("green.png"),
        std::string("blue.png"),
        std::string("yellow.png"),
        std::string("white.png")
    };
    unsigned int texture1,texture2,texture3;
    dice.use();

    //TODO:: bind texture unit wite shader
	load_text(&texture2, 2, std::string("favicon.png"),1);
    load_cube(&texture1, 3, files,1);
    load_cube(&texture3, 0, magic_cube_files,1);
    load_cube(&texture1, 1, noise_files,0);

    // load_img(&imgshader,std::string("/a.jpg"),0);
    load_img(&imgshader,std::string("blue.png"),1);
    // load_img(&imgshader,std::string("/noise_1.jpg"),0);
//GL configuration
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // glEnable(GL_CULL_FACE);
    glLineWidth(5);
    glPointSize(50);
    // glFrontFace(GL_CW);
    glCullFace(GL_FRONT);

    lastFrame = glfwGetTime();
    camera_main.MoveSpeed=10.0f;
    dice.setBool("col",true);
    frame_count = INTERVAL_RTT;
    frame_move = 0;
    while (!glfwWindowShouldClose(win_main))
    {
        // input
        // -----
        window_update();
        frame_count+=frame_move;
        // render
        // ------
        rend_dynamic_model(dice,&plane,camera_main);
        // rend_dynamic_model(dice,&cube,camera_main);
        rend_magic_cube(cube_shader,&cube,camera_main,(frame_count));
        // rend_img(&imgshader);
        // rend_dot(edge,VAO,camera_main);
        //refresh
        glfwSwapBuffers(win_main);
        glfwPollEvents();
    }
    of.flush();
    of.close();
    glfwTerminate();
    return 0;
}
