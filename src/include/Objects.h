#pragma once
#include "Vector.h"
#include "glad/glad.h"


struct Material
{
    Vec3 color = Vec3(0.7f, 0.7f, 0.7f);
	float emission_strength = 0.0f;
	
    Vec3 emission_color = Vec3(1.0, 1.0, 1.0);
	float roughness = 1.0f;
	

    Vec3 specular_reflectance = Vec3(1.0, 1.0, 1.0);
	float specular_probability = 0.0f;
	
    Vec3 absorbance = Vec3(1.0, 1.0, 1.0);
    float IOR = 0.0; 

	float padding[2];
	float absorbance_strength = 1.0f;
	float mettalic = 0.0f;
	
};



struct Object
{
	Material material;
};

struct Square
{
	Vec3 center;
	float pad1 = 1.0f;
	Vec3 u;
	float pad2 =1.0f;
	Vec3 v;
	float pad3 = 1.0f;

	Material material;
};

struct Sphere
{

	Vec3 position;
	float radius;
	Material material;

};

struct Plane
{
	Vec3 normal;
private:
	float pad1 = 1.0f;
public:
	Vec3 point;
private:
	float pad2 = 1.0f;
public:
	Material material;
};

