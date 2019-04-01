#include"Application.h"

int main()
{
	Application* app = new Application();

	if (app->Startup(1280.f, 720.f, "Nebula"))
	{
		while (app->Run() == true)
			app->Draw();

		app->ExitApplication();
	}
	return 0;	
}