#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <Core/RVertex.h>

using namespace At0;


class App : public Ray::Engine
{
public:
private:
	void Update() override {}
};


int main()
{
	Ray::Log::Open("../../Ray.log");
	Ray::Log::SetLogLevel(Ray::LogLevel::Trace);

	Ray::VertexLayout layout;
	layout.Append(Ray::VertexLayout::Position3D, Ray::VertexLayout::Float3Color);

	Ray::VertexInput vertexInput(layout);
	vertexInput.Emplace(Ray::Float3{ -0.5f, -0.5f, 0.0f }, Ray::Float3{ 1.0f, 0.0f, 0.0f });
	vertexInput.Emplace(Ray::Float3{ 0.5f, -0.5f, 0.0f }, Ray::Float3{ 0.0f, 1.0f, 0.0f });
	vertexInput.Emplace(Ray::Float3{ 0.0f, 0.5f, 0.0f }, Ray::Float3{ 0.0f, 0.0f, 1.0f });

	App app;

	Ray::Window::Create();
	Ray::Window::Get().Show();
	Ray::Graphics::Get();

	return app.Run();
}
