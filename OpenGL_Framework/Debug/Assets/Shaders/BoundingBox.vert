#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;    

out vec3 pos;

void main()
{
	
	vec4 viewSpace = uView * uModel * vec4(in_vert, 1.0f);
	
	gl_Position = uProj * viewSpace;
	
	pos = viewSpace.xyz;
}