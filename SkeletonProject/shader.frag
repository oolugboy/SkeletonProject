#version 330 core
out vec4 color;

// The input values
in vec3 outNormal;
in vec3 outPosition;
void main()
{
	vec3 lightColor1 = vec3(1.0f, 1.0f, 0.0f);
	vec3 lightColor2 = vec3(1.0f, 0.0f, 0.0f);
	//vec3 objectColor = vec3(0.184f, 0.310f, 0.310f);
	vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

	vec3 lightOnePos = vec3(10.0f, 10.0f, -10.0f);
	vec3 lightTwoPos = vec3(-10.0f, 10.0f, -10.0f);
	
	float ambientStrength = 0.1f;
	vec3 ambient = (ambientStrength * lightColor1) + (ambientStrength * lightColor2);
	vec3 result = ambient * objectColor;

	vec3 lightOneDir = normalize(lightOnePos - outPosition);
	vec3 lightTwoDir = normalize(lightTwoPos - outPosition);

	float diff1 = max(dot(outNormal, lightOneDir), 0.0);
	float diff2 = max(dot(outNormal, lightTwoDir), 0.0);

	vec3 diffuse = (diff1 * lightColor1) + (diff2 * lightColor2);

	result += diffuse;
	color = vec4(result, 1.0f);
}