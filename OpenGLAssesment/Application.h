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
	/// Setting default values
	Application();
	~Application();

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
	bool Startup(int _width, int _height, const char* _title);

	/// <summary>
	/// Calculating the deltaTime and running a while loop to update every frame
	/// </summary>
	/// <returns>
	/// return a bool, true if the user doesnt exit the application and false if the user
	/// exits the application
	/// </returns>
	bool Run();

	/// <summary>
	/// Draws on the screen
	/// </summary>
	void Draw();

	/// <summary>
	/// Exits the application
	/// </summary>
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

