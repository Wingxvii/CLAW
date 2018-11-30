#version 420

uniform float healthAmount;

out vec4 outColor;

void main()
{
    vec4 red = vec4(1,0,0,1);
    vec4 green = vec4(0,1,0,1);;
    
	outColor = mix(red, green, healthAmount);
	
}