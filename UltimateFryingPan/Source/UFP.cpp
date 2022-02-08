#include "pch.h"
#include "UFP.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

SpriteRenderer* Renderer;

UFP::UFP()
    :Width(1080),Height(720), State(GAME_ACTIVE), Window(), Keys() {}
UFP::~UFP(){}

void UFP::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    Window = glfwCreateWindow(Width, Height, "Ultimate Frying Pan", nullptr, nullptr);
    glfwMakeContextCurrent(Window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetWindowUserPointer(Window, Window);

    glfwSetKeyCallback(Window, key_callback);
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InitResources();
}

void UFP::InitResources()
{   
    // load shaders
    ResourceManager::LoadShader("Source/Shaders/SpriteVS.shader", "Source/Shaders/SpriteFS.shader", nullptr, "sprite");

    // load textures
    ResourceManager::LoadTexture("Assets/CleanGrass.png", true, "grass");

    // configure shaders
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("Image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", proj);

    // configure renderer
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
}

void UFP::Clean()
{
    ResourceManager::Clear();

    delete Renderer;
}

void UFP::Run()
{
    // calculate delta time
    curTime = (float)glfwGetTime();
    dt = curTime - lastTime;
    lastTime = curTime;

	while (isRunning)
	{
        ProcessInput(dt);
		Update(dt);
		Render();
	}
}

void UFP::ProcessInput(float dt)
{
    if (State == GAME_ACTIVE)
    {
        if (Keys[GLFW_KEY_SPACE])
        {
        }
    }
}

void UFP::Update(float dt)
{
}

void UFP::Render()
{
    glfwPollEvents();
    if (State == GAME_ACTIVE)
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("grass"), glm::vec2(0.0), glm::vec2(50, 50));
    }
    glfwSwapBuffers(Window);
}

