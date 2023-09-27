#version 330 core
out vec4 FragColor;

in vec3 TexCoord;
in vec3 FragCoord;

// texture samplers
uniform sampler3D texture0;
uniform vec3		camPos;

float stepSize = 0.002;
int MAX_SAMPLES = int(1.732/stepSize);
vec4 rayCasting()
{
	vec4 result = vec4(0.0);
	//vec3 camPosTex = camPos;
	//vec3 dataPos = TexCoord;
	vec3 geomDir = normalize(FragCoord - camPos);
	vec3 dirStep = stepSize*geomDir;
	for (int i = 0; i < MAX_SAMPLES; ++i)
	{
		vec3 dataPos = TexCoord + dirStep * i;
		float sample = texture(texture0, dataPos).r;
        float src_alpha = 0.0f;
        if(sample > 0){
	        src_alpha = 4.682 * sample;
        }
		//float src_alpha = ((1.0/MAX_SAMPLES)*(i));
		//result.rgb+=(vec3(sample,sample,sample)*(1-result.a));
        result.rgb+=src_alpha*(vec3(1,1,1)*(1-result.a));
		result.a+=src_alpha*(1-result.a);
	}
	return result;
}

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);
	//FragColor = texture(texture0, TexCoord);
	//FragColor = FragColor * 200;
	FragColor = rayCasting();
}