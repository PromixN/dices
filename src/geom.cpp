#include <geom.h>
 int check_direction(vec3 v1,vec3 v2,vec3 v3)
{
	float a = glm::dot(v1, v2),
	      b = glm::dot(v2, v3),
		  c = glm::dot(v3, v1);
	return  std::signbit(a) ==std::signbit(b) && std::signbit(b)==std::signbit(c);
}
 float  get_reflect_index(float cos_i,float refract_from,float refract_to)
{
	float relative_refractive_index = refract_to*refract_to / refract_from*refract_from;
	float squre_root= sqrt(relative_refractive_index + 1.0f - cos_i * cos_i);
	return ((cos_i - squre_root) / (cos_i + squre_root) + \
		(relative_refractive_index * cos_i - squre_root) / (relative_refractive_index * cos_i + squre_root))\
		/ 2.0f;
}
 float get_transpare_index(float cos_i,float refract_from,float refract_to)
{
	float relative_refractive_index = refract_to * refract_to / refract_from * refract_from;
	float squre_root = sqrt(relative_refractive_index + 1.0f - cos_i * cos_i);
	return cos_i / (cos_i + squre_root) + refract_to * cos_i / (refract_from * (relative_refractive_index * cos_i + squre_root));
}
 vec3 get_text(float * texture,float x,float y)
 {
	 int index_x= (int)(256*x);
 	 int index_y= (int)(256*y);
 	vec3 color=vec3(0.0f);
 	int x_t=3*index_x*256,y_t=3*index_y;
 	float count=0.0f;
 	for(int i=-20;i<20;i++)
 	{
 		if(index_x+i>255||index_x+i<0)continue;
 		x_t=3*(index_x+i)*256;
 		for(int j=-20;j<20;j++)
 		{
 			if(index_y+j>255||index_y+j<0)continue;
 			count+=1.0f;
 			y_t=3*(index_y+j);
 			color.x+=texture[x_t+y_t];
 			color.y+=texture[x_t+y_t+1];
 			color.z+=texture[x_t+y_t+2];
 		}
 	}
 	if(count<1.0f) return vec3(1.0f);
 	 return color/count;
 }
void set_text(float * texture,float x,float y,vec3 color)
 {
	 x= (int)(floor(256*x));
 	 y= (int)(floor(256*y));

 	int index_x;
 	int index_y;
 	 
 	for(int i=0;i<20;i++)
 	{
 		index_x=x+(i-10);
 		if(index_x>255||index_x<0) break;
 		for (int j=0;j<20;j++)
 		{
 			index_y=y+(j-10);
 			if(index_y>255||index_y<0) break;
			float rate = 1/(sqrt((x-index_x)*(x-index_x)+(y-index_y)*(y-index_y)+1));
 			texture[3*(256*index_x+index_y)]+= color.x*rate;
 			texture[3*(256*index_x+index_y)+1]+= color.y*rate;
 			texture[3*(256*index_x+index_y)+2]+= color.z*rate;
 			if(texture[3*(256*index_x+index_y)]>  0.95f)  texture[3*(256*index_x+index_y)] =0.95f;
			if(texture[3*(256*index_x+index_y)+1]>0.95f)texture[3*(256*index_x+index_y)+1] =0.95f;
			if(texture[3*(256*index_x+index_y)+2]>0.95f)texture[3*(256*index_x+index_y)+2] =0.95f;
 		}
 	}
 }
