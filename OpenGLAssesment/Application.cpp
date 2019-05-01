#include"Application.h"
#include"Shader.h"
#include"Model.h"
#include"filesystem.h"

#include<iostream>

/// Setting default values
Application::Application()
{
	m_fPrevTime = (float)(glfwGetTime());
	m_fCurTime = 0;
	m_fDeltaTime = 0;
}

Application::~Application()
{
	
}

/// <summary>
/// Initializing glfw, create a mainwindow and call ogl_LoadFunctions()
/// Create a camera 
/// </summary>
/// <returns>
/// returns true if everything goes to plan without any error
/// </returns>
/// <param name="_width"> float </param>
/// <param name="_height"> float </param>
/// <param name="_title"> const char* </param>
bool Application::Startup(int _width, int _height, const char* _title)
{
	// Initialize glfw
	if (!glfwInit())
	{
		std::cout << "GLFW Initialization failed" << std::endl;
		return false;
	}

	// Create the main window
	m_mainWindow = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
	if (m_mainWindow == nullptr)
	{
		std::cout << "Window Initialization failed" << std::endl;
		glfwTerminate();
		return false;
	}

	// Set the current window
	glfwMakeContextCurrent(m_mainWindow);

	//Load opengl functions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cout << "OpenGL load failed" << std::endl;
		glfwDestroyWindow(m_mainWindow);
		glfwTerminate();
		return false;
	}

	// Create teh Gizmos
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	// Creating the camera and set its view and projection transform
	m_cam = new FlyCamera(4.f, glm::vec3(0, 1, 0));
	m_cam->setLookAt(glm::vec3(20, 20, 20), glm::vec3(0), glm::vec3(0,1,0));
	m_cam->setPerspective(glm::pi<float>() * 0.25f,
		16 / 9.f, 0.1f, 1000.f);

	// Disable the cursor
	glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Create the shader program with shaders attached
	m_shader = new Shader("./Shader/shader.vert", "./Shader/shader.frag");

	// load the model
	ourModel = new Model(FileSystem::getPath("resources/nanosuit/nanosuit.obj"));
	return true;
}

/// <summary>
/// Calculating the deltaTime and running a while loop to update every frame
/// </summary>
/// <returns>
/// return a bool, true if the user doesnt exit the application and false if the user
/// exits the application
/// </returns>
bool Application::Run()
{
	// Calculate DeltaTIme
	m_fCurTime = (float)(glfwGetTime());
	m_fDeltaTime = m_fCurTime - m_fPrevTime;
	m_fPrevTime = m_fCurTime;

	// if the user didnt exit the application continue to run and update stuff
	if (glfwWindowShouldClose(m_mainWindow) == false &&
		glfwGetKey(m_mainWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		m_cam->update(m_fDeltaTime);
		return true;
	}

	// exit the application
	return false;
}

/// <summary>
/// Draws on the screen
/// </summary>
void Application::Draw()
{
	// Set the background color
	glClearColor(0.19f, 0.6f, 0.8f, 1);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	aie::Gizmos::clear();
	aie::Gizmos::addTransform(glm::mat4(1));

	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	aie::Gizmos::draw(m_cam->getProjectionViewTransform());

	// Rotating the light around the model
	float lightX = (float)(3.0f * sin(glfwGetTime()));
	float lightY = -0.3f;
	float lightZ = (float)(2.5f * cos(glfwGetTime()));
	glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);

	// Use the shader
	m_shader->use();

	// Setup teh uniforms
	m_shader->setMat4("projection", m_cam->getProjectionTransform());
	m_shader->setMat4("view", m_cam->getViewTransform());
	m_shader->setVec3("lightPos", lightPos);
	m_shader->setVec3("viewPos", m_cam->getPos());
	m_shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	m_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	m_shader->setMat4("model", model);
	ourModel->Draw(m_shader);
	   
	// Swap Buffers 
	glfwSwapBuffers(m_mainWindow);
	glfwPollEvents();
}

/// <summary>
/// Exits the application
/// </summary>
void Application::ExitApplication()
{
	aie::Gizmos::destroy();
	glfwDestroyWindow(m_mainWindow);
	glfwTerminate();
	return;
}
