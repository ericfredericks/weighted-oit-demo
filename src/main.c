#include "main.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


 
int main()
{
	context_load();
	variable_load();
	init();

	while (!glfwWindowShouldClose(window))
	{
		input_loop(&input);
		glm_translate(view,(vec3){-input.xAxisL,0.f,-input.yAxisL});

		/* OPAQUE PASS */
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			glUseProgram(quadShader);
			glUniformMatrix4fv(glGetUniformLocation(quadShader,"uView"),1,GL_FALSE,(GLfloat*)view);
			glUniformMatrix4fv(glGetUniformLocation(quadShader,"uProjection"),1,GL_FALSE,(GLfloat*)projection);
			glBindVertexArray(quadVAO);
			{
				/* OPAQUE PANE */
				/*
				glUniformMatrix4fv(glGetUniformLocation(quadShader,"uModel"),1,GL_FALSE,(GLfloat*)paneModel[1]);
				glUniform1i(glGetUniformLocation(quadShader,"uDiffuseMapOn"),0);
				glUniform1i(glGetUniformLocation(quadShader,"uAlphaMapOn"),0);
				glUniform4fv(glGetUniformLocation(quadShader,"uDiffuse"),1,(GLfloat[4]){0.f,0.f,1.f,1.f});
				glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)0);
				*/
			}
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,transPass);
		glBlitFramebuffer(0,0,800,600,0,0,800,600,GL_DEPTH_BUFFER_BIT,GL_NEAREST);

		/* TRANSPARENT PASS */
		glBindFramebuffer(GL_FRAMEBUFFER,transPass);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glClearBufferfv(GL_COLOR,0,(GLfloat[4]){0.f,0.f,0.f,0.f});
		glClearBufferfv(GL_COLOR,1,(GLfloat[1]){1.f});
		glBlendFunci(0,GL_ONE,GL_ONE);
		glBlendFunci(1,GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
		{
			/* QUADS */
			glUseProgram(oitQuadShader);
			glUniformMatrix4fv(glGetUniformLocation(oitQuadShader,"uView"),1,GL_FALSE,(GLfloat*)view);
			glUniformMatrix4fv(glGetUniformLocation(oitQuadShader,"uProjection"),1,GL_FALSE,(GLfloat*)projection);
			glBindVertexArray(quadVAO);
			{
				/* TREE */
				glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,treeColor);
				glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D,treeAlpha);
				glUniformMatrix4fv(glGetUniformLocation(oitQuadShader,"uModel"),1,GL_FALSE,(GLfloat*)treeModel);
				glUniform1i(glGetUniformLocation(oitQuadShader,"uDiffuseMapOn"),1);
				glUniform1i(glGetUniformLocation(oitQuadShader,"uAlphaMapOn"),1);
				glUniform1i(glGetUniformLocation(oitQuadShader,"uDiffuseMap"),0);
				glUniform1i(glGetUniformLocation(oitQuadShader,"uAlphaMap"),1);
				glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)0);

				/* TRANSPARENT PANE */
				glUniformMatrix4fv(glGetUniformLocation(oitQuadShader,"uModel"),1,GL_FALSE,(GLfloat*)paneModel[0]);
				glUniform1i(glGetUniformLocation(oitQuadShader,"uDiffuseMapOn"),0);
				glUniform1i(glGetUniformLocation(oitQuadShader,"uAlphaMapOn"),0);
				glUniform4fv(glGetUniformLocation(oitQuadShader,"uDiffuse"),1,(GLfloat[4]){1.f,0.f,0.f,0.5f});
				glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)0);
				glUniformMatrix4fv(glGetUniformLocation(oitQuadShader,"uModel"),1,GL_FALSE,(GLfloat*)paneModel[1]);
				glUniform4fv(glGetUniformLocation(oitQuadShader,"uDiffuse"),1,(GLfloat[4]){1.f,1.f,0.f,0.5f});
				glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)0);
			}

			/* GRID */
			glUseProgram(oitGridShader);
			glUniformMatrix4fv(glGetUniformLocation(oitGridShader,"uView"),1,GL_FALSE,(GLfloat*)view);
			glUniformMatrix4fv(glGetUniformLocation(oitGridShader,"uProjection"),1,GL_FALSE,(GLfloat*)projection);
			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,gridColor);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D,gridAlpha);
			glUniformMatrix4fv(glGetUniformLocation(oitGridShader,"uModel"),1,GL_FALSE,(GLfloat*)gridModel);
			glUniform1i(glGetUniformLocation(oitGridShader,"uDiffuseMap"),0);
			glUniform1i(glGetUniformLocation(oitGridShader,"uAlphaMap"),1);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)0);
		}
		if (input.lMouseClick)
		{
			/* VIEW TRANSPARENT PASS */
			glBindFramebuffer(GL_FRAMEBUFFER,0);
			glDisable(GL_BLEND);
			glUseProgram(screenShader);
			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,transAccum);
			glUniform1i(glGetUniformLocation(screenShader,"uTexture"),0);
			glBindVertexArray(quadVAO);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)0);
		}
		else
		{
			/* COMPOSITE PASS */
			glBindFramebuffer(GL_FRAMEBUFFER,0);
			glDepthMask(GL_TRUE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glUseProgram(oitScreenShader);
			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,transAccum);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D,transRevealage);
			glUniform1i(glGetUniformLocation(oitScreenShader,"uAccumTexture"),0);
			glUniform1i(glGetUniformLocation(oitScreenShader,"uRevealageTexture"),1);
			glBindVertexArray(quadVAO);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)0);
		}

		int i;
		while ((i = glGetError()) != GL_NO_ERROR)
			printf("GL: %x\n",i);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	end();
	return EXIT_SUCCESS;
}
void context_load()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,0);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
	if ((window = glfwCreateWindow(800,600,"gl",NULL,NULL)) == NULL)
		exit(EXIT_FAILURE);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		exit(EXIT_FAILURE);
}

void variable_load()
{
	/* SHADERS */
	if (!(oitQuadShader = shaderprog_load("asset/shd/quad.vert","asset/shd/quad_oit.frag")))
		exit(EXIT_FAILURE);
	if (!(oitScreenShader = shaderprog_load("asset/shd/oit_screen.vert","asset/shd/oit_screen.frag")))
		exit(EXIT_FAILURE);
	if (!(quadShader = shaderprog_load("asset/shd/quad.vert","asset/shd/quad.frag")))
		exit(EXIT_FAILURE);
	if (!(screenShader = shaderprog_load("asset/shd/screen.vert","asset/shd/screen.frag")))
		exit(EXIT_FAILURE);
	if (!(oitGridShader = shaderprog_load("asset/shd/grid.vert","asset/shd/grid_oit.frag")))
		exit(EXIT_FAILURE);
	


	/* TEXTURES */
	stbi_set_flip_vertically_on_load(1);
	int w,h,n;
	unsigned char *data;
	if ((data = stbi_load("asset/tex/tree_color.png",&w,&h,&n,0)) == NULL)
		exit(EXIT_FAILURE);
	glGenTextures(1,&treeColor);
	glBindTexture(GL_TEXTURE_2D,treeColor);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	if ((data = stbi_load("asset/tex/tree_alpha.png",&w,&h,&n,0)) == NULL)
		exit(EXIT_FAILURE);
	glGenTextures(1,&treeAlpha);
	glBindTexture(GL_TEXTURE_2D,treeAlpha);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_R8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	if ((data = stbi_load("asset/tex/grid_color.png",&w,&h,&n,0)) == NULL)
		exit(EXIT_FAILURE);
	glGenTextures(1,&gridColor);
	glBindTexture(GL_TEXTURE_2D,gridColor);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	if ((data = stbi_load("asset/tex/grid_alpha.png",&w,&h,&n,0)) == NULL)
		exit(EXIT_FAILURE);
	glGenTextures(1,&gridAlpha);
	glBindTexture(GL_TEXTURE_2D,gridAlpha);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_R8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}
void init()
{
	/* FRAMEBUFFERS */
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	{
		glClearColor(0.8f,0.8f,0.8f,1.f);
	}
	glGenFramebuffers(1,&transPass);
	glBindFramebuffer(GL_FRAMEBUFFER,transPass);
	{	
		glGenRenderbuffers(1,&transDepth);
		glBindRenderbuffer(GL_RENDERBUFFER,transDepth);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,800,600);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,transDepth);

		glGenTextures(1,&transAccum);
		glBindTexture(GL_TEXTURE_2D,transAccum);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,800,600,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,transAccum,0);

		glGenTextures(1,&transRevealage);
		glBindTexture(GL_TEXTURE_2D,transRevealage);
		glTexImage2D(GL_TEXTURE_2D,0,GL_R8,800,600,0,GL_RED,GL_UNSIGNED_BYTE,NULL);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,transRevealage,0);

		glDrawBuffers(2,(GLenum[2]){GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1});

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("GL: %i\n",glCheckFramebufferStatus(GL_FRAMEBUFFER));
			exit(EXIT_FAILURE);
		}
	}



	/* VAOS */
	float quadVertices[] = {
		1.f,1.f,0.f,1.f,1.f,
		-1.f,-1.f,0.f,0.f,0.f,
		1.f,-1.f,0.f,1.f,0.f,
		-1.f,1.f,0.f,0.f,1.f
	};
	unsigned int quadIndices[] = {
		0,2,1,
		0,1,3
	};
	glGenVertexArrays(1,&quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1,&quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glGenBuffers(1,&quadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(quadIndices),quadIndices,GL_STATIC_DRAW);



	/* MATRICES */
	glm_mat4_identity(treeModel);
	glm_scale(treeModel,(vec3){20.f,20.f,0.f});
	glm_translate(treeModel,(vec3){0.f,1.f,0.f});
	glm_mat4_identity(paneModel[0]);
	glm_translate(paneModel[0],(vec3){0.f,0.f,30.f});
	glm_scale(paneModel[0],(vec3){10.f,10.f,0.f});
	glm_translate(paneModel[0],(vec3){0.f,1.f,0.f});
	glm_mat4_identity(paneModel[1]);
	glm_translate(paneModel[1],(vec3){0.f,0.f,60.f});
	glm_scale(paneModel[1],(vec3){10.f,10.f,0.f});
	glm_translate(paneModel[1],(vec3){0.f,1.f,0.f});
	glm_mat4_identity(gridModel);
	glm_scale(gridModel,(vec3){1000.f,0.f,1000.f});
	glm_rotate(gridModel,glm_rad(90.f),(vec3){1.f,0.f,0.f});	

	glm_mat4_identity(view);
	glm_translate(view,(vec3){0.f,0.f,-50.f});
	glm_rotate(view,glm_rad(10.f),(vec3){1.f,0.f,0.f});
	glm_perspective(glm_rad(45.f),800.f/600.f,0.1f,1000.f,projection);



	input_init(&input);
}
void end()
{
	/* VAOS */
	glDeleteVertexArrays(1,&quadVAO);
	glDeleteBuffers(1,&quadVBO);
	glDeleteBuffers(1,&quadEBO);

	/* TEXTURES */
	glDeleteTextures(1,&treeColor);
	glDeleteTextures(1,&treeAlpha);

	/* SHADERS */
	glDeleteProgram(quadShader);
	glDeleteProgram(oitQuadShader);
	glDeleteProgram(screenShader);
	glDeleteProgram(oitScreenShader);

	/* FRAMEBUFFERS */
	glDeleteTextures(1,&transAccum);
	glDeleteTextures(1,&transRevealage);
	glDeleteRenderbuffers(1,&transDepth);
	glDeleteFramebuffers(1,&transPass);

	glfwTerminate();
}
void resize(GLFWwindow *window,int width,int height)
{
	glViewport(0,0,width,height);
}
