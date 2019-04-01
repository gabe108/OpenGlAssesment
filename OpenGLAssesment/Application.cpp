#include "Application.h"

Application::Application()
{

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
	view = glm::lookAt(glm::vec3(20, 20, 20), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f,
		16 / 9.f, 0.1f, 1000.f);

	return true;
}

bool Application::Run()
{
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

	aie::Gizmos::draw(projection * view);

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
