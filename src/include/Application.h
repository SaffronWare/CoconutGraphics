#pragma once 
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"



class Application
{
private:
	bool RUNNING = false;
	GLFWwindow* window;
	Renderer renderer;
	Scene scene;
	Camera camera;
public:
	Application() = default;
	void Setup();
	void Run();
	void Finish();
};