#pragma once
#include <string>
#include "Objects.h"


#define MAX_NUM_SPHERES 10
#define MAX_NUM_PLANES 10
#define MAX_NUM_SQUARES 5
#define MAX_NUM_BICONVEX_SPHERES 3

struct Scene
{
	std::string scene_name = "scene";

	std::vector<Sphere> spheres = {};
	std::vector<Plane> planes = {};
	std::vector<Square> squares = {};
	std::vector<BiconvexSphere> biconvexsheres = {};
};