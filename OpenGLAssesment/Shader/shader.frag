#version 440 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos; 

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;  
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{    
	float specularStrength = 1.0;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

	vec3 result = (ambient + diffuse + specular) * vec3(texture(texture_diffuse1, TexCoords));
	FragColor = vec4(result, 1.0);
}