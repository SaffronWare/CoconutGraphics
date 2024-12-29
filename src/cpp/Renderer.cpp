#include "Renderer.h"



void Renderer::Initialize(GLFWwindow* window, Scene* scene, Camera* camera)
{
    GLcontext.Initialize();

    imgui_renderer.Initialize(window, scene, camera);

    GLcontext.OnPlaneBufferChange(*scene);
    GLcontext.OnSphereBufferChange(*scene);


   
}

void Renderer::NewFrame(GLFWwindow* window, const Scene& scene, unsigned int fps)
{
    
    GLcontext.UseProgram();
    //GLcontext.PassObjects(scene);
    


    imgui_renderer.BeforeRender();

    imgui_renderer.RenderViewport(GLcontext, fps);
 
    imgui_renderer.AfterRender();

    glfwSwapBuffers(window);

    GLcontext.EndUseProgram();
}

void Renderer::Finish()
{
    
    imgui_renderer.CleanUp();
    GLcontext.EndContext();
}

unsigned int Renderer::getFPS()
{
    current_Time = glfwGetTime();
    double time_Difference = current_Time - previous_Time;
    unsigned int fps = static_cast<unsigned int>(1.0 / time_Difference);
    previous_Time = current_Time;
    return fps;
    
}

void Renderer::reset()
{
    GLcontext.frame_count = 0;
}