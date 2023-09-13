#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

// texture samplers
//uniform sampler2D texture1;
//uniform sampler2D texture2;



struct Material
{
	//vec3 ambient;
	//vec3 diffuse;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 viewPos;
uniform bool showSmile;
uniform Material material;
uniform Light light;

void main()
{
	if (showSmile)
	{
		// linearly interpolate between both textures (80% container, 20% awesomeface)
		//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	}
	else
	{
		//FragColor = texture(texture1, TexCoord);
	}

	//FragColor = vec4(lightColor * FragColor.rgb, FragColor.a);
	// ambient
	//float ambientStrength = 0.1;
	//vec3 ambient = ambientStrength * lightColor;
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// specular
	//float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	//ragColor = vec4((ambient+diffuse+specular) * FragColor.xyz, 1.0);
	FragColor = vec4(ambient+diffuse+specular, 1.0);
}