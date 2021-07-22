#version 330 core

in vec2 texCoord;

uniform sampler2D colorMap;
uniform sampler2D alphaMap;

out vec4 FragColor;

void main()
{
	FragColor = vec4(texture(colorMap,texCoord).rgb,texture(alphaMap,texCoord).r);
}

