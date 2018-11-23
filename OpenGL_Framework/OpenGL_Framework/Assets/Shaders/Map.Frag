#version 420

uniform mat4 uModel;

uniform vec3 uSceneAmbient = vec3(0.2, 0.2, 0.2);
uniform vec3 uLightPosition = vec3(3.0, 0.0, 0.0);
uniform vec3 uLightColor = vec3(1.0, 0.9, 0.8);
uniform float uLightSpecularExponent = 16.0;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

void main()
{
	outColor.rgb = uSceneAmbient; 

	// Fix length after rasterizer interpolates
	vec3 normal = normalize(norm);

	vec3 lightVec = uLightPosition - pos;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	float NdotL = dot(normal, lightDir);

	// If the normal is facing the light
	if (NdotL > 0.0)
	{
		// Normalized vector pointing towards the camera
		vec3 eye = normalize(-pos);
		
		// Calculate attenuation (falloff)
		// Add a small number to avoid divide by zero.
		float attenuation = 1.0 / (1.0 + dist * dist * 0.1);

		// Calculate the diffuse contribution
		outColor.rgb += uLightColor * NdotL * attenuation;
		
		vec3 reflection = reflect(-lightDir, normal);
		
		float specularStrength = dot(reflection, eye);
		specularStrength = max(specularStrength, 0.0f); // don't let it fall before zero

		// Calculate the specular contribution
		outColor.rgb += uLightColor * pow(specularStrength, uLightSpecularExponent) * attenuation;
	}
}