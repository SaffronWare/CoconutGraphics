

#version 330 core

struct Material
{
	vec3 color;
	float emission_strength;
	
	vec3 emission_color;
    float roughness;

	vec3 specular_reflectance;
    float specular_probability;

	vec3 absorbance;
    float IOR; 

	vec3 padding;
	float metallic;
	

};

struct Sphere {
    vec3 center;
    float radius;
	//float padding[3];
	Material material;
};

struct Plane
{
	vec3 normal;
	float pad1;
	vec3 point;
	float pad2;

	Material material;
};

struct Square
{
	vec3 center;
	float pad1;
	vec3 u;
	float pad2;
	vec3 v;
	float pad3;
	
	Material material;
};



const float max_dist = 30000;
const int num_spheres = 10;
const int num_planes = 10;
const int num_squares =5;
const int num_biconvex_spheres = 3;

const int num_bounces = 10;
const float normal_offset = 0.01f;
const int num_samples = 4;


const vec3 cLightDir = vec3(1,1,1);

uniform float u_AspectRatio;
uniform vec3 u_CameraPosition;
uniform vec3 u_CameraFront;
uniform vec3 u_CameraRight;
uniform vec3 u_CameraUp;
uniform uint u_FrameCount;
uniform bool u_RenderMode;
uniform float u_AliasingBlur;
uniform sampler2D prevRender;

layout(std140) uniform PlaneData 
{
	Plane planes[num_planes];
};

layout(std140) uniform SphereData {
    Sphere spheres[num_spheres]; 
};

layout(std140) uniform SquareData {
	Square squares[num_squares];
};


in vec3 fPos;
out vec4 FragColor;

//uniform sampler2D PrevRender;

uint seed = 0u;
uint pcg_hash()
{
    uint state = seed * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	seed =  (word >> 22u) ^ word;
	seed *= u_FrameCount;
    return seed;
}

float rand()
{
	return float(pcg_hash()) / float(0xFFFFFFFFu);
	
}

float NormalRandDist()
{
	float theta = 2 * 3.141592653589f * rand();
	float rho = sqrt(-2 * log(rand()));
	return rho * cos(theta);
}

vec3 randomOnHemisphere(vec3 normal)
{
	vec3 dir = normalize(vec3(NormalRandDist(), NormalRandDist(), NormalRandDist()));
	if (dot(dir, normal) < 0) {dir = -dir;}
	return dir;
}

struct HitData
{
	bool hit;
	float dist;
	vec3 normal;
	vec3 point;
	Material mat;
};

struct Ray 
{
	vec3 pos; 
	vec3 dir;
	float IOR;

};





vec3 at(Ray ray, float t)
{
	return ray.pos + t * ray.dir;
}

vec3 correct_normal(vec3 normal, vec3 dir)
{
	if (dot(normal,dir) < 0)
	{
		return normal;
	} else {return -normal;}
}



HitData sphereInterection(Sphere sphere, Ray ray)
{
	HitData hit;
	hit.hit = false;
	hit.mat = sphere.material;
	

	vec3 k = ray.pos - sphere.center;
	float b = 2.0 * dot(ray.dir, k);
	float c = dot(k,k) - sphere.radius * sphere.radius;

	float disc = b * b - 4 * c;
	
	if (disc < 0.0f)
	{
		return hit;
	}

	float d  = sqrt(disc);

	if (d == 0.0f && b < 0.0f)
	{
		hit.hit = true;
		hit.dist = -b * 0.5f;
		
	} else 
	{

		float t1 = 0.5 * (-b - d);

		if (t1 >= 0)
		{
			hit.hit = true;
			hit.dist = t1;
			
		} else 
		{

			float t2 = 0.5 * (-b + d);

			if (t2 >= 0)
			{
				hit.hit = true;
				hit.dist = t2;
			}
		}
	}
	hit.point = at(ray, hit.dist);
	hit.normal = correct_normal(normalize(hit.point - sphere.center),ray.dir);
	
	return hit;
};



HitData planeIntersection(Plane plane, Ray ray)
{
	HitData hit;
	hit.mat = plane.material;
	hit.hit = false;
	
	float dotp = dot(ray.dir, plane.normal);

	if (dotp == 0 || dot(ray.pos - plane.point, plane.normal) == 0) {return hit;}

	hit.dist = -dot(ray.pos - plane.point, plane.normal) / dotp;
	hit.hit = (hit.dist > 0);
	hit.normal = correct_normal(plane.normal, ray.dir);
	hit.point = at(ray, hit.dist);

	return hit;
}

HitData squareIntersection(Square square, Ray ray)
{
	HitData hit;
	hit.hit = false;
	hit.mat = square.material;
	hit.normal = correct_normal(cross(square.u, square.v),ray.dir);

	Plane plane;
	plane.normal = hit.normal;
	plane.point = square.center;

	HitData temp = planeIntersection(plane, ray);
	if (temp.hit == false) {return hit;}
	//else {hit.hit = true; hit.dist = temp.dist; return hit;}
	
	vec3 p = (temp.point-square.center);
	
	float x = dot(p,square.u);
	float y = dot(p,square.v);

	if (abs(x) > dot(square.u, square.u) || abs(y) > dot(square.v, square.v))
	{
		return hit;
	}
	
	hit.hit = true;
	hit.dist = temp.dist;
	hit.point = temp.point;
	


	return hit;
}


HitData closestHit(Ray ray)
{
	HitData hit;
	HitData curr_hit;
	hit.dist = max_dist;

	for (int i  = 0; i < num_spheres; i++)
	{
		if (spheres[i].radius == 0) {break;}
		curr_hit = sphereInterection(spheres[i], ray);

		if (curr_hit.hit && (curr_hit.dist < hit.dist))
		{
				hit = curr_hit;
		}
	}

	for (int i = 0; i < num_planes; i++)
	{
		if (planes[i].pad1  == 0.0) {break;}
		curr_hit = planeIntersection(planes[i], ray);

		if (curr_hit.hit && (curr_hit.dist < hit.dist))
		{
				hit = curr_hit;
		}
	}

	for (int i = 0; i < num_squares; i++)
	{
		if (squares[i].pad1 == 0.0) {break;}

		curr_hit = squareIntersection(squares[i], ray);

		if (curr_hit.hit && (curr_hit.dist < hit.dist))
		{
				hit = curr_hit;
		}
	}

	return hit;
}

vec3 get_color(Ray ray)
{
	vec3 out_color = vec3(0,0,0);
	vec3 ray_color = vec3(1,1,1);

	if (!u_RenderMode)
	{

		for (int i = 0; i <= num_bounces; i++)
		{
			HitData hit = closestHit(ray);

			if (dot(ray_color,ray_color) < 0.05) {break;}
			
			if (hit.hit)
			{	
				//seed += uint(num_bounces);
			
				vec3 specular = ray.dir - 2.0f * hit.normal * dot(ray.dir, hit.normal);

				if (hit.mat.IOR < 1)
				{
					ray.pos = hit.point + hit.normal * normal_offset;
					vec3 diffuse = normalize(hit.normal + randomOnHemisphere(hit.normal));

				
					bool isSpecular = (rand() < hit.mat.specular_probability);
					float s = (1-hit.mat.roughness) * float(isSpecular); //woah...
	

					ray.dir = diffuse * (1-s) * (1-hit.mat.metallic) + specular * s;
					ray.dir = normalize(ray.dir);

					out_color += ray_color * hit.mat.emission_color * hit.mat.emission_strength;
					ray_color *= hit.mat.color * (1-float(isSpecular)) + hit.mat.specular_reflectance * float(isSpecular);
				}
				else 
				{
			
					vec3 normal = -hit.normal;
					vec3 k = normalize(cross(normal,cross(normal, ray.dir)));

					vec3 v = ray.dir;

					float n1 = ray.IOR;
					float n2 = hit.mat.IOR;

					bool should_beer = false;

					if (n2 == n1) {n2 = 1.0f; should_beer = true;}
					ray.IOR = n2;

					float mu = n1 / n2;
					float kComp = mu * dot(v,k);
					float discriminant_for_refraction = kComp * kComp;

					if (abs(discriminant_for_refraction) < 1)
					{
						ray.pos = hit.point + normal * normal_offset;

						float nComp = sqrt(1 - discriminant_for_refraction);

						vec3 refracted = k * kComp + normal * nComp;

						float N = dot(v,normal);

						float Rsn = n1 * N - n2 * nComp;
						float Rsd = n1 * N + n2 * nComp;

						float Rs = abs(Rsn / Rsd); Rs *= Rs;

						float Rpn = n1 * nComp - n2 * N;
						float Rpd = n1 * nComp + n2 * N;

						float Rp = abs(Rpn / Rpd); Rp *= Rp;

						float R = Rp + Rs; R *= 0.5;
						//return refracted;

						if (rand() > 1.0f * R) // i didnt like how weird reflections looked
						{
							ray.dir = refracted;
						}
						else {
							ray.IOR = n1;
							ray.pos = hit.point + hit.normal * normal_offset;
							ray.dir = specular;
							ray_color *= hit.mat.specular_reflectance;
						}
				
					} 
					else 
					{
						ray.pos = hit.point + hit.normal * normal_offset;
						ray.dir = specular;
						ray_color *= hit.mat.specular_reflectance;
						ray.IOR = n1;
					}

					if (should_beer)
					{
							ray_color.x *= exp(-hit.dist * (1.0f - hit.mat.absorbance.x));
							ray_color.y *= exp(-hit.dist * (1.0f - hit.mat.absorbance.y));
							ray_color.z *= exp(-hit.dist * (1.0f - hit.mat.absorbance.z));
					}
				}
			}
			else 
			{
			}

		}
	}
	else 
	{
		HitData hit = closestHit(ray);
		out_color = hit.mat.color * max(0.2f, 0.5f * (1 + dot(hit.normal, cLightDir)));;
	}

	return out_color;

}

vec4 gamma(vec4 col)
{	
	return sqrt(col);
}

void main()
{
	int caseOfAntialiasing = int(mod(u_FrameCount, 4));
	
	

	

	vec2 uv = (vec2(fPos.x, fPos.y) + vec2(1,1)) / 2;

	Ray ray;
	ray.IOR = 1.0f;
	vec3 out_color;
	
	float x = fPos.x;
	float y = fPos.y;


	seed = uint(101239 * fract(sin(abs(x * 0.12394128f + y * 923.2321878f + float(u_FrameCount+1u) * 10.964f))));


	uint amod = uint(mod(u_FrameCount, 2));



	for (int i = 0; i < num_samples; i++)
	{
		vec3 offset = vec3(0,0,0);
		if (!u_RenderMode)
		{
		switch (i)
			{
				case 0:
					offset = vec3(-u_AliasingBlur,-u_AliasingBlur,0);
					break;
				case 1:
					offset = vec3(u_AliasingBlur,u_AliasingBlur,0);
					break;
				case 2:
					offset = vec3(-u_AliasingBlur, u_AliasingBlur,0);
				case 3: 
					offset = vec3(u_AliasingBlur, -u_AliasingBlur, 0); 
			}
		}
		float x_ = fPos.x * u_AspectRatio + offset.x;
		float y_ = fPos.y + offset.y;
		vec3 pixel_pos = u_CameraFront + x_*u_CameraRight + y_*u_CameraUp;
		ray.IOR = 1.0f;
		ray.pos = u_CameraPosition;
		ray.dir = normalize(pixel_pos );

		out_color += get_color(ray);
	}

	out_color /= float(num_samples);

	
	if (!u_RenderMode)
	{
	
	FragColor = 1.0f * (gamma(vec4(out_color, 1)) + float(u_FrameCount) * texture(prevRender, uv)) / float(u_FrameCount + 1u);

	}
	else 
	{
		FragColor = gamma(vec4(out_color,1));
	}
}