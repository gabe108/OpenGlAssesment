#include"Application.h"

int main()
{
	Application* app = new Application();

	if (app->Startup(1280, 720, "Nebula"))
	{
		while (app->Run() == true)
			app->Draw();

		app->ExitApplication();
	}
	return 0;	
}