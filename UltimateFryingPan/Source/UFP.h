#pragma once

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class UFP
{
public:
	static UFP& getInstance()
	{
		static UFP instance;
		return instance;
	}

	UFP(const UFP&) = delete;

	void Init();
	void Run();
	void Clean();
private:
	void Render();
	void Update(float dt);
	void ProcessInput(float dt);
	
	void InitResources();

	GameState State;
	bool Keys[1024];
	unsigned int Width, Height;

	// delta time calculations
	float dt = 0;
	float curTime = 0;
	float lastTime = 0;

	GLFWwindow* Window;
	bool isRunning = true;
private: 
	UFP();
	~UFP();

	//  GLFW Callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				getInstance().Keys[key] = true;

			else if (action == GLFW_RELEASE)
				getInstance().Keys[key] = false;
		}
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
};

static UFP& Game = UFP::getInstance();
