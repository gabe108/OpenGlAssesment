#pragma once
#include"gl_core_4_4.h"
#include"Gizmos.h"
#include<GLFW/glfw3.h>
#include<glm/ext.hpp>
#include<iostream>

class Application
{
public:
	Application();
	~Application();

	bool Startup(float _width, float _height, const char* _title);
	bool Run();
	void Draw();
	void ExitApplication();

	bool m_exit;
	bool m_view = true;
protected:
	GLFWwindow* m_mainWindow;
	glm::mat4 view;
	glm::mat4 projection;
};

