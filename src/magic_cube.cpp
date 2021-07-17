#include <glpre.hpp>

int     hight_index;                //hight light choosed cube
int     rotate_direction;           // 1 for cw and 0 for ccw
glm::uvec3 at_pos;                  // tuple for reprenting index on each axis
int     to_axis;                    // rotate axis
int     touch_face;                 // choosed face

//box collider for magic cube
Cuboid box(
        glm::vec3(0),
        glm::vec3(1.0,0,0),
        glm::vec3(0,1,0),
        6,6,6,
        glm::vec3(0),
        1.0,
        1,
        1);
glm::vec3 hit_pos=glm::vec3(0);     //where cursor_ray intersect with box collider

//use left, upper, front face as base index
//in ccw order, the last in each face is the center of face
unsigned int cube_idx[3][9]=        
{
	{10 ,19 ,22 ,25,16,7,4,1,13},
	{7 ,8 ,9 ,18,27,26,25,16,17},
	{1 ,2 ,3 ,6 ,9 ,8 ,7 ,4 ,5 }
};
//index offset for each axis and each index on axis
int cube_pos_d[3][3]                
{
	{0 , 1, 2 },
	{-6,-3, 0 },
	{0 , 9, 18}
};
glm::vec3 axises [3]=
{
	glm::vec3(1.0,0.0,0.0),
	glm::vec3(0.0,1.0,0.0),
	glm::vec3(0.0,0.0,1.0)
};

// face_dir* consists of combination of axis and direction;
// each element can be reprented by 2*axis + direction

//first dim for look_from direction, in back-right-left-front order
//second dim for mouse move direction, in left-upper-lower-right order
int face_dir1[4][4]=
{
	{4,1,0,5},
	{0,4,5,1},
	{1,5,4,0},
	{5,0,1,4}
};
//right,left,front,back
int face_dir2[4][4]=
{
	{3,4,5,2},
	{3,5,4,2},
	{3,1,0,2},
	{3,0,1,2}
};

//the x-y-z order doesn't match that in face_dir
int face_map[6]={0,1,4,5,2,3};

char const* ax_str  [] = {"x","y","z"};
char const* fa_str  [] = {"右","左","前","后","上","下"};
char const* dir_str [] = {"逆","顺"};


void init_list(cube_dsc_t* dsc)
{
	dsc->next=dsc->prev=dsc;
}
void push_list(cube_dsc_t* dsc,cube_dsc_t* new_dsc)
{
	new_dsc->next=dsc;
	new_dsc->prev=dsc->prev;
	dsc->prev->next=new_dsc;
	dsc->prev=new_dsc;
}
glm::uvec3 get_index(glm::vec3 pos)
{
	glm::uvec3 cor;
	for(int i = 0; i<3; i++)
	{
		if(abs(pos[i])>2.9)
		{
			touch_face=face_map[2*i+(pos[i]<0)];
		}
		if(pos[i]>1.0f)
			cor[i]=2;
			else if (pos[i]>-1.0f)
				cor[i]=1;
				else cor[i]=0;
	}
	cor[2]=2-cor[2];  //different increase order on z-axis
	return cor;
}
void get_axis_direction(int &axis,int &direction)
{
	if(touch_face<4)
	{
		axis = face_dir2[touch_face][move_dir]/2;
		direction = face_dir2[touch_face][move_dir]%2;
	}
	else 
	{
		float xoffset = -camera_main.Front.x;
		float yoffset = camera_main.Front.z;
		auto ur = glm::dot(glm::vec2(xoffset,yoffset),glm::vec2(-1.0,1.0));
		auto ul = glm::dot(glm::vec2(xoffset,yoffset),glm::vec2(1.0,1.0));
		int dir = (ur>0? 2:0)+(ul>0? 1:0);
		axis = face_dir1[dir][move_dir]/2;
        if(touch_face==5&&to_axis==0)
			direction = 1-(face_dir1[dir][move_dir]%2);
		else
			direction = face_dir1[dir][move_dir]%2;
	}

}
void choose_cube(double xpos,double ypos)
{
	float x_0 = view_x+window_width*0.5;
	float y_0 = view_y+window_height*0.5;

    //relative x/y on srceen
	float x = 2*(xpos-x_0)/window_width;    
	float y = -2*(ypos-y_0)/window_height;
	// std::cout<<x<<"::"<<y<<std::endl;

    //gradients
	float y_front = tan(radians(camera_main.Angle)*0.5); 
	float x_front = camera_main.Rate*y_front;

	auto front = camera_main.Front;
	float distance = sqrt(glm::dot(front,front));
	glm::vec3 Front = glm::normalize(vec3(camera_main.Front.x,0.0f,camera_main.Front.z));
	glm::vec3 right = glm::normalize(cross(Front,camera_main.WorldUp));
	glm::vec3 up    = glm::normalize(cross(camera_main.Right,camera_main.Front));

	auto dir = (right*x*x_front+up*y*y_front)*distance+front; //a point in the cursor direction
	dir = glm::normalize(dir);

	auto pos = camera_main.Position;
	hit_pos =dir*box.intersect(pos,dir)+pos;
	// printf("%f:%f:%f\n",pos.x,pos.y,pos.z);
	// printf("%f:%f:%f\n",front.x,front.y,front.z);
	// printf("%f:%f:%f\n",dir.x,dir.y,dir.z);
	// printf("%f\n",box.intersect(pos,dir));
	// printf("%f:%f:%f\n\n",pos.x,pos.y,pos.z);
	at_pos = get_index(hit_pos);
	hight_index=9*at_pos[2]+3*at_pos[1]+at_pos[0]; 
}

void rend_magic_cube(Shader& shader,models* model,Camera& camera,int interval)
{
	shader.use();
	glm::mat4 view       = camera.view;
	glm::mat4 projection = glm::perspective(radians(camera.Angle), camera.Rate, 0.1f, 10000.0f);
	
    // glActiveTexture(GL_TEXTURE0+0);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, 3);
    // glActiveTexture(GL_TEXTURE0+1);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, 2);
	shader.setInt("texture1",0);
	shader.setInt("texture2",1);
	cube_dsc_t * dsc = cube_dsc_head->prev;
	float agl_d = dsc->record.direction? -1:1;
	agl_d*=(float(interval)*0.5*PI/INTERVAL_RTT);
	for(int i = 0; i < 27; i++ )
	{
		glm::mat4 model_t= glm::mat4(1.0f);
		model_t = glm::translate(model_t,model->p+dsc->pos[i])*model->model_t;
		model_t = glm::scale(model_t,model->scale_t);
		shader.setMat4("model",model_t);
		shader.setMat4("view",view);
		shader.setVec3("viewPos",camera.Position);
		if(i==hight_index)
			shader.setVec3("highlight",glm::vec3(0.1,0.1,0.1));
		else
			shader.setVec3("highlight",glm::vec3(0,0,0));
		shader.setMat4("projection",projection);
		int found = -1;
		int count = 0;
		if(interval<INTERVAL_RTT)
			for(auto & pair : dsc->operand)
			{
				if(pair[1]==i)
					found = count;
				count++;
			}
		else
			frame_move = 0;
		if(interval<INTERVAL_RTT&&found!=-1)
		{
			auto trans = glm::rotate(glm::mat4(1.0),agl_d,axises[dsc->record.axis]);
			shader.setMat4("rotate",trans*dsc->prev->rotate[dsc->operand[found][0]]);
		}
		else
			shader.setMat4("rotate",dsc->rotate[i]);
    	// shader.setInt("Texture1", 1);
		glBindVertexArray(model->buffer.VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->buffer.EBO);
		glDrawElements(GL_TRIANGLES, model->buffer.size, GL_UNSIGNED_INT, 0);
	}

}
void rend_magic_cube(Shader& shader,models* model,Camera& camera)
{
	shader.use();
	glm::mat4 view       = camera.view;
	glm::mat4 projection = glm::perspective(camera.Angle, camera.Rate, 0.1f, 10000.0f);
	shader.setInt("texture1",0);
	shader.setInt("texture2",1);
	cube_dsc_t * dsc = cube_dsc_head->prev;
	for(int i = 0; i < 27; i++ )
	{
		glm::mat4 model_t= glm::mat4(1.0f);
		model_t = glm::translate(model_t,model->p+dsc->pos[i])*model->model_t;
		model_t = glm::scale(model_t,model->scale_t);
		shader.setMat4("model",model_t);
		shader.setMat4("view",view);
		shader.setVec3("viewPos",camera.Position);
		shader.setMat4("projection",projection);
		shader.setMat4("rotate",dsc->rotate[i]);
    	// shader.setInt("Texture1", 1);
		glBindVertexArray(model->buffer.VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->buffer.EBO);
		glDrawElements(GL_TRIANGLES, model->buffer.size, GL_UNSIGNED_INT, 0);
	}

}
void step(int axis, int pos, int cw)
{
    int idx_d = cw? 6: 2; //8 cube rotate index with stride of 2
    float agl_d = cw? -0.5*PI:0.5*PI;
    int idx_offset = cube_pos_d[axis][pos];
    cube_dsc_t * new_dsc = new(cube_dsc_t);
    cube_dsc_t * prev = cube_dsc_head->prev;
    *new_dsc = *(cube_dsc_head->prev);
    new_dsc->operand = std::vector<uvec2>(0);
    for(int i = 0; i < 9; i ++)
    {
        int index = cube_idx[axis][i]+idx_offset-1; 
        int next;
        if(i<8)
            next = cube_idx[axis][(i+idx_d)%8]+idx_offset-1; 
        else 
            next = index;
        auto trans = glm::rotate(glm::mat4(1.0),agl_d,axises[axis]);
        new_dsc->pos[next]      = prev->pos[index];
        new_dsc->index[next]    = prev->index[index];
        new_dsc->rotate[next]   = trans*prev->rotate[index];
        new_dsc->operand.push_back(glm::uvec2(index,next));
    }
    new_dsc->record = {axis,pos,cw};
    push_list(cube_dsc_head,new_dsc);
    frame_count = 0;
    frame_move = 1;
}
void rend_dot(Shader& shader,unsigned int VAO,Camera& camera)
{
	shader.use();
	glm::mat4 view       = camera.view;
	glm::mat4 projection = glm::perspective(radians(camera.Angle), camera.Rate, 0.1f, 10000.0f);
	// glm::mat4 model_t= glm::scale(glm::mat4(1.0f),glm::vec3(0.1,0.1,0.1));
	glm::mat4 model_t= glm::mat4(1.0f);
	model_t = glm::translate(model_t,hit_pos);
	shader.setMat4("model",model_t);
	shader.setMat4("view",view);
	shader.setVec3("viewPos",camera.Position);
	shader.setMat4("projection",projection);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES,0,9);
}

