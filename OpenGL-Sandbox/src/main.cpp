#include"SandboxLayer.h"
#include<GLCore/Core/Application.h>

using namespace GLCore;

class Sandbox : public Application
{
public:
	Sandbox()
		:Application("the", 1280, 720)
	{
		PushLayer(new SandboxLayer());
	}
};

int main()
{
	std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
	app->Run();
}