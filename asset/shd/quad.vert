#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;
out vec2 texCoord;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos,1.f);
	texCoord = aTexCoord;
}
