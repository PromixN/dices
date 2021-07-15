#pragma once
#ifndef GEOM_H

#define GEOM_H
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_transform.hpp>
#define LEFT  0
#define RIGHT 1
#define FRONT 2
#define BACK  3
#define TOP   4
#define BOTTOM 5

#define DIFFUSE 1
#define MIRROR  2
#define TRANSP  3
#define TEXTED  4
#define EMISSION 5

using namespace glm;
 int check_direction(vec3 v1,vec3 v2,vec3 v3);
 float  get_reflect_index(float cos_i,float refract_from,float refract_to);
 float get_transpare_index(float cos_i,float refract_from,float refract_to);
 vec3 get_text(float * texture,float x,float y);
void set_text(float * texture,float x,float y,vec3 color);

class Shape
{
public:
	unsigned int ID;
	unsigned int TYPE;
 	float refractive_index;
 	float *texture;
 	vec3 color;
	 Shape()
	{
		ID = -1;
		TYPE =0;
	 	refractive_index=1.0f;
	}
	 Shape(const Shape&);
	 virtual float intersect(vec3 pos, vec3 ray)
	{
		return 1.0f;
	}
	 virtual void shader(vec3 pos, vec3 *col)
	{
		*col = vec3(0.01f);
	}
 	virtual void set_shader(vec3 pos, vec3 col)
	{
	}
	  ~Shape(){};
 	virtual  void refract (vec3 pos,vec3* dir,float refract_index)
 	{
 		return ;
 	}
 	virtual  vec3 get_nor (vec3 pos)
 	{
 		return vec3();
 	}
 	virtual  vec3 get_x_axis (vec3 pos,vec3 dir)
 	{
 		return vec3();
 	}
 	virtual void init_text()
 	{
 	}
};


class Plane :public Shape
{
public:
	vec3 center;
	vec3 normal_vec;
	vec3 x_axis;
	vec3 y_axis;
	float	width;
	float	height;
	float   d;
	 Plane()
	{
		center = vec3(.0f, .0f, .0f);
		normal_vec = vec3(.0f, .0f, .0f);
		x_axis = vec3(.0f, .0f, .0f);
		y_axis = vec3(.0f, .0f, .0f);
		width = 0.0f;
		height = 0.0f;
		d = 0.0f;
	 	refractive_index=1.0f;
	 	color =vec3();
	}
	 Plane(vec3 cent,vec3 nor,vec3 x,float wid,float hei,unsigned int id ,unsigned int type)
	{
		center = cent;
		normal_vec = nor;
		x_axis = x;
		y_axis = glm::cross(nor,x);
		width = wid;
		height = hei;
		d = glm::dot(nor, cent);
		ID=id;
		TYPE =type;
		refractive_index=1.0f;
	 	color =vec3(0.5);
	 	texture =(float *)malloc(sizeof(float)*256*256*3);
	 	for(int i=0;i<256*256*3;i++)texture[i]=0.15f;
	}
	 Plane(vec3 cent,vec3 nor,vec3 x,float wid,float hei,vec3 col,float refract_index,unsigned int id ,unsigned int type)
	{
		center = cent;
		normal_vec = nor;
		x_axis = x;
		y_axis = glm::cross(nor,x);
		width = wid;
		height = hei;
		d = glm::dot(nor, cent);
		ID=id;
		TYPE =type;
		refractive_index=refract_index;
	 	color =col;
	 	
	 	texture =(float *)malloc(sizeof(float)*256*256*3);
	 	
	 	for(int i=0;i<256*256*3;i++)texture[i]=0.15f;
	}
	 float intersect(vec3 pos, vec3 dir) override
	{
		if (abs(glm::dot(dir, normal_vec)) < FLT_EPSILON) return - 1.0f;
		else
		{
			vec3 relative_pos = pos - center;
			float distance = -glm::dot(relative_pos, normal_vec)/(glm::dot(dir,normal_vec));
			relative_pos = relative_pos + distance * dir ;
			float x = glm::dot(relative_pos, x_axis);
			float y = glm::dot(relative_pos, y_axis);
			if (-width / 2.0f < x && x < width / 2.0f && -height / 2.0f < y && y < height / 2.0f)
				return distance;
			else
				return -1.0f;
		}
	}
	 void shader(vec3 pos,vec3 *col) override
	{
	 	vec3 relative_pos = pos-(center-0.5f*width*x_axis-0.5f*height*y_axis);
	 	float x =dot(relative_pos,x_axis)/width;
	 	float y = dot(relative_pos,y_axis)/height;
	 	//printf("p");
		*col=  get_text(texture,x,y);
		
	}
	void set_shader(vec3 pos, vec3 col) override
	{
	 	vec3 relative_pos = pos-(center-0.5f*width*x_axis-0.5f*height*y_axis);
	 	float x =dot(relative_pos,x_axis)/width;
	 	float y = dot(relative_pos,y_axis)/height;
	 	set_text(texture,x,y,col);
	}
	  void refract (vec3 pos,vec3* dir,float refract_index) override
 	{
	  vec3 axis = normalize(cross(get_nor(pos),*dir));
	 	float cos_i = dot(get_nor(pos),*dir);
	 	if(abs(cos_i)>1-FLT_EPSILON) return;
	 	float sin_i =sqrt(1-cos_i*cos_i);
		char  outside_in = cos_i<FLT_EPSILON;
	 	float sin_o = outside_in? sin_i*refract_index/refractive_index : sin_i*refract_index;
		if(sin_o>0.99f||sin_o<-0.99f)
		{
			*dir=-*dir;
			return;
		}
	 	float angle_i = asin(sin_i);
	 	float angle_o = asin(sin_o);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, angle_o-angle_i, normalize(axis));
	 	*dir = (trans*vec4(*dir,1.0));
 		return ;
 	}
 	  vec3 get_nor (vec3 pos) override
 	{
 		return normal_vec;
 	}
 	 vec3 get_x_axis (vec3 pos,vec3 dir) override
 	{
 		return x_axis;
 	}
	void init_text() override
 	{
 		for(int i=0;i<256*256*3;i++) texture[i]=0.15f;
 	}
};

class Cuboid:public Shape
{
public:
	vec3 center;
	vec3 x_axis;
	vec3 y_axis;
	vec3 z_axis;
	Plane planes[6];
	float length;
	float width;
	float height;

	 Cuboid()
	{
		center = vec3(.0f, .0f, .0f);
		x_axis = vec3(.0f, .0f, .0f);
		y_axis = vec3(.0f, .0f, .0f);
		z_axis = vec3(.0f, .0f, .0f);
		length = 0.0f;
		width = 0.0f;
		height = 0.0f;
	 	refractive_index=1.0f;
	 	color=vec3();
		for(int i =0;i<6;i++)
		{
			planes[i] = Plane();
		}

	}
	 Cuboid(vec3 cent,vec3 x,vec3 y,float len,float wid,float hei,vec3 col,float refract_index,unsigned int id,unsigned int type)
	{
		center = cent;
		x_axis = x;
		y_axis = y;
		z_axis = glm::cross(x,y);
		length = len;
		width = wid;
		height = hei;
		ID =id;
		TYPE =type;
	 	refractive_index=refract_index;
	 	color =col;
		planes[LEFT] = Plane(cent - x * len / 2.0f, -x, -y, wid, hei,col,refract_index,LEFT,type);
		planes[RIGHT] = Plane(cent + x * len / 2.0f, x, y, wid, hei,col,refract_index,RIGHT,type);
		planes[FRONT] = Plane(cent - y * wid / 2.0f, -y, x, len, hei,col,refract_index,FRONT,type);
		planes[BACK] = Plane(cent + y * wid / 2.0f, y, -x, len, hei,col,refract_index,BACK,type);
		planes[TOP] = Plane(cent + z_axis * hei / 2.0f, z_axis, x, len, wid,col,refract_index,TOP,type);
		planes[BOTTOM] = Plane(cent - z_axis * hei / 2.0f, -z_axis, x, len, wid,col,refract_index,BOTTOM,type);
	}
	 float intersect(vec3 pos, vec3 dir) override
	{
		float distance;
		float min_pos_distance=INFINITY;
		for(int i=0;i<6;i++)
		{
			distance = planes[i].intersect(pos, dir);
			if (distance > FLT_EPSILON && distance < min_pos_distance) 
				min_pos_distance = distance;
		}
		return min_pos_distance;
	}
	 void shader(vec3 pos,vec3 *col) override
	{
		vec3 relative_pos=pos-center;
	 	float x=dot(relative_pos,x_axis)/length;
	 	float y=dot(relative_pos,y_axis)/width;
	 	float z=dot(relative_pos,z_axis)/height;
	 	int choose_plane;
	 	if(abs(x)>abs(y))
	 	{
	 		if(abs(x)>abs(z))
				choose_plane=std::signbit(x)? 1 :0;
			else
				choose_plane=std::signbit(z)? 4 :5;
		}else
		{
			if(abs(y)>abs(z))
				choose_plane= std::signbit(y)? 3 :2;
			else
				choose_plane=std::signbit(z)? 4 :5;
		}
	 	planes[choose_plane].shader(pos,col);
	
	}
	  void refract (vec3 pos,vec3* dir,float refract_index) override
 	{
 		 vec3 axis = normalize(cross(get_nor(pos),*dir));
	 	float cos_i = dot(get_nor(pos),*dir);
	 	if(abs(cos_i)>1-FLT_EPSILON) return;
	 	float sin_i =sqrt(1-cos_i*cos_i);
		char  outside_in = cos_i<FLT_EPSILON;
	 	float sin_o = outside_in? sin_i*refract_index/refractive_index : sin_i*refract_index;
		if(sin_o>0.99f||sin_o<-0.99f)
		{
			*dir=-*dir;
			return;
		}
	 	float angle_i = asin(sin_i);
	 	float angle_o = asin(sin_o);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, angle_o-angle_i, normalize(axis));
	 	*dir = (trans*vec4(*dir,1.0));
 		return ;
 	}
 	  vec3 get_nor (vec3 pos) override
 	{
	 	// need edited
 		vec3 relative_pos=pos-center;
	 	float x=dot(relative_pos,x_axis)/length;
	 	float y=dot(relative_pos,y_axis)/width;
	 	float z=dot(relative_pos,z_axis)/height;
	 	int choose_plane;
	 	if(abs(x)>abs(y))
	 	{
	 		if(abs(x)>abs(z))
				choose_plane=std::signbit(x)? 1 :0;
			else
				choose_plane=std::signbit(z)? 4 :5;
		}else
		{
			if(abs(y)>abs(z))
				choose_plane= std::signbit(y)? 3 :2;
			else
				choose_plane=std::signbit(z)? 4 :5;
		}
	 	return planes[choose_plane].get_nor(pos);
 	}
	void set_shader(vec3 pos, vec3 col) override
	{
	 	vec3 relative_pos=pos-center;
	 	float x=dot(relative_pos,x_axis)/length;
	 	float y=dot(relative_pos,y_axis)/width;
	 	float z=dot(relative_pos,z_axis)/height;
	 	int choose_plane;
	 	if(abs(x)>abs(y))
	 	{
	 		if(abs(x)>abs(z))
				choose_plane=std::signbit(x)? 1 :0;
			else
				choose_plane=std::signbit(z)? 4 :5;
		}else
		{
			if(abs(y)>abs(z))
				choose_plane= std::signbit(y)? 3 :2;
			else
				choose_plane=std::signbit(z)? 4 :5;
		}
			planes[choose_plane].set_shader(pos,col);
	}
 	 vec3 get_x_axis (vec3 pos,vec3 dir) override
 	{
 		vec3 relative_pos=pos-center;
	 	 float x=dot(relative_pos,x_axis)/length;
	 	float y=dot(relative_pos,y_axis)/width;
	 	float z=dot(relative_pos,z_axis)/height;
	 	int choose_plane;
	 	if(abs(x)>abs(y))
	 	{
	 		if(abs(x)>abs(z))
				choose_plane=std::signbit(x)? 1 :0;
			else
				choose_plane=std::signbit(z)? 4 :5;
		}else
		{
			if(abs(y)>abs(z))
				choose_plane= std::signbit(y)? 3 :2;
			else
				choose_plane=std::signbit(z)? 4 :5;
		}
	 	return planes[choose_plane].get_x_axis(pos,dir);
 	}
	void init_text() override
 	{
 		for(int i=0;i<6;i++)
 		{
 			planes[i].init_text();
 		}
 	}
};

class Triangular_patch :public Shape
{
public:
	vec3 vertex_a;
	vec3 vertex_b;
	vec3 vertex_c;
	vec3 a_b;
	vec3 b_c;
	vec3 c_a;
	vec3 normal_vec;
	 Triangular_patch()
	{
		vertex_a = vec3(.0f, .0f, .0f);
		vertex_b = vec3(.0f, .0f, .0f);
		vertex_c = vec3(.0f, .0f, .0f);
		a_b = vec3(.0f, .0f, .0f);
		b_c = vec3(.0f, .0f, .0f);
		c_a = vec3(.0f, .0f, .0f);
		normal_vec = vec3(.0f, .0f, .0f);
		ID =1;
		TYPE =0;
	 	color=vec3();
	 	refractive_index=1.0f;
	 	
	 	texture =(float *)malloc(sizeof(float)*256*256*3);
	 	
	 	for(int i=0;i<256*256*3;i++)texture[i]=0.15f;
	}
	 Triangular_patch(vec3 a,vec3 b, vec3 c,vec3 col,float refract_index,unsigned int id,unsigned int type)
	{
		vertex_a = a;
		vertex_b = b;
		vertex_c = c;
		a_b = vertex_b - vertex_a;
		b_c = vertex_c - vertex_b;
		c_a = vertex_a - vertex_c;
		normal_vec = glm::normalize(glm::cross(a_b, b_c));
		ID= id;
		TYPE = type;
	 	refractive_index=refract_index;
	 	color=col;
	 	
	 	texture =(float *)malloc(sizeof(float)*256*256*3);
	 	
	 	for(int i=0;i<256*256*3;i++)texture[i]=0.15f;
	}
	float intersect(vec3 pos, vec3 dir) override
	{
		if (abs(glm::dot(dir, normal_vec)) < FLT_EPSILON) return -1.0f;
		else
		{
			
			float distance = -(glm::dot(pos, normal_vec) - glm::dot(vertex_a,normal_vec)) / \
							(glm::dot(dir, normal_vec));
			vec3 intersect_point = pos + distance * dir;
			vec3 a_p=intersect_point-vertex_a;
			vec3 b_p=intersect_point-vertex_b;
			vec3 c_p=intersect_point-vertex_c;
			a_p = glm::cross(a_p, a_b);
			b_p = glm::cross(b_p, b_c);
			c_p = glm::cross(c_p, c_a);
			if (check_direction(a_p, b_p, c_p))
				return distance;
			else
				return -1.0f;
		}
	}

	  void refract (vec3 pos,vec3* dir,float refract_index) override
 	{
 		 vec3 axis = normalize(cross(get_nor(pos),*dir));
	 	float cos_i = dot(get_nor(pos),*dir);
	 	if(abs(cos_i)>1-FLT_EPSILON) return;
	 	float sin_i =sqrt(1-cos_i*cos_i);
		char  outside_in = cos_i<FLT_EPSILON;
	 	float sin_o = outside_in? sin_i*refract_index/refractive_index : sin_i*refract_index;
		if(sin_o>0.99f||sin_o<-0.99f)
		{
			*dir=-*dir;
			return;
		}
	 	float angle_i = asin(sin_i);
	 	float angle_o = asin(sin_o);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, angle_o-angle_i, normalize(axis));
	 	*dir = (trans*vec4(*dir,1.0));
 		return ;
 	}
 	  vec3 get_nor (vec3 pos) override
 	{
 		return normal_vec;
 	}
 	 vec3 get_x_axis (vec3 pos,vec3 dir) override
 	{
 		return normalize(a_b);
 	}
	 void shader(vec3 pos,vec3 *col) override
	{
	 	vec3 x = normalize(a_b);
	 	vec3 y = cross(x,normal_vec);
	 	pos	= pos-vertex_a;
	 	x.x=dot(pos,x);
	 	y.y=dot(pos,y);
	 	//printf("t");
	 	get_text(texture,x.x,y.y);
	}
	void set_shader(vec3 pos, vec3 col) override
	{
	 	vec3 x = normalize(a_b);
	 	vec3 y = cross(x,normal_vec);
	 	pos	= pos-vertex_a;
	 	x.x=dot(pos,x);
	 	y.y=dot(pos,y);
	 	set_text(texture,x.x,y.y,col);
	}
	void init_text() override
 	{
 		for(int i=0;i<256*256*3;i++) texture[i]=0.15f;
 	}
};

class Thousand_prism:public Shape
{
	
};

class Sphere :public Shape
{
public:
	vec3 position;
	vec3 emission;
	float diameter;
	vec3 velocity;
	
	 Sphere()
	{
		position = vec3(0.0f, 0.0f, 0.0f);
		diameter = -1.0f;
		emission = vec3(0.0f, 0.0f, 0.0f);
		color = vec3(0.0f, 0.0f, 0.0f);
		texture = nullptr;
		refractive_index = 1.0f;
		ID =1;
		TYPE =0;
		velocity = vec3(0.0f,0.0f,0.0f);
	 	
	 	refractive_index=1.0f;
	} 
	 Sphere(vec3 pos) 
	{
		position = pos;
		diameter = -1.0f;
		emission = vec3(0.0f, 0.0f, 0.0f);
		color = vec3(0.0f, 0.0f, 0.0f);
		texture = nullptr;
		refractive_index = 1.0f;
		ID =1;
		TYPE =0;
		velocity = vec3(0.0f,0.0f,0.0f);
	 	
	 	refractive_index=1.0f;
	}
	 Sphere(vec3 pos, float dia,vec3 emis,vec3 col,float * tex,float refract_index,unsigned int id, unsigned int type) 
	{
		position = pos;
		diameter = dia;
		emission = emis;
		color = col;
		texture = (float*) malloc(sizeof(float)*256*256*3);
		ID= id;
		TYPE =type;
		velocity = vec3(0.0f,0.0f,0.0f);
	 	refractive_index=refract_index;
	 	for(int i=0;i<256*256*3;i++)texture[i]=0.15f;
	}

	 float intersect(vec3 pos,vec3 dir) override
	{
		vec3 point_to_center = pos - position;
		float radial_distance = glm::dot(point_to_center, point_to_center);
		float insect_angle_cos = glm::dot(dir, point_to_center) / \
			sqrt(glm::dot(dir, dir) * radial_distance);
		float b = 2.0f * sqrt(radial_distance) * insect_angle_cos;
		float c = radial_distance - diameter * diameter;

		float delta = b * b - 4 * c;
		if (delta<FLT_EPSILON)
		{
			return -1.0f;
		}
		else
		{
			float sqrt_delta = sqrt(delta);
			if (-b - sqrt_delta < FLT_EPSILON) return (-b + sqrt_delta) / 2.0f;
			else return (-b - sqrt_delta) / 2.0f;
		}
	}
	  void refract (vec3 pos,vec3* dir,float refract_index) override
 	{
 		 vec3 axis = normalize(cross(get_nor(pos),*dir));
	 	float cos_i = dot(get_nor(pos),*dir);
	 	if(abs(cos_i)>1-FLT_EPSILON) return;
	 	float sin_i =sqrt(1-cos_i*cos_i);
		char  outside_in = cos_i<FLT_EPSILON;
	 	float sin_o = outside_in? sin_i*refract_index/refractive_index : sin_i*refract_index;
		if(sin_o>0.99f||sin_o<-0.99f)
		{
			*dir=-*dir;
			return;
		}
	 	float angle_i = asin(sin_i);
	 	float angle_o = asin(sin_o);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, angle_o-angle_i, normalize(axis));
	 	*dir = (trans*vec4(*dir,1.0));
 		return ;
 	}
 	  vec3 get_nor (vec3 pos) override
 	{
 		return normalize(pos-position);
 	}
 	 vec3 get_x_axis (vec3 pos,vec3 dir) override
 	{
 		return normalize(cross(dir,get_nor(pos)));
 	}
	void set_velocity(vec3 vel)
	{
		velocity =vel;
	}
	void move()
	 {
		 if(position.z>0.2f) position+=velocity;
		 else
		 {
			 velocity =-velocity;
		   	 position+=velocity;
		 }
	 }
	 void shader(vec3 pos,vec3 *col) override
	{
	 	vec3 relative_pos = get_nor(pos);
	 	vec3 cos_i =relative_pos*relative_pos;
	 	float x =acos(cos_i.x+cos_i.y)/pi<float>()+0.5f;
	 	vec2 project =normalize(vec2(relative_pos.x,relative_pos.y));
	 	float y =(0.5f*acos(project.x)/pi<float>()+0.25f)+project.y>0?0.0f:0.5f;
	 	
	 //	printf("s");
		*col=  get_text(texture,x,y);
	}
	void set_shader(vec3 pos, vec3 col) override
	{
	 	vec3 relative_pos = get_nor(pos);
	 	vec3 cos_i =relative_pos*relative_pos;
	 	float x =acos(cos_i.x+cos_i.y)/pi<float>()+0.5f;
	 	vec2 project =normalize(vec2(relative_pos.x,relative_pos.y));
	 	float y =(0.5f*acos(project.x)/pi<float>()+0.25f)+project.y>0?0.0f:0.5f;
	 	set_text(texture,x,y,col);
	}
	void init_text() override
 	{
 		for(int i=0;i<256*256*3;i++) texture[i]=0.15f;
 	}
	
};
#endif 