#pragma once
#include <string>
#include "Objects.h"
#include <vector>
#include <cstdlib>


#define MAX_NUM_SPHERES 50
#define MAX_NUM_PLANES 10
#define MAX_NUM_SQUARES 5


struct Scene
{
	std::string scene_name = "scene";

	void random_sphere_scene(unsigned int num_spheres, float minr, float maxr, float domain_size);

	Sphere empty_spheres[MAX_NUM_SPHERES] = {};
	Plane empty_planes[MAX_NUM_PLANES] = {};
	Square empty_squares[MAX_NUM_SQUARES] = {};

	std::vector<Sphere> spheres = {};
	std::vector<Plane> planes = {};
	std::vector<Square> squares = {};
};