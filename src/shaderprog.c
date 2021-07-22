#include "shaderprog.h"



static void* fileIntoBuffer(const char*);
static size_t getFileSize(const char*);



GLuint shaderprog_load(const char *vFilename,const char *fFilename)
{
	char *buffer;
	int success;
	char infoLog[512];

	GLint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	buffer = fileIntoBuffer(vFilename);
	if (buffer == NULL)
	{
		printf("Failed to allocate buffer!\n");
		return 0;
	}
	glShaderSource(vertexShader,1,(const GLchar* const*)&buffer,NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
		printf("%s: %s\n",vFilename,infoLog);
		free(buffer);
		return 0;
	}
	free(buffer);

	GLint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	buffer = fileIntoBuffer(fFilename);
	if (buffer == NULL)
	{
		printf("Failed to allocate buffer!\n");
		return 0;
	}
	glShaderSource(fragmentShader,1,(const GLchar* const*)&buffer,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
		printf("%s: %s\n",fFilename,infoLog);
		free(buffer);
		return 0;
	}
	free(buffer);

	GLuint id = glCreateProgram();
	glAttachShader(id,vertexShader);
	glAttachShader(id,fragmentShader);
	glLinkProgram(id);
	glGetProgramiv(id,GL_LINK_STATUS,&success);
	if (!success)
	{
		glGetProgramInfoLog(id,512,NULL,infoLog);
		printf("OpenGL: %s\n",infoLog);
		return 0;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return id;
}



static void* fileIntoBuffer(const char *filename)
{
	FILE *file;
	char *buffer;
	file = fopen(filename,"rb");
	size_t len = getFileSize(filename);
	buffer = calloc(len,1);
	fread(buffer,1,len,file);
	fclose(file);
	return buffer;
}
static size_t getFileSize(const char *filename)
{
	size_t result = 0;
#if defined (_WIN64)
	struct _stat64 st;
	wchar_t filenameB[100];
	mbstowcs(filenameB,filename,100);
	_wstat64(filenameB,&st);
#elif defined (__linux__)
	struct stat st;
	stat(filename,&st);
#endif
	result = (size_t)st.st_size;
	return result;
}



