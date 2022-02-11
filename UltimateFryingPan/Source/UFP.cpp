#include "pch.h"
#include "UFP.h"

#include "ResourceManager.h"
#include "Coordinator.h"

//Components
#include "Transform.h"
#include "Gravity.h"
#include "Rigidbody.h"
#include "RenderableSprite.h"

//Systems
#include "PhysicsSystem.h"
#include "SpriteRendererSystem.h"

// ECS
Coordinator gCoordinator;

// Systems
std::shared_ptr<SpriteRendererSystem> spriteRendererSystem;
std::shared_ptr<PhysicsSystem> physicsSystem;

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
    ResourceManager::LoadTexture("Assets/orb.png", true, "orb");

    // configure shaders
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("Image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", proj);

    // -------------Configure ECS-------------------------------------------
    gCoordinator.Init();

    gCoordinator.RegisterComponent<Transform>();
    gCoordinator.RegisterComponent<Rigidbody>();
    gCoordinator.RegisterComponent<Gravity>();
    gCoordinator.RegisterComponent<RenderableSprite>();

    //// set sprite renderer system
    spriteRendererSystem = gCoordinator.RegisterSystem<SpriteRendererSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<RenderableSprite>());
        gCoordinator.SetSystemSignature<SpriteRendererSystem>(signature);
    }
    spriteRendererSystem->Init();

    // set physicsSystem Signature
    physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Rigidbody>());
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<Gravity>());
        gCoordinator.SetSystemSignature<PhysicsSystem>(signature);
    }
    physicsSystem->Init();
    // ----------------------------------------------------------------------

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(0.0f, Width);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(10.0f, 15.0f);
    std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

    float scale = randScale(generator);

    //std::vector<Entity> entities(MAX_ENTITIES -1);
    std::vector<Entity> entities(1500);
    for (Entity& entity : entities)
    {
        entity = gCoordinator.CreateEntity();
        gCoordinator.AddComponent<Gravity>(entity, {glm::vec3(0.0f, 9.8, 0.0f)});
        gCoordinator.AddComponent<Rigidbody>(entity, { glm::vec3(0.0f), glm::vec3(0.0f) });
        gCoordinator.AddComponent<Transform>(entity, {
            glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator)),
            glm::vec3(scale, scale, scale)
            });
        gCoordinator.AddComponent<RenderableSprite>(entity, { glm::vec3(1.0f), ResourceManager::GetTexture("orb") });
    }
}

void UFP::Clean()
{
    ResourceManager::Clear();
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
    physicsSystem->Update(dt);
}

double prevtime = 0;
double curTime = 0;
double timeDiff;
unsigned int counter = 0;
void UFP::Render()
{
    curTime = glfwGetTime();
    timeDiff = curTime - prevtime;
    counter++;
    if (timeDiff >= 1.0 / 30.0)
    {
        std::string FPS = std::to_string((1.0 / timeDiff) * counter);
        std::string ms = std::to_string((timeDiff / counter) * 1000);
        std::string newTitle = "ECS " + FPS + " FPS / " + ms + "ms";
        glfwSetWindowTitle(Window, newTitle.c_str());
        prevtime = curTime;
        counter = 0;
    }

    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    if (State == GAME_ACTIVE)
    {
        spriteRendererSystem->Update();
    }
    glfwSwapBuffers(Window);
}

