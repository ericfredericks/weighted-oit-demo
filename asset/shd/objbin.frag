#version 330 core

in vec2 texCoord;
in vec3 normal;

uniform sampler2D diffuseMap;
uniform int diffuseMapOn;
uniform vec4 diffuse;

out vec4 FragColor;

float near = 0.1f;
float far = 1000.f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	FragColor = diffuse*(1-diffuseMapOn) + texture(diffuseMap,texCoord)*diffuseMapOn;
/*
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	FragColor = vec4(vec3(depth),1.f);
*/
}
