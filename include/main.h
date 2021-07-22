#ifndef MAIN_H
#define MAIN_H

#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#define OBJBIN_LOADER_IMPLEMENTATION
#include "model.h"
#include "shaderprog.h"
#include "cubemap.h"
#include "window.h"




mat4 view;
mat4 projection;



GLuint transPass;
GLuint transAccum;
GLuint transRevealage;
GLuint transDepth;

GLuint screenShader;
GLuint oitScreenShader;

GLuint quadShader;
GLuint oitQuadShader;
GLuint quadVAO;
GLuint quadVBO;
GLuint quadEBO;

mat4 treeModel;
GLuint treeColor;
GLuint treeAlpha;

mat4 paneModel[2];

GLuint oitGridShader;
mat4 gridModel;
GLuint gridColor;
GLuint gridAlpha;




void context_load();
void variable_load();
void init();
void end();
void resize(GLFWwindow*,int,int);



#endif
