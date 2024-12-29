#include "ImGuiRenderer.h"
#ifdef SRC_DIR
#endif



void ImGuiRenderer::Initialize(GLFWwindow* window, Scene* scene, Camera* camera)
{

	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext(IMGUI_HAS_DOCK);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::LoadIniSettingsFromDisk(((std::string)SRC_DIR + (const char*)"/imgui.ini").c_str());

	ImGui::StyleColorsDark();
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330"); 

	this->camera = camera;
	this->scene = scene;
}

void ImGuiRenderer::RenderLocMatMenu(Material& mat)
{
	float s_color[3] = { mat.color.x, mat.color.y, mat.color.z };
	float s_emission_color[3] = { mat.emission_color.x, mat.emission_color.y, mat.emission_color.z };
	float s_absorbance_color[3] = { mat.absorbance.x, mat.absorbance.y, mat.absorbance.z };
	float s_specular_color[3] = { mat.specular_reflectance.x, mat.specular_reflectance.y, mat.specular_reflectance.z };

	ImGui::SliderFloat("roughness", &mat.roughness, 0.0f, 1.0f);
	ImGui::SliderFloat("specular probability", &mat.specular_probability, 0.0f, 1.0f);
	ImGui::SliderFloat("mettalic", &mat.mettalic, 0.0f, 1.0f);
	ImGui::NewLine();

	ImGui::PushItemWidth(200);
	ImGui::ColorPicker3("color", s_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
	ImGui::PopItemWidth();
	ImGui::NewLine();

	ImGui::PushItemWidth(200);
	ImGui::ColorPicker3("specular color", s_specular_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
	ImGui::PopItemWidth();
	ImGui::NewLine();

	ImGui::SliderFloat("emission strength", &mat.emission_strength, 0.0f, 10.0f);
	ImGui::PushItemWidth(200);
	ImGui::ColorPicker3("emission color", s_emission_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
	ImGui::PopItemWidth();
	ImGui::NewLine();

	ImGui::Text("put the IOR to below 1 to make object non-transparent");
	ImGui::SliderFloat("index of refraction", &mat.IOR, 0, 5);

	ImGui::PushItemWidth(200);
	ImGui::ColorPicker3("absorbance color", s_absorbance_color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
	ImGui::PopItemWidth();


	mat.color.x = s_color[0];
	mat.color.y = s_color[1];
	mat.color.z = s_color[2];

	mat.emission_color.x = s_emission_color[0];
	mat.emission_color.y = s_emission_color[1];
	mat.emission_color.z = s_emission_color[2];

	mat.absorbance.x = s_absorbance_color[0];
	mat.absorbance.y = s_absorbance_color[1];
	mat.absorbance.z = s_absorbance_color[2];

	mat.specular_reflectance.x = s_specular_color[0];
	mat.specular_reflectance.y = s_specular_color[1];
	mat.specular_reflectance.z = s_specular_color[2];

}

void ImGuiRenderer::RenderObjectTreeNodes()
{
	ImGui::Begin("Objects Tree");
	//ImGui::Text("Object Tree");
	if (ImGui::TreeNode("Objects"))
	{
		if (ImGui::TreeNode("Spheres"))
		{
			for (int i = 0; i < scene->spheres.size(); i++)
			{	
				if (&(scene->spheres.at(i)) != sphere_adding)
				{
					if (ImGui::TreeNode((std::string("Sphere ") + std::to_string(i)).c_str()))
					{	
	
						//ImGui::Begin("s0");
					
						if (ImGui::Button("delete")) { scene->spheres.erase(scene->spheres.begin() + i); }
						else {

							Sphere& sphere = scene->spheres.at(i);
							Material& mat = sphere.material;

							float s_center[3] = { sphere.position.x, sphere.position.y, sphere.position.z };

							ImGui::InputFloat3("center", s_center, "%.2f");
							ImGui::InputFloat("radius", &sphere.radius);
							ImGui::NewLine();

							RenderLocMatMenu(mat);

							sphere.position.x = s_center[0];
							sphere.position.y = s_center[1];
							sphere.position.z = s_center[2];


							
						}
						EDITED_SPHERES = true;

						ImGui::TreePop();
					}
				}
			}

			ImGui::TreePop();
		}
		

		if (ImGui::TreeNode("Planes"))
		{
			for (int i = 0; i < scene->planes.size(); i++)
			{
				if (&(scene->planes.at(i)) != plane_adding)
				{
					if (ImGui::TreeNode((std::string("Plane ") + std::to_string(i)).c_str()))
					{
						if (ImGui::Button("delete")) { scene->planes.erase(scene->planes.begin() + i); }
						else {
							Plane& plane = scene->planes.at(i);
							Material& mat = plane.material;

							float p_point[3] = { plane.point.x, plane.point.y, plane.point.z };
							float p_normal[3] = { plane.normal.x, plane.normal.y, plane.normal.z };

							ImGui::InputFloat3("point", p_point, "%.2f");
							ImGui::InputFloat3("normal", p_normal, "%.2f");
							ImGui::NewLine();

							RenderLocMatMenu(mat);

							plane.point.x = p_point[0];
							plane.point.y = p_point[1];
							plane.point.z = p_point[2];

							plane.normal.x = p_normal[0];
							plane.normal.y = p_normal[1];
							plane.normal.z = p_normal[2];

							//std::cout << plane.material.color.x;
						}

						EDITED_PLANES = true;

						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Rectangles"))
		{
			for (int i = 0; i < scene->squares.size(); i++)
			{
				if (&(scene->squares.at(i)) != square_adding)
				{
					if (ImGui::TreeNode((std::string("Rect ") + std::to_string(i)).c_str()))
					{
						if (ImGui::Button("delete")) { scene->squares.erase(scene->squares.begin() + i); }
						else
						{
							Square& square = scene->squares.at(i);
							Material& mat = square.material;

							float s_center[3] = {square.center.x, square.center.y, square.center.z};
							float s_u[3] = {square.u.x, square.u.y, square.u.z};
							float s_v[3] = { square.v.x, square.v.y, square.v.z };

							ImGui::InputFloat3("center", s_center, "%.2f");
							ImGui::InputFloat3("Side 1", s_u, "%.2f");
							ImGui::InputFloat3("Side 2", s_v, "%.2f");

							RenderLocMatMenu(mat);

							square.center.x = s_center[0];
							square.center.y = s_center[1];
							square.center.z = s_center[2];
							
							square.u.x = s_u[0];
							square.u.y = s_u[1];
							square.u.z = s_u[2];
							
							square.v.x = s_v[0];
							square.v.y = s_v[1];
							square.v.z = s_v[2];
						}

						EDITED_SQUARES = true;

						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
		
		//if (ImGui::TreeNode("Biconvex Spheres"))
		
		ImGui::TreePop();
	}
	ImGui::End();
}
	
	



void ImGuiRenderer::RenderCamMenu()
{
	ImGui::Begin("Camera Options");

	ImGui::Checkbox("SHOW CAMERA DEBUG", &SHOW_DEBUG_CAM);
	ImGui::NewLine();

	if (SHOW_DEBUG_CAM)
	{
		ImGui::Text("Camera Positition");
		ImGui::Text("X: %.1f, Y:%.1f, Z:%.1f", camera->position.x, camera->position.y, camera->position.z);
		ImGui::NewLine();

		ImGui::Text("Camera Angles");
		ImGui::Text("Horizontal angle : %.2f radians", camera->Rotation_X);
		ImGui::Text("Vertical angle : %.2f radians", camera->Rotation_Y);
		ImGui::NewLine();

		ImGui::Text("Viewport Distance : %.2f", camera->GetViewportDist());
		ImGui::NewLine();

		ImGui::Text("Camera Vectors");
		ImGui::Text("FRONT | x : %.2f, y : %.2f, z : %.2f", camera->getFront().x, camera->getFront().y, camera->getFront().z);
		ImGui::Text("RIGHT | x : %.2f, y : %.2f, z : %.2f", camera->getRight().x, camera->getRight().y, camera->getRight().z);
		ImGui::Text("UP | x : %.2f, y : %.2f, z : %.2f", camera->getUp().x, camera->getUp().y, camera->getUp().z);
		ImGui::NewLine();

		if (ImGui::Button("RESET CAMERA POSITITON"))
		{
			camera->position = Vec3(0, 0, 0);
			camera->Rotation_X = 0.0f;
			camera->Rotation_Y = 0.0f;
		}
	}
	
	ImGui::SliderFloat("Sensitivity", &camera->sensitivity, 0.1f, 3.0f);
	ImGui::SliderFloat("Speed", &camera->speed, 1.0f, 25.0f);
	ImGui::SliderFloat("FOV (rads)", &(camera->FOV), 0.5f, 3.0f);

	ImGui::SliderFloat("BLUR (small value for antialiasing", &camera->blur, 0.0f, 1.0f, "%.5f");

	ImGui::End();
}

void ImGuiRenderer::RenderObjectMenus()
{

	ImGui::Begin("OBJECTS MENU");

	RenderObjectTreeNodes();
	
	if (!(ADDING_PLANE || ADDING_SPHERE || ADDING_SQUARE))
	{
		if (ImGui::Button("CLEAR ALL"))
		{
			scene->spheres.clear();
			scene->planes.clear();
			EDITED_PLANES = true;
			EDITED_SPHERES = true;
		}
		if (scene->spheres.size() >= MAX_NUM_SPHERES)
		{
			ImGui::Text("too many spheres! delete one to add more!");
		}
		else
		{
			if (ImGui::Button("ADD SPHERE")) 
			{ 
				ADDING_SPHERE = true; 
				scene->spheres.push_back(Sphere());
				sphere_adding = &scene->spheres.at(
					scene->spheres.size() - 1
				);
			}
		}

		if (scene->planes.size() >= MAX_NUM_PLANES)
		{
			
			ImGui::Text("too many planes! delete one to add more!");
		}
		else
		{
			if (ImGui::Button("ADD PLANE")) 
			{
				ADDING_PLANE = true; 
				scene->planes.push_back(Plane());
				plane_adding = &scene->planes.at(
					scene->planes.size() - 1
				);
			}
		}

		if (scene->squares.size() >= MAX_NUM_SQUARES)
		{
			ImGui::Text("too many rectangles! delete one to add more!");
		}
		else
		{
			if (ImGui::Button("ADD RECT"))
			{
				ADDING_SQUARE = true;
				scene->squares.push_back(Square());
				square_adding = &scene->squares.at(scene->squares.size() - 1);
			}
		}
	}
	else if (ADDING_SPHERE)
	{

		ImGui::Text("Adding Sphere");
		ImGui::InputFloat3("Center", sphere_center);
		ImGui::InputFloat("Radius", &sphere_radius);
;

		

		sphere_adding->position.x = sphere_center[0];
		sphere_adding->position.y = sphere_center[1];
		sphere_adding->position.z = sphere_center[2];

		sphere_adding->radius = sphere_radius;

		if (ImGui::Button("FINISH"))
		{
			ADDING_SPHERE = false;
			sphere_adding = nullptr;
		}
		EDITED_SPHERES = true;
		
	}
	else if (ADDING_PLANE)
	{
		ImGui::Text("Adding Plane");

		ImGui::InputFloat3("Normal", plane_normal);
		ImGui::InputFloat3("Point", plane_point);

		plane_adding->normal.x = plane_normal[0];
		plane_adding->normal.y = plane_normal[1];
		plane_adding->normal.z = plane_normal[2];

		plane_adding->point.x = plane_point[0];
		plane_adding->point.y = plane_point[1];
		plane_adding->point.z = plane_point[2];

		if (ImGui::Button("FINISH"))
		{
			ADDING_PLANE = false;
			plane_adding = nullptr;
		}

		EDITED_PLANES = true;
	}
	else if (ADDING_SQUARE)
	{
		ImGui::Text("Adding Rectangle");

		ImGui::InputFloat3("Center", square_center);
		ImGui::InputFloat3("Side 1", square_u);
		ImGui::InputFloat3("Side 2", square_v);

		square_adding->center.x = square_center[0];
		square_adding->center.y = square_center[1];
		square_adding->center.z = square_center[2];

		square_adding->u.x = square_u[0];
		square_adding->u.y = square_u[1];
		square_adding->u.z = square_u[2];

		square_adding->v.x = square_v[0];
		square_adding->v.y = square_v[1];
		square_adding->v.z = square_v[2];

		if (ImGui::Button("FINISH"))
		{
			ADDING_SQUARE = false;
			square_adding = nullptr;
		}

		EDITED_SQUARES = true;
	}
	ImGui::End();

	
}

void ImGuiRenderer::RenderViewport(OpenGLContext& context, unsigned int fps)
{
	if (camera->RenderMode)
	{
		RenderObjectMenus();
		RenderCamMenu();
	}

	
	if (EDITED_SPHERES) { context.OnSphereBufferChange(*scene); }
	if (EDITED_PLANES) { context.OnPlaneBufferChange(*scene); }
	if (EDITED_SQUARES) { context.OnSquareBufferChange(*scene); }


	ImGui::Begin("Scene");
	{
		if (camera->RenderMode)
		{
			ImGui::Text("TO ENTER RENDER MODE, PRESS R!");
			if (camera->isConnectedToCursor)
			{
				ImGui::Text("PRESS C TO EXIT CAMERA VIEW");
			}
			else
			{
				ImGui::Text("PRESS C TO ENTER CAMERA VIEW");
			}
		}
		else
		{
			ImGui::Text("CURRENTLY IN RENDER MODE (CANT MOVE CAMERA)! PRESS R TO EXIT!");
		}

		if (ImGui::Button("Save UI Layout"))
		{
			ImGui::SaveIniSettingsToDisk(((std::string)SRC_DIR + (const char*)"/imgui.ini").c_str());
		}

		ImGui::BeginChild("Viewport");
		ImGui::Text("FPS: %u", fps);
		ImVec2 RenderDimensions = ImGui::GetContentRegionAvail();

		if (RenderDimensions.x != old_viewport_dimensions.x || RenderDimensions.y != old_viewport_dimensions.y)
		{
			context.Resize((unsigned int)RenderDimensions.x, (unsigned int)RenderDimensions.y);
			camera->changeAspectRatio(RenderDimensions.x / RenderDimensions.y);
			old_viewport_dimensions = RenderDimensions;
			context.frame_count = 0;
			
		}

		//std::cout << camera->getFront().x << std::endl;
		camera->updateViewportDist();
		context.PassCamera(*camera);

		GLuint ViewportTexture = context.RenderToViewport();

		ImGui::Image(
			(ImTextureID)ViewportTexture,
			ImGui::GetContentRegionAvail(),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
	}
	ImGui::EndChild();
	ImGui::End();

	
}

void ImGuiRenderer::BeforeRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | 
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		//ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	windowFlags |= /*ImGuiWindowFlags_NoTitleBar|*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	
	ImGui::Begin("CoconutGraphics", nullptr, windowFlags);
	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

	ImGui::DockSpace(ImGui::GetID("InvisibleWindow"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();
}

void ImGuiRenderer::AfterRender()
{

	EDITED_SPHERES = false; 
	EDITED_PLANES = false;
	EDITED_SQUARES = false;
	

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiRenderer::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

