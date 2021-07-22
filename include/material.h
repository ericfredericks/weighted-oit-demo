#ifndef MATERIAL_H
#define MATERIAL_H
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <objbin_loader.h>



typedef struct
{
	int ID;
	GLuint diffuseMap;
	int diffuseMapOn;
	float diffuse[4];
} Material;
void material_init();
int material_load(Material*,Objbin*,int,const char*);
void material_free(Material*);



#endif
