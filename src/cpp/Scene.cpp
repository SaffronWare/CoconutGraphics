#include "Scene.h"

void Scene::random_sphere_scene(unsigned int num_spheres, float minr, float maxr, float domain_size)
{
	spheres.clear();
	planes.clear();

	Plane floor;
	floor.point = Vec3(0.0f, -0.05f, 0.0f);
	floor.normal = Vec3(0.0f, 1.0f, 0.0f);
	planes.push_back(floor);

	

	if (num_spheres > MAX_NUM_SPHERES) { num_spheres = MAX_NUM_SPHERES; }

	Sphere ball;
	bool should_add = false;
	for (int i = 0; i < num_spheres/2u; i++)
	{
		while (!should_add)
		{
			should_add = true;
			ball.radius = (float)std::rand() / RAND_MAX * (maxr - minr) + minr;
			ball.position.x = ((float)std::rand() / RAND_MAX - 0.5f) * domain_size;
			ball.position.z = ((float)std::rand() / RAND_MAX - 0.5f) * domain_size;
			ball.position.y = ball.radius;
			ball.material.color.x = (float)std::rand() / RAND_MAX * 0.5f + 0.5f;
			ball.material.color.y = (float)std::rand() / RAND_MAX * 0.5f + 0.5f;
			ball.material.color.z = (float)std::rand() / RAND_MAX * 0.5f + 0.5f;
			
			for (Sphere& s : spheres)
			{
				if ((s.position - ball.position).length() < (s.radius + ball.radius) * 0.8f)
				{
					should_add = false;
					break;
				}
			}

			if (should_add)
			{
				spheres.push_back(ball);
			}
		}

		should_add = false;
		
	}

	ball.material.specular_probability = 0.15;
	ball.material.roughness = 0.0f;


	for (int i = 0; i < num_spheres / 4u; i++)
	{
		while (!should_add)
		{
			should_add = true;
			ball.radius = (float)std::rand() / RAND_MAX * (maxr - minr) + minr;
			ball.position.x = ((float)std::rand() / RAND_MAX - 0.5f) * domain_size;
			ball.position.z = ((float)std::rand() / RAND_MAX - 0.5f) * domain_size;
			ball.position.y = ball.radius;
			ball.material.color.x = (float)std::rand() / RAND_MAX * 0.5f + 0.5f;
			ball.material.color.y = (float)std::rand() / RAND_MAX * 0.5f + 0.5f;
			ball.material.color.z = (float)std::rand() / RAND_MAX * 0.5f + 0.5f;

			for (Sphere& s : spheres)
			{
				if ((s.position - ball.position).length() < (s.radius + ball.radius) * 0.8f)
				{
					should_add = false;
					break;
				}
			}

			if (should_add)
			{
				spheres.push_back(ball);
			}

		}
		should_add = false;
	}

	ball.material.IOR = 2.0f;
	
	for (int i = 0; i < num_spheres / 4u; i++)
	{
		while (!should_add)
		{
			should_add = true;
			ball.radius = (float)std::rand() / RAND_MAX * (maxr - minr) + minr;
			ball.position.x = ((float)std::rand() / RAND_MAX - 0.5f) * domain_size;
			ball.position.z = ((float)std::rand() / RAND_MAX - 0.5f) * domain_size;
			ball.position.y = ball.radius;
			ball.material.absorbance.x = (float)std::rand() / RAND_MAX * 0.2f + 0.8f;
			ball.material.absorbance.y = (float)std::rand() / RAND_MAX * 0.2f + 0.8f;
			ball.material.absorbance.z = (float)std::rand() / RAND_MAX * 0.2f + 0.8f;

			for (Sphere& s : spheres)
			{
				if ((s.position - ball.position).length() < (s.radius + ball.radius) * 0.8f)
				{
					should_add = false;
					break;
				}
			}

			if (should_add)
			{
				spheres.push_back(ball);
			}
		}
		should_add = false;
	}
}
