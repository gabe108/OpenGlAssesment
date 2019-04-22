#include"Application.h"
#include"Shader.h"
#include"Model.h"
#include"filesystem.h"

#include<iostream>

Application::Application()
{
	m_fPrevTime = glfwGetTime();
	m_fCurTime = 0;
	m_fDeltaTime = 0;
}

Application::~Application()
{
	
}

bool Application::Startup(float _width, float _height, const char* _title)
{
	if (!glfwInit())
	{
		std::cout << "GLFW Initialization failed" << std::endl;
		return false;
	}

	m_mainWindow = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
	if (m_mainWindow == nullptr)
	{
		std::cout << "Window Initialization failed" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_mainWindow);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cout << "OpenGL load failed" << std::endl;
		glfwDestroyWindow(m_mainWindow);
		glfwTerminate();
		return false;
	}

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_cam = new FlyCamera(4.f, glm::vec3(0, 1, 0));
	m_cam->setLookAt(glm::vec3(20, 20, 20), glm::vec3(0), glm::vec3(0,1,0));
	m_cam->setPerspective(glm::pi<float>() * 0.25f,
		16 / 9.f, 0.1f, 1000.f);

	glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_shader = new Shader("./Shader/shader.vert", "./Shader/shader.frag");

	ourModel = new Model(FileSystem::getPath("resources/nanosuit/nanosuit.obj"));
	return true;
}

bool Application::Run()
{
	m_fCurTime = glfwGetTime();
	m_fDeltaTime = m_fCurTime - m_fPrevTime;
	m_fPrevTime = m_fCurTime;

	if (glfwWindowShouldClose(m_mainWindow) == false &&
		glfwGetKey(m_mainWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		return true;
	}
	return false;
}

void Application::Draw()
{
	glClearColor(0.3f, 0.2f, 0.2f, 1);
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

	m_cam->update(m_fDeltaTime);
	aie::Gizmos::draw(m_cam->getProjectionViewTransform());

	float lightX = 3.0f * sin(glfwGetTime());
	float lightY = -0.3f;
	float lightZ = 2.5f * cos(glfwGetTime());
	glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);

	m_shader->use();

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
	   
	glfwSwapBuffers(m_mainWindow);
	glfwPollEvents();
}

void Application::ExitApplication()
{
	aie::Gizmos::destroy();
	glfwDestroyWindow(m_mainWindow);
	glfwTerminate();
	return;
}
