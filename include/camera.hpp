#pragma once

#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include "glm/glm.hpp"

using namespace glm;

enum Camere_type
{
	LOOKINGAT,
	EULER,
	FREE,
	CONSTANT
};

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
constexpr char move_mask [6]= {32,16,8,4,2,1};

class Camera
{
public:
	mat4 view;
	vec3 Position,
		 Front,
		 Up,
		 Right,
		 WorldFront,
		 WorldRight,
		 WorldUp;

	 Camere_type Style;
	
	vec3 LookingAt;

	float Yaw,
		  Pitch,
		  Roll;

	float MoveSpeed,
		  MouseSensitivity,
		  AngleSensitivity;
	
	float Zoom;

	float Angle;
	float Rate;//width on height

	int pause;

	

	Camera(vec3 position =vec3(0.0f,0.0f,3.0f), vec3 lookingat = vec3(0.0f), vec3 up = vec3(0.0f,1.0,0.0f),Camere_type style=FREE)
	{
		Style =style;
		view=mat4(1.0f);
		Position = position;
		WorldFront = vec3(0.0f,0.0f,-1.0f);
		WorldRight = vec3(1.0f,0.0f,0.0f);
		WorldUp = vec3(0.0f,1.0f,0.0f);
		Front=normalize(LookingAt-Position);
		Yaw = Pitch = Roll =0.0f;
		LookingAt = lookingat;
		LookingTo();
		MouseSensitivity =0.5f;
		AngleSensitivity =0.05f;
		MoveSpeed =1.0f;
		Zoom = 1.0f;
		Angle = 45.0f;
		Rate  = 1.0f;
		pause = 1;
	}
	


	void set_style(Camere_type style)
	{
		Style = style;
	}

	void set_yaw(float yaw)
	{
		Yaw = yaw;
	}
	void set_pitch(float pitch)
	{
		Pitch = pitch;
	}
	void set_roll(float roll)
	{
		Roll = roll;
	}

	 void ProcessKeyboard(char direction, float deltaTime)
    {
		 if(Style == CONSTANT) return ;

		int move_dirs [6];
		for(int i =0 ; i < 6; i ++)
		 {
			 move_dirs[i]= direction & move_mask[i];
		 }
		float velocity = MoveSpeed * deltaTime;

		vec3 front = normalize(vec3(Front.x,0.0f,Front.z));
		vec3 right = normalize(cross(front,WorldUp));
		vec3 up    = normalize(cross(Right,Front));
		vec3  movement=vec3(0.0f);
        
		if(move_dirs[0])movement += front * velocity;
        if(move_dirs[1])movement -= front * velocity;
        if(move_dirs[2])movement -= right * velocity;
        if(move_dirs[3])movement += right * velocity;
        if(move_dirs[4])movement += up * velocity;
        if(move_dirs[5])movement -= up * velocity;


		Position += movement;
		if(Style == LOOKINGAT) return;

		LookingAt += movement;
	}
	void ProcessMouseMovement(float xoffset, float yoffset, unsigned char constrainPitch = true)
    {
		if(Style == CONSTANT) return ;
		
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
		
        Yaw   += xoffset;
        Pitch += yoffset;
		
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
	
		
    }
	 void ProcessMouseScroll(float yoffset)
    {
        Angle -= yoffset*AngleSensitivity;
        if (Angle < 1.0f)
            Angle = 1.0f;
        if (Angle > 75.0f)
            Angle = 75.0f; 
    }
	 void UpdataCamera()
	{
		UpdateView();
		LookingTo();
	}
private:
	void UpdateView()
	{
		vec3 front;
        front.z = cos(radians(Yaw)) * cos(radians(Pitch));
        front.y = sin(radians(Pitch));
        front.x = sin(radians(Yaw)) * cos(radians(Pitch));
		Front = normalize(front.x* WorldRight + front.y*WorldUp + front.z*WorldFront);
		LookingAt = cross(WorldUp,WorldFront) ;
	}
		void LookingTo()
	{
		Right = normalize(cross(Front,WorldUp));
		Up    = normalize(cross(Right,Front));
		view = mat4(vec4(Right,0.0f),vec4(Up,0.0f),-vec4(Front,0.0f),vec4(vec3(0.0f),1.0f));
		view = transpose(view);
		view = translate(view,-Position);

		//  std::cout<<Position.x<<" "<<Position.y<<" "<<Position.z<<" "<<std::endl;
	}
};
