#pragma once



#include "OpenGLContext.h"
#include "ImGuiRenderer.h"
#include "Scene.h"


class Renderer
{
private:
	OpenGLContext GLcontext;
	ImGuiRenderer imgui_renderer;
	double previous_Time = 0.0;
	double current_Time = 0.0;

	

public:
	Renderer() = default;
	unsigned int getFPS();
	void Initialize(GLFWwindow* window, Scene* scene, Camera* camera);
	void Finish();
	void NewFrame(GLFWwindow* window, const Scene& scene, unsigned int fps);
	void reset();
};