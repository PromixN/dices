#define STB_IMAGE_IMPLEMENTATION
#include <glpre.hpp>
#include <magic_cube.hpp>
#include <cstdarg>

#include <typeinfo>
GLFWwindow* win_main;
int window_width;
int window_height;
int view_x;
int view_y;

int frame_count;
int frame_move;

int     move_dir;                   

Camera camera_main;

Shader gradual;
Shader diffuss;
Shader point;

float deltaTime ;
float lastFrame ;

int firstMouse = 1;
bool left_down = false;
bool right_down = false;

float lastX =0.0f;
float lastY =0.0f;

int init_glfw(int maj_version, int min_version)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, maj_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	return 0;
}

int init_window(GLFWwindow** window, int width, int height, \
	const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
	deltaTime = lastFrame = 0.0f ;
	*window = glfwCreateWindow(width, height, title, monitor, share);
	if (*window == NULL)
	{
		std::cout << "Failed to create a new GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	return 0;
}

int init_glad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return 0;
}

int init_cube(unsigned int * VAO,unsigned int * VBO)
{
	Bind_data((int)8, (char)2, VAO, VBO, cube_box, sizeof(float)*CUBE_NUM, (int)6, (int)3, (int) 3);
	return 0;		
}
void processInput(GLFWwindow* window)
{
	char direction=0;
	float moveTime = deltaTime * 2.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction= direction | move_mask[0];
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction= direction | move_mask[1];
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction= direction | move_mask[2];
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction= direction | move_mask[3];
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) direction= direction | move_mask[4];
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) direction= direction | move_mask[5];
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) camera_main.pause=!camera_main.pause;
    if(direction)  camera_main.ProcessKeyboard(direction,moveTime);
}
void mouse_button_callback(GLFWwindow* window, int botton, int action, int mods)
{
	bool res = action==GLFW_PRESS;
	switch (botton)
	{
	case GLFW_MOUSE_BUTTON_LEFT :
		left_down = res;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		right_down = res;
		if(!res&&frame_count==INTERVAL_RTT)
		{
			get_axis_direction(to_axis,rotate_direction);
			printf("%d:%d:%d\n",at_pos.x,at_pos.y,at_pos.z);
			printf("%s:%s:%s\n",fa_str[touch_face],ax_str[to_axis],dir_str[rotate_direction]);
			step(to_axis,at_pos[to_axis],rotate_direction);
		}
		break;
	default:	
		break;
	}
	if(!res) firstMouse = true;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(left_down)
	{
		if(firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; 
		lastX = xpos;
		lastY = ypos;

		/*if(abs(xoffset)>FLT_EPSILON||abs(yoffset)>FLT_EPSILON)*/
		camera_main.ProcessMouseMovement(xoffset,yoffset);
	}else 
	if(right_down)
	{
		// std::cout<<xpos<<"::"<<ypos<<std::endl;
		if(hight_index==-1)
		{
			choose_cube(xpos,ypos);
		}
		else
		{
			if(firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}
			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; 
			auto ur = glm::dot(glm::vec2(xoffset,yoffset),glm::vec2(1.0,-1.0));
			auto ul = glm::dot(glm::vec2(xoffset,yoffset),glm::vec2(1.0,1.0));
			move_dir = (ur>0? 2:0)+(ul>0? 1:0);
			// printf("%f:%f\n",xoffset,yoffset);
			// printf("%s\n",dir_str[move_dir]);
		}

	}
	else
	{
		hight_index=-1;
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera_main.ProcessMouseScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	int tem = width > height ? height : width;
	window_width = tem;
	window_height = tem;
	view_x = width > height ? (width - height) / 2 : 0;
	view_y = width < height ? (height - width) / 2 : 0;
	glViewport(view_x, view_y, tem, tem);
	std::cout << width << "::" << height << std::endl;
}

void window_update()
{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		processInput(win_main);
		camera_main.UpdataCamera();
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rend_dynamic_model(Shader& shader,models* model,Camera& camera)
{
	shader.use();
	glm::mat4 view       = camera.view;
	glm::mat4 projection = glm::perspective(radians(camera.Angle), camera.Rate, 0.1f, 10000.0f);
	glm::mat4 model_t= glm::mat4(1.0f);
	// if(!camera.pause) 
		model->updatet();
	model_t = glm::translate(model_t,model->p)*model->model_t;
	model_t = glm::scale(model_t,model->scale_t);
    // glActiveTexture(GL_TEXTURE0+0);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, 2);
	// glActiveTexture(GL_TEXTURE0+1);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, 2);
	shader.setInt("texture1",3);
	shader.setInt("texture2",3);
	shader.setMat4("model",model_t);
	shader.setMat4("view",view);
	shader.setVec3("viewPos",camera.Position);
	shader.setMat4("projection",projection);
	glBindVertexArray(model->buffer.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->buffer.EBO);
	glDrawElements(GL_TRIANGLES, model->buffer.size, GL_UNSIGNED_INT, 0);
}
void Bind_static_float_element_3(
	unsigned int* VAO, unsigned int* VBO, unsigned int* EBO, float* vertices, \
	unsigned long long int vertices_size, unsigned int* indices, \
	unsigned long long int indices_size, int normalize, int offset, int layout)
{
	glGenBuffers(1, EBO);
	glGenBuffers(1, VBO);
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(layout, 3, GL_FLOAT, normalize, 3 * sizeof(float), (void*)offset);
	glEnableVertexAttribArray(layout);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Bind_dynamic_float_vertice(
	unsigned int* VAO, unsigned int* VBO, float* vertices, \
	unsigned long long int vertices_size, int layout, int interval, \
	int throughput, int normalize, int offset)
{
	glGenBuffers(1, VBO);
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(layout, throughput, GL_FLOAT, GL_FALSE, interval * sizeof(float), (void*)offset);
	glEnableVertexAttribArray(layout);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
// 0:ELEMENT static  1: ELEMENT dynamic  2:float static 3: float dynamic >=4 : GL_TRUE
// VAO/VBO/EBO buffer sizeofbuffer stride throughput
void Bind_data(int count, ...)
{
	va_list argv;
	int index =0;
	va_start(argv, count);
	char mode = va_arg(argv, int);		//0: ELEMENT 
	index++;
	char normalize=0;
	int offset = 0;
	int layout = 0;
	if(mode >=4) 
	{
		mode -= 4;
		normalize = 1;
	}
	if (mode<2)
	{
		unsigned int*			EBO				= va_arg(argv, unsigned int*);
		unsigned int*			indices			= va_arg(argv, unsigned int*);
		unsigned long long int	indices_size	= va_arg(argv, unsigned long long int);

		glGenBuffers(1, EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, mode % 2 ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		unsigned int*			VAO				= va_arg(argv, unsigned int*);
		unsigned int*			VBO				= va_arg(argv, unsigned int*);
		float*					vertices		= va_arg(argv, float*);
		unsigned long long int	vertices_size	= va_arg(argv, unsigned long long int);
		int						interval		= va_arg(argv, int);
		int						throughput;
		index += 5;
		glGenBuffers(1, VBO);
		glGenVertexArrays(1, VAO);
		
		glBindVertexArray(*VAO);
		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, mode % 2 ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		while (index < count)
		{
			throughput = va_arg(argv, int);
			index++;
			glVertexAttribPointer(layout, throughput, GL_FLOAT, GL_FALSE, interval * sizeof(float), (void*)(offset * sizeof(float)));
			glEnableVertexAttribArray(layout);

			offset += throughput;
			layout++;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	va_end(argv);
	return;
}

void load_text(unsigned int* texture,int num, const char* file_name,char is_alpha)
{
	stbi_set_flip_vertically_on_load(false);
	glGenTextures(1, texture);
	glActiveTexture(GL_TEXTURE0+num);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChnnels;
	unsigned char* data = stbi_load(file_name, &width, &height, &nrChnnels, 0);

	if (data) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB+is_alpha, width, height, 0, GL_RGB+is_alpha, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
void load_cube(unsigned int* texture,int num,std::vector<std::string> file_name,char is_alpha)
{
	stbi_set_flip_vertically_on_load(false);
	glGenTextures(1, texture); //生成材质id
    glActiveTexture(GL_TEXTURE0+num);

    glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);//绑定材质ID 与立方体贴图对象进行关联

	// glCreateTextures(1,GL_TEXTURE_CUBE_MAP,texture);
	// glActiveTexture(GL_TEXTURE0+num);

	// glTexStorage2D(*texture,10,GL_RGB+is_alphaA8,1024,1024);


	int width, height, nrChnnels,count=0;
	for(auto file : file_name)
	{
		unsigned char* data  = stbi_load(file.c_str(), &width, &height, &nrChnnels, 0);
		if (data) 
		{
		glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + count++, 0,
            GL_RGB+is_alpha, width, height, 0, GL_RGB+is_alpha, GL_UNSIGNED_BYTE, data
        );
		stbi_image_free(data);

		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		

	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void Bezeir(GLFWwindow* window, float* points,  float* dots,\
	unsigned long long dot_size, unsigned long long points_size, \
	int count, Shader line, Shader point)
{
	clock_t start;
	unsigned long long dot_max = dot_size / (3 * sizeof(float)), point_max = points_size / (3 * sizeof(float));
	float rate;
	float* dot = (float*)malloc(dot_size);
	for (int i = 0; i < point_max; i++)
	{

		for (int j = 0; j < dot_max; j++)
		{
			dot[j] = dots[j];
		}
		rate = ((float)i / (float)(point_max));
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		for (int j = count; j > 1; j--)
		{
			line.use();
			line.setFloat("offset", 0.5f * ((float)j / (float)count));
			glBufferData(GL_ARRAY_BUFFER, dot_size, dot, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_LINE_STRIP, 0, j);
			for (int k = 0; k < j - 1; k++)
			{
				dot[3 * k] = rate * dot[3 * k] + (1.0f - rate) * dot[3 * k + 3];
				dot[3 * k + 1] = rate * dot[3 * k + 1] + (1.0f - rate) * dot[3 * k + 4];
			}
		}
		points[3 * i] = dot[0];
		points[3 * i + 1] = dot[1];
		points[3 * i + 2] = dot[2];
		point.use();
		glBufferData(GL_ARRAY_BUFFER, points_size, points, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_POINTS, 0, i);
		glfwSwapBuffers(window);
		glfwPollEvents();
		start = clock();
		while ((clock() - start) < 10);
	}
}

unsigned int bind_model(models &model, int times)
{
	for(int i = 0 ; i < times;i++)
  		model.devide();

    std::vector<unsigned int> indices = model.get_indicies();
    std::vector<glm::vec3>vertices = model.get_vertices();
    std::vector<glm::vec3>normals = model.get_normal();
    // std::vector<glm::vec3>normals = model.get_sharp_normal();

    std::vector<glm::vec2>texture = model.get_texture();
    // std::cout<<indices.size()<<std::endl;
    // std::cout<<vertices.size()<<std::endl;
    std::vector<glm::vec3>eles;
    for(int i = 0 ; i < vertices.size(); i++)
    {
        eles.push_back(vertices[i]);
        eles.push_back(normals[i]);
        eles.push_back(glm::vec3(texture[i],1.0));

    }
    Bind_data((int)9, (char)2,&(model.buffer.VAO), &(model.buffer.VBO), &(eles[0]), 3*sizeof(float)*eles.size(), (int)9, (int)3, (int) 3, (int) 3);
    Bind_data(5,0,&(model.buffer.EBO),&(indices[0]),sizeof(int)*indices.size());
	model.buffer.size=indices.size();
	return 0;
}
unsigned int bind_edge(models model,unsigned int * VAO, unsigned int * VBO, unsigned int * EBO, int times)
{
	for(int i = 0 ; i < times;i++)
  		model.devide();

    std::vector<unsigned int> indices = model.get_edges();
    std::vector<glm::vec3>vertices = model.get_vertices();

    std::cout<<indices.size()<<std::endl;
    std::cout<<vertices.size()<<std::endl;
    std::vector<glm::vec3>eles;
    for(int i = 0 ; i < vertices.size(); i++)
    {
        eles.push_back(vertices[i]);
    }
    // Bind_data((int)7, (char)2,VAO, VBO, &(eles[0]), 3*sizeof(float)*eles.size(), (int)3, (int)3);
    Bind_data(5,0,EBO,&(indices[0]),sizeof(int)*indices.size());
	return indices.size();
}

DL_Infactor getDLInfactor(int distance)
{
	return DLI_map.lower_bound(distance)->second;
}
void init_imgfunc(ImgShader *shader)
{
	float vertices []=
	{
	-1.0, -1.0, 0.0,	0.0,1.0,
	-1.0,  1.0, 0.0,	0.0,0.0,
	1.0	, -1.0, 0.0,	1.0,1.0,
	1.0 ,  1.0, 0.0,	1.0,0.0,
	1.0	, -1.0, 0.0,	1.0,1.0,
	-1.0,  1.0, 0.0,	0.0,0.0
	};
	Bind_data((int)8,(char)2,&(shader->buffer.VAO),&(shader->buffer.VBO),&(vertices[0]),sizeof(vertices),(int)5, (int)3, (int) 2);
	shader->buffer.size=6;
}
void load_img(ImgShader* shader, char * path,int isalpha)
{
	unsigned int text;
	load_text(&text, 0, path, isalpha);
	int width, height, nrChnnels;
	stbi_load(path, &width, &height, &nrChnnels, 0);
	float rate = ((float)height)/((float)width);
	shader->img_buffer.push_back({text,0,0,0.5f,0.5f*rate});
		std::cout<<shader->img_buffer[0].texture<<std::endl;
}
void rend_img(ImgShader* shader)
{
	shader->use();
	for(auto img: shader->img_buffer)
	{
		// shader->setFloat("offset_x",img.offset_x);
		// shader->setFloat("offset_y",img.offset_y);
		shader->setFloat("offset_x",hit_pos[0]);
		shader->setFloat("offset_y",hit_pos[1]);
		shader->setFloat("scale_x",img.scale_x);
		shader->setFloat("scale_x",0.1);
		shader->setFloat("scale_y",img.scale_y);
		shader->setFloat("scale_y",0.1);
		glBindTexture(GL_TEXTURE_2D, img.texture);
		glBindVertexArray(shader->buffer.VAO);
		glDrawArrays(GL_TRIANGLES,0,shader->buffer.size);
		// glDrawElements(GL_TRIANGLES, shader->buffer.size, GL_UNSIGNED_INT, 0);
	}


}



