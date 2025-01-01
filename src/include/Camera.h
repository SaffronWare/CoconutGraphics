#pragma once
#define _USE_MATH_DEFINES
#include "Vector.h"
#include <math.h>


#include "Vector.h"

struct Camera
{
private:
	Vec3 up;
	Vec3 right;
	Vec3 front;
	float ViewportDist;
	float aspect_ratio = 1.0f;
public:
	float speed = 12.0f;
	float sensitivity = 0.25f;

	Vec3 position = Vec3(0,0,0);
	float Rotation_X = 0.0f; float Rotation_Y = 0.0f;
	float FOV = 1.0f;

	float blur = 0.0f;
	float SkyboxIntensity = 1.0f;
	float SkyboxContrast = 1.0f;

	bool isConnectedToCursor = false;
	bool RenderMode = true; // kinda weird but true is for the preview renderer and false is for the actual renderer

	

	Camera() = default;

	Camera(unsigned int width, unsigned int height, float FOV)
	{
		FOV = FOV;
		aspect_ratio = (float)width / (float)height;
		updateDirs();
		updateViewportDist();
	}

	void changeAspectRatio(float aspect_ratio)
	{
		this->aspect_ratio = aspect_ratio;
		updateViewportDist();
	}

	float getAspectRatio() const
	{
		return aspect_ratio;
	}

	void updateDirs()
	{
		front = Vec3::from_spherical(Rotation_X, Rotation_Y);
		up = Vec3::from_spherical(Rotation_X, Rotation_Y + 0.5f * (float)M_PI);
		right = Vec3::from_spherical(Rotation_X + 0.5f * (float)M_PI, 0.0);
	}

	void updateViewportDist()
	{
		ViewportDist = aspect_ratio / std::tanf(0.5f * FOV);
	}

	Vec3 getUp() const  { return up; }
	Vec3 getFront() const { return front; }
	Vec3 getRight() const { return right; }
	Vec3 getViewportFront() const { return front * ViewportDist; }

	const float GetViewportDist()
	{
		return ViewportDist;
	}


};