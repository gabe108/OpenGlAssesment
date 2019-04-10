#pragma once
#include"gl_core_4_4.h"
#include"Gizmos.h"
#include"FlyCamera.h"
#include<glm/ext.hpp>
#include<iostream>

class Shader;
class Model;

class Application
{
public:
	Application();
	~Application();

	bool Startup(float _width, float _height, const char* _title);
	bool Run();
	void Draw();
	void ExitApplication();

protected:
	float m_fCurTime;
	float m_fDeltaTime;
	float m_fPrevTime;
	Shader* m_shader;
	GLFWwindow* m_mainWindow;
	FlyCamera* m_cam;
	Model* ourModel;
};

