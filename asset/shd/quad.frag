#version 330 core

in vec2 texCoord;
uniform int uDiffuseMapOn;
uniform int uAlphaMapOn;
uniform sampler2D uDiffuseMap;
uniform sampler2D uAlphaMap;
uniform vec4 uDiffuse;
out vec4 FragColor;

void main()
{
	FragColor = (1-uDiffuseMapOn) * uDiffuse;
	FragColor += uDiffuseMapOn * uAlphaMapOn * vec4(texture(uDiffuseMap,texCoord).rgb,texture(uAlphaMap,texCoord).r);
	FragColor += uDiffuseMapOn * (1-uAlphaMapOn) * texture(uDiffuseMap,texCoord);
}
