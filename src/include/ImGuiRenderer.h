#pragma once

#include "OpenGLContext.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

class ImGuiRenderer
{
private:
	ImVec2 old_viewport_dimensions;
	Camera* camera;
	Scene* scene;

	bool SHOW_DEBUG_CAM = false;

	bool ADDING_SPHERE = false;
	Sphere* sphere_adding;
	float sphere_center[3] = { 0.0f, 0.0f, 0.0f };
	float sphere_radius = 1.0f;

	bool ADDING_PLANE = false;
	Plane* plane_adding;
	float plane_normal[3] = { 0.0f, 0.0f, 0.0f };
	float plane_point[3] = { 0.0f, 0.0f, 0.0f };

	bool ADDING_SQUARE = false;
	Square* square_adding;
	float square_center[3] = { 0.0f, 0.0f, 0.0f };
	float square_u[3] = {0.0f, 0.0f, 0.0f};
	float square_v[3] = { 0.0f, 0.0f, 0.0f };


	bool EDITED_SPHERES = false;
	bool EDITED_PLANES = false;
	bool EDITED_SQUARES = false;


public:
	ImGuiRenderer() = default;
	void Initialize(GLFWwindow* window,Scene* scene, Camera* camera);
	void BeforeRender();
	void AfterRender();
	void CleanUp();

	void RenderViewport(OpenGLContext& context, unsigned int fps);
	void RenderObjectMenus();
	void RenderCamMenu();
	void RenderObjectTreeNodes();

	void RenderLocMatMenu(Material& mat);
};