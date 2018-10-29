#version 420

uniform vec4 lightPosition;
// color
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
//scalar
uniform float lightSpecularExponent;
uniform float attenuation_Constant;
uniform float attenuation_Linear;
uniform float attenuation_Quadratic;

uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;//position in view space

out vec4 outColor;

void main()
{
	outColor.rgb = lightAmbient;
	//account for rasterizer interpolating 
	vec3 normal = normalize(norm);
	
	//direction to render point to light
	vec3 lightVec = lightPosition.xyz - pos;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;
	
	//normal dot light 
	float NdotL = dot(normal, lightDir);
	
	if(NdotL > 0.0) {
		// the light can contribute to this surface
	
		//Calculate attenuation
		float attenuation = 1.0 / (attenuation_Constant + (attenuation_Linear * dist) + (attenuation_Quadratic * dist * dist));
		
		//Calculate diffuse contribution
		outColor.rgb += lightDiffuse * NdotL * attenuation;
		
		//Blinn-Phong half vector
		float NdotHv =  max(dot(normal, normalize(lightDir + normalize(-pos))), 0.0);
		
		//Calculate specular contribution
		outColor.rgb += lightSpecular * pow(NdotHv, lightSpecularExponent) * attenuation;
		
	}
	
	vec4 textureColor = texture(uTex, texcoord);
	outColor.rgb *= textureColor.rgb;
	outColor.a = textureColor.a;
	
	
}