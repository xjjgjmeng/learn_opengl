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
	vec3 direction;
	int type;
	bool attenuation;
	bool softEdges;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform bool showSmile;
uniform Material material;
uniform Light light;

void main()
{
	const int directionalLight = 0;
	const int pointLight = 1;
	const int spotLight = 2;

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

	vec3 lightDir;
	switch (light.type)
	{
	case directionalLight:
		lightDir = normalize(-light.direction);
		break;
	case pointLight:
	case spotLight:
		lightDir = normalize(light.position - FragPos);
		break;
	}

	if ((spotLight == light.type) && !light.softEdges)
	{
		float theta = dot(lightDir, normalize(-light.direction));
		if(theta < light.cutOff)
		{
			FragColor = vec4(ambient, 1.0);
			return;
		}
		
	}

	// diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// specular
	//float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	if ((light.type == spotLight) && light.softEdges)
	{
		// spotlight (soft edges)
		float theta = dot(lightDir, normalize(-light.direction)); 
		float epsilon = (light.cutOff - light.outerCutOff);
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		diffuse  *= intensity;
		specular *= intensity;
	}

	if (((light.type == pointLight)||(light.type == spotLight)) && light.attenuation)
	{
		float distance = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

		// remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
		if (light.type == pointLight)
		{
			ambient *= attenuation;
		}
		diffuse *= attenuation;
		specular *= attenuation;
	}

	//ragColor = vec4((ambient+diffuse+specular) * FragColor.xyz, 1.0);
	FragColor = vec4(ambient+diffuse+specular, 1.0);
}