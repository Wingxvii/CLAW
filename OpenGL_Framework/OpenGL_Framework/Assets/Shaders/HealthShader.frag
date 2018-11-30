#version 420

uniform float healthAmount;

out vec4 outColor;

void main()
{
    vec4 red = {1.0f, 0.0f. 0.0f, 1.0f};
    vec4 green = {1.0f, 0.0f. 0.0f, 1.0f};
    
	outColor = mix(red, green, healthAmount);
	
}