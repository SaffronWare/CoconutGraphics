#pragma once
#include <cstdlib>
#include <iostream>


#include "glad/glad.h"
#include <glfw3.h>

#include "Shader.h"
#include "Scene.h"
#include "Camera.h"
#include "HDRloader.h"

#ifdef ASSET_DIR
#endif


struct UNIFORMLOCS
{
	
	GLuint CameraPosition;
	GLuint CameraFront;
	GLuint CameraRight;
	GLuint CameraUp;
	GLuint AspectRatio;
	GLuint RenderMode;
	GLuint Aliasing_Blur;
	GLuint FrameCount;
	GLuint SkyboxIntensity;
	GLuint SkyboxContrast;

	GLuint sphereBuffer;
	GLuint planeBuffer;
	GLuint squareBuffer;
	GLuint skybox;
	GLuint prevRender;

	void Initialize(GLuint program);
};


class OpenGLContext
{
private:
	GLfloat vertices_on_viewport[18] = {
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	

	Shader shader;
	GLuint Viewport_VBO;
	GLuint Viewport_VAO;
	GLuint Viewport_FBO;
	GLuint Viewport_Texture;
	GLuint Viewport_RBO;

	GLuint Skybox_Texture;


	GLuint SphereUBO;
	GLuint PlaneUBO;
	GLuint SquareUBO;


	UNIFORMLOCS LOCS;
public:
	unsigned int frame_count = 0;
	OpenGLContext() = default;

	void Initialize();
	void EndContext();

	void Resize(unsigned int viewport_width, unsigned int viewport_height);

	void PassCamera(const Camera& camera);

	void UseProgram();
	void EndUseProgram();

	void OnSphereBufferChange(const Scene& scene);
	void OnPlaneBufferChange(const Scene& scene);
	void OnSquareBufferChange(const Scene& scene);
	


	GLuint RenderToViewport();
};