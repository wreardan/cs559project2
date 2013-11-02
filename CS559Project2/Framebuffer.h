#pragma once

class Framebuffer
{
public:
	unsigned int fboHandle; // The handle to the FBO
	unsigned int renderTex; // 
	unsigned int depthBuf; //


	Framebuffer(void);
	~Framebuffer(void);

	bool Initialize(int width, int height);
	void Use();
	void Disable();
	
};

