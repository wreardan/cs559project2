#include "Framebuffer.h"

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>

using namespace std;

Framebuffer::Framebuffer(void)
{
}
Framebuffer::~Framebuffer(void)
{
}

void Framebuffer::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
}

void Framebuffer::Disable()
{
	//Unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Borrowed heavily from GLSL cookbook
//https://github.com/daw42/glslcookbook/blob/master/chapter04/scenerendertotex.cpp
bool Framebuffer::Initialize(int width, int height)
{
	GLuint depthBuf, posTex, normTex, colorTex;
	this->width = width;
	this->height = height;

    // Create and bind the FBO
	glGenFramebuffers(1, &renderFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);

    // The depth buffer
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // The position buffer
    glActiveTexture(GL_TEXTURE0);   // Use texture unit 0
    glGenTextures(1, &posTex);
    glBindTexture(GL_TEXTURE_2D, posTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // The normal buffer
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &normTex);
    glBindTexture(GL_TEXTURE_2D, normTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // The color buffer
    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Attach the images to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

    GLenum drawBuffers[] = {GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(4, drawBuffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}


//Taken from GLSL cookbook Chapter 7
//https://github.com/daw42/glslcookbook/blob/master/chapter07/sceneshadowmap.cpp
bool Framebuffer::InitializeShadowMap(int width, int height)
{
	this->width = width;
	this->height = height;

    GLfloat border[] = {1.0f, 0.0f,0.0f,0.0f };
    // The depth buffer texture
	GLuint & depthTex = this->depthBuf;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width,
                 height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);    

    // Assign the depth buffer texture to texture channel 4
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depthTex);

    // Create and set up the FBO
    glGenFramebuffers(1, &renderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, depthTex, 0);

    GLenum drawBuffers[] = {GL_NONE};
    glDrawBuffers(1, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    } else {
        printf("Framebuffer is not complete.\n");
		return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
	return true;
}