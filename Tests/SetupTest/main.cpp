#include <Core/RApplication.h>
#include <Devices/RWindow.h>


using namespace At0;

int main()
{
	Ray::Window::Create();
	Ray::Window::Get().Show();

	return Ray::Application::Get().Run();
}
