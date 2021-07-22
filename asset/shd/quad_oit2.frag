#version 330 core

in vec2 texCoord;
in vec4 gl_FragCoord;
uniform sampler2D uAccumTexture;
uniform sampler2D uRevealageTexture;
out vec4 FragColor;

float maxComponent(vec4 v)
{
	return max(max(max(v.x,v.y),v.z),v.w);
}

void main()
{
	ivec2 C = ivec2(gl_FragCoord.xy);
	float revealage = texelFetch(uRevealageTexture,C,0).r;
	if (revealage == 1.0)
		discard;

	vec4 accum = texelFetch(uAccumTexture,C,0);
	if (isinf(maxComponent(abs(accum))))
		accum.rgb = vec3(accum.a);

	vec3 averageColor = accum.rgb / max(accum.a,0.00001);
	FragColor = vec4(averageColor,1.0-revealage);	
}
