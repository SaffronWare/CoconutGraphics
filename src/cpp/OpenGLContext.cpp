#include "OpenGLContext.h"
#include <ostream>



void UNIFORMLOCS::Initialize(GLuint program)
{
   
  
    GLint p_CameraPosition = glGetUniformLocation(program, "u_CameraPosition");
    GLint p_CameraUp = glGetUniformLocation(program, "u_CameraUp");
    GLint p_CameraRight = glGetUniformLocation(program, "u_CameraRight");
    GLint p_CameraFront = glGetUniformLocation(program, "u_CameraFront");
    GLint p_AspectRatio = glGetUniformLocation(program, "u_AspectRatio");
    GLint p_FrameCount = glGetUniformLocation(program, "u_FrameCount");
    GLint p_RenderMode = glGetUniformLocation(program, "u_RenderMode");
    GLint p_AliasingBlur = glGetUniformLocation(program, "u_AliasingBlur");
    GLint p_SkyboxIntensity = glGetUniformLocation(program, "u_SkyboxStrength");
    GLint p_SkyboxContrast = glGetUniformLocation(program, "u_SkyboxContrast"); 

    GLint p_SphereBuffer = glGetUniformBlockIndex(program, "SphereData");
    GLint p_PlaneBuffer = glGetUniformBlockIndex(program, "PlaneData");
    GLint p_SquareBuffer = glGetUniformBlockIndex(program, "SquareData");

    GLint p_prevRender = glGetUniformLocation(program, "prevRender");
    GLint p_skybox = glGetUniformLocation(program, "skybox");



    
    if (p_CameraPosition == -1) { std::cerr << "CameraPosition uniform not found!\n"; }
    if (p_CameraUp == -1) { std::cerr << "CameraUp uniform not found!\n"; }
    if (p_CameraRight == -1) { std::cerr << "CameraRight uniform not found!\n"; }
    if (p_CameraFront == -1) { std::cerr << "CameraFront uniform not found!\n"; }
    if (p_AspectRatio == -1) { std::cerr << "AspectRatio uniform not found\n"; }
    if (p_RenderMode == -1) { std::cerr << "RenderMode uniform not found\n"; }
    if (p_AliasingBlur == -1) { std::cerr << "Aliasing Blur uniform not found\n"; }
    if (p_SkyboxIntensity == -1) { std::cerr << "Skybox Intensity uniform not found\n"; }
    if (p_SkyboxContrast == -1) { std::cerr << "Skybox Contrast uniform not found\n"; }
    if (p_SphereBuffer == -1) { std::cerr << "SphereBuffer uniform not found\n"; }
    if (p_PlaneBuffer == -1) { std::cerr << "PlaneBuffer uniform not found\n"; }
    if (p_FrameCount == -1) { std::cerr << "FrameCount uniform not found\n"; }
    if (p_SquareBuffer == -1) { std::cerr << "SquareBuffer uniform not found\n"; }
    if (p_prevRender == -1) { std::cerr << "Previous render texture uniform not found\n"; }
    if (p_skybox == -1) { std::cerr << "Skybox uniform not found\n"; }




   
    CameraPosition = p_CameraPosition;
    CameraUp = p_CameraUp;
    CameraRight = p_CameraRight;
    CameraFront = p_CameraFront;
    AspectRatio = p_AspectRatio;
    FrameCount = p_FrameCount;
    RenderMode = p_RenderMode;
    Aliasing_Blur = p_AliasingBlur;
    SkyboxIntensity = p_SkyboxIntensity;
    SkyboxContrast = p_SkyboxContrast;

    sphereBuffer = p_SphereBuffer;
    planeBuffer = p_PlaneBuffer;
    squareBuffer = p_SquareBuffer;
    prevRender = p_prevRender;
    skybox = p_skybox;

}

void OpenGLContext::Initialize()
{


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "FAILED TO INTIALIZE GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, 800, 800);

    shader.Compile("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    shader.Use();
    LOCS.Initialize(shader.GetProgram());
    glUniform1i(LOCS.prevRender, 0);
    glUniform1i(LOCS.skybox, 1);
    shader.EndUse();




    glGenVertexArrays(1, &Viewport_VAO);
    glGenBuffers(1, &Viewport_VBO);

    glBindVertexArray(Viewport_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Viewport_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_on_viewport), vertices_on_viewport, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glGenFramebuffers(1, &Viewport_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, Viewport_FBO);

    glGenTextures(1, &Viewport_Texture);
    glBindTexture(GL_TEXTURE_2D, Viewport_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 100, 100, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // width and height get reset anyway
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Viewport_Texture, 0);


    glGenRenderbuffers(1, &Viewport_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, Viewport_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, 100, 100);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Viewport_RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenBuffers(1, &SphereUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, SphereUBO);
    glUniformBlockBinding(shader.GetProgram(), LOCS.sphereBuffer, LOCS.sphereBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Sphere) * MAX_NUM_SPHERES, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, LOCS.sphereBuffer, SphereUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    glGenBuffers(1, &PlaneUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, PlaneUBO);
    glUniformBlockBinding(shader.GetProgram(), LOCS.planeBuffer, LOCS.planeBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Plane) * MAX_NUM_PLANES, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, LOCS.planeBuffer, PlaneUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    glGenBuffers(1, &SquareUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, SquareUBO);
    glUniformBlockBinding(shader.GetProgram(), LOCS.squareBuffer, LOCS.squareBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Square) * MAX_NUM_SQUARES, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, LOCS.squareBuffer, SquareUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    std::string path_to_skybox = "assets/skybox.hdr";
    LoadHDR(&Skybox_Texture, path_to_skybox.c_str());
}

void OpenGLContext::EndContext()
{
    glDeleteFramebuffers(1, &Viewport_FBO);
    glDeleteTextures(1, &Viewport_Texture);
    glDeleteRenderbuffers(1, &Viewport_RBO);
    glDeleteBuffers(1, &PlaneUBO);
    glDeleteBuffers(1, &SphereUBO);
    glDeleteTextures(1, &Skybox_Texture);
    glFinish();
    shader.Delete();
}

void OpenGLContext::Resize(unsigned int viewport_width, unsigned int viewport_height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, Viewport_FBO);

    glBindTexture(GL_TEXTURE_2D, Viewport_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewport_width, viewport_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Viewport_Texture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, Viewport_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, viewport_width, viewport_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Viewport_RBO);

    glViewport(0, 0, viewport_width, viewport_height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}



GLuint OpenGLContext::RenderToViewport()
{
 

    glUniform1ui(LOCS.FrameCount, frame_count);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Skybox_Texture);

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, Viewport_Texture);

    glBindFramebuffer(GL_FRAMEBUFFER, Viewport_FBO);
    glBindVertexArray(Viewport_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    frame_count += 1;
    

    return Viewport_Texture;
    
}



void OpenGLContext::PassCamera(const Camera& camera)
{
    glUniform1i(LOCS.RenderMode, camera.RenderMode);
    glUniform1f(LOCS.AspectRatio, camera.getAspectRatio());
    glUniform1f(LOCS.Aliasing_Blur, camera.blur * 0.01f);
    glUniform1f(LOCS.SkyboxIntensity, camera.SkyboxIntensity);
    glUniform1f(LOCS.SkyboxContrast, camera.SkyboxContrast);
    glUniform3f(LOCS.CameraUp, camera.getUp().x, camera.getUp().y, camera.getUp().z);
    glUniform3f(LOCS.CameraRight, camera.getRight().x, camera.getRight().y, camera.getRight().z);
    glUniform3f(LOCS.CameraFront, camera.getViewportFront().x, camera.getViewportFront().y, camera.getViewportFront().z);
    glUniform3f(LOCS.CameraPosition, camera.position.x, camera.position.y, camera.position.z);
}

void OpenGLContext::UseProgram()
{
    shader.Use();
}

void OpenGLContext::EndUseProgram()
{
    shader.EndUse();
}

void OpenGLContext::OnSphereBufferChange(const Scene& scene)
{
    glBindBuffer(GL_UNIFORM_BUFFER, SphereUBO);
    glBufferData(GL_UNIFORM_BUFFER, MAX_NUM_SPHERES * sizeof(Sphere), nullptr, GL_STATIC_DRAW);
    glBufferData(GL_UNIFORM_BUFFER,  scene.spheres.size() * sizeof(Sphere), scene.spheres.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLContext::OnPlaneBufferChange(const Scene& scene)
{
    glBindBuffer(GL_UNIFORM_BUFFER, PlaneUBO);
    glBufferData(GL_UNIFORM_BUFFER, MAX_NUM_PLANES * sizeof(Plane), nullptr, GL_STATIC_DRAW);
    glBufferData(GL_UNIFORM_BUFFER, scene.planes.size() * sizeof(Plane), scene.planes.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void OpenGLContext::OnSquareBufferChange(const Scene& scene)
{   
    glBindBuffer(GL_UNIFORM_BUFFER, SquareUBO);
    glBufferData(GL_UNIFORM_BUFFER, MAX_NUM_SQUARES * sizeof(Square), nullptr, GL_STATIC_DRAW);
    glBufferData(GL_UNIFORM_BUFFER, scene.squares.size() * sizeof(Square), scene.squares.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

