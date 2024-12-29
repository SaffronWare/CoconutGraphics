#include "Application.h"


void Application::Setup() 
{

    if (!glfwInit())
    {
        std::cerr << "      GLFW INITIALIZATION FAILED" << std::endl;
        exit(EXIT_FAILURE);
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RED_BITS, 24);
    glfwWindowHint(GLFW_GREEN_BITS, 24);
    glfwWindowHint(GLFW_BLUE_BITS, 24);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        std::cerr << "Failed to get the primary monitor!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
        return;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) {
        std::cerr << "Failed to get video mode!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
        return;
    }

    window = glfwCreateWindow(mode->width,mode->height, "CoconutGraphics", NULL, NULL);
    if (!window)
    {
        std::cerr << "WINDOW CREATION FAILED" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
        return;
    }

    glfwMakeContextCurrent(window);

    camera = Camera(mode->width, mode->height, 1.5);
    
    glfwSwapInterval(0);

    renderer.Initialize(window, &scene, &camera);
    //std::cout << scene.planes[0].normal.x;

    
}

void Application::Run()
{
    RUNNING = true;
    float dt = 0;
    unsigned int fps = 0;

    bool C_WAS_PRESSED = false;
    bool R_WAS_PRESSED = false;

    double LastCursorX = 0.0; double LastCursorY = 0.0;
    double CursorX = 0.0; double CursorY = 0.0;

    //glfwGetCursorPos(window, &LastCursorX, &LastCursorY);

    while (!glfwWindowShouldClose(window))
    {
        fps = renderer.getFPS();
        dt = 1.0f / (float)fps;

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) { renderer.reset(); }

        if (camera.RenderMode)
        {
            

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camera.position += camera.getFront() * camera.speed * dt;
                renderer.reset();
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camera.position -= camera.getFront() * camera.speed * dt;
                renderer.reset();
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camera.position += camera.getRight() * camera.speed * dt;
                renderer.reset();
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camera.position -= camera.getRight() * camera.speed * dt;
                renderer.reset();
            }
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
                camera.position += Vec3(0, 1, 0) * camera.speed * dt;
                renderer.reset();
            }
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
                camera.position -= Vec3(0, 1, 0) * camera.speed * dt;
                renderer.reset();
            }
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !C_WAS_PRESSED)
        {
            C_WAS_PRESSED = true;
        }
        else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && C_WAS_PRESSED)
        {
            
            C_WAS_PRESSED = false;
            camera.isConnectedToCursor = !camera.isConnectedToCursor;
            glfwSetInputMode(window, GLFW_CURSOR, camera.isConnectedToCursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            glfwGetCursorPos(window, &LastCursorX, &LastCursorY);

        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !R_WAS_PRESSED)
        {
            R_WAS_PRESSED = true;
            
        }
        else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && R_WAS_PRESSED)
        {

            R_WAS_PRESSED = false;
            camera.RenderMode = !camera.RenderMode;
            renderer.reset();

        }
        
        if (camera.isConnectedToCursor && camera.RenderMode)
        {
            glfwGetCursorPos(window, &CursorX, &CursorY);
            //std::cout << CursorX;

            float dX = (float)(CursorX - LastCursorX);
            float dY = (float)(CursorY - LastCursorY);

            if (dX != 0 || dY != 0) { renderer.reset(); }

            camera.Rotation_X += dX * dt * camera.sensitivity;
            camera.Rotation_Y -= dY * dt * camera.sensitivity;

            camera.Rotation_Y = std::fminf(camera.Rotation_Y, 0.5f * (float)M_PI);
            camera.Rotation_Y = std::fmaxf(camera.Rotation_Y, -0.5f * (float)M_PI);

            LastCursorX = CursorX;
            LastCursorY = CursorY;

        }



        camera.updateDirs();
        renderer.NewFrame(window, scene, fps);
        glfwPollEvents();
        
    }

    RUNNING = false;

}

void Application::Finish()
{
    renderer.Finish();
    glfwDestroyWindow(window);
    glfwTerminate();
}