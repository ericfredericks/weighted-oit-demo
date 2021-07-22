#include "material.h"
#include <stb_image.h>
#define mID (this->ID)
#define mDiffuse (this->diffuse)
#define mDiffuseMap (this->diffuseMap)
#define mDiffuseMapOn (this->diffuseMapOn)



static int setUpTexture(GLuint*,const char*,const char*,float[4]);



void material_init()
{
	stbi_set_flip_vertically_on_load(1);
}
int material_load(Material *this,Objbin *preObject,int materialIndex,const char *path)
{
	memset(this,0,sizeof(Material));

	mID = materialIndex;
	memcpy(mDiffuse,preObject->diffuse+(materialIndex*3),3*sizeof(float));
	mDiffuse[3] = 1.f;
	
	char *diffuseMapName = preObject->diffuseMaps+preObject->diffuseMapPtrs[materialIndex];
	if (diffuseMapName[0]!='.')
	{
		if (!setUpTexture(&mDiffuseMap,diffuseMapName,path,mDiffuse))
			goto error;
		mDiffuseMapOn = 1;
	}
	return 1;
error:
	free(diffuseMapName);
	material_free(this);
	return 0;
}
void material_free(Material *this)
{
	glDeleteTextures(1,&mDiffuseMap);
}


#undef mID
#undef mDiffuse
#undef mDiffuseMap
#undef mDiffuseMapOn


static int setUpTexture(GLuint (*out),const char *filename,const char *path,float borderColor[4])
{
	int width,height,numChannels;
	unsigned char *data;

	char *stbiFile = malloc(strlen(path)+strlen(filename)+1);
	if (stbiFile == NULL)
		return 0;
	strcpy(stbiFile,path);
	strcat(stbiFile,filename);

	data = stbi_load(stbiFile,&width,&height,&numChannels,0);
	free(stbiFile);
	if (data == NULL)
	{
		printf("stb_image: %s\n",stbi_failure_reason());
		return 0;
	}

	GLenum internalFormat = GL_RGB;
	if (numChannels == 4)
		internalFormat = GL_RGBA;

	glGenTextures(1,out);
	glBindTexture(GL_TEXTURE_2D,(*out));
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,internalFormat,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return 1;
}




