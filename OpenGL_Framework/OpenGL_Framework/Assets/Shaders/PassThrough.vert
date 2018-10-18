#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;    
layout(location = 1) in vec2 in_uv;      
layout(location = 2) in vec3 in_normal;  

out vec2 texcoord;

void main()
{
	texcoord = in_uv;

	gl_Position = uProj * uView * uModel * vec4(in_vert, 1.0f);
}