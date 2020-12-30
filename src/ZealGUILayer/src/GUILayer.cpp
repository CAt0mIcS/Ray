#include "glpch.h"
#include "GUILayer.h"

// QUESTION: Should I add all includes that the client might need into Ray.h or let them include them on their own (but they will need to set additional include paths to other libraries)
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>

#include <Ray/Ray.h>
#include <RayRender/Renderer3D.h>
#include <RayRender/Drawable/Triangle.h>
#include <RayRender/Drawable/Cube.h>
#include <RayRender/Drawable/ShadedCube.h>
#include <RayRender/PointLight.h>
#include <RayRender/Drawable/Model.h>
#include <RayRender/Camera.h>



class FPS
{
public:
	FPS()
		: m_FPS(0), m_FPSCount(0), m_InitialInterval(GetTickCount64())
	{
	}

	void Update()
	{
		m_FPSCount++;

		// one second elapsed? (= 1000 milliseconds)
		if (IntervalValue() > 1000)
		{
			// save the current counter value to m_FPS
			m_FPS = m_FPSCount;

			// reset the counter and the interval
			m_FPSCount = 0;
			m_InitialInterval = GetTickCount64();

			std::ostringstream oss;
			oss << GetFPS() << " FPS";
			// RAY_TODO: Abort when trying to set the title on an unopened window!
			At0::Ray::Application::Get().GetMainWindow().SetTitle(oss.str());
		}
	}

	uint32_t GetFPS() const
	{
		return m_FPS;
	}

	uint32_t IntervalValue() const
	{
		return GetTickCount64() - m_InitialInterval;
	}

private:
	uint32_t m_FPS;
	uint32_t m_FPSCount;
	uint64_t m_InitialInterval;
};

FPS g_FPS;



namespace At0::Layers
{
#define RENDER 1

	std::vector<Ray::ShadedCube> cubes;
	static constexpr uint64_t numCubes = 4000;
	Ray::Model model;
	std::mt19937 mtEngine;
	Ray::Camera cam;
	Ray::PushButton* button;
	Ray::Scope<Ray::PointLight> ptLight;

	GUILayer::GUILayer(std::string_view name)
		: Ray::Layer(name),
		EventListener<Ray::WindowCloseEvent>(GetMainWindow()),
		EventListener<Ray::KeyPressedEvent>(GetMainWindow()),
		EventListener<Ray::KeyReleasedEvent>(GetMainWindow()),
		EventListener<Ray::CharEvent>(GetMainWindow()),

		EventListener<Ray::MouseButtonPressedEvent>(GetMainWindow()),
		EventListener<Ray::MouseButtonReleasedEvent>(GetMainWindow()),

		EventListener<Ray::HoverEnterEvent>(GetMainWindow()),
		EventListener<Ray::HoverLeaveEvent>(GetMainWindow()),

		EventListener<Ray::MouseWheelUpEvent>(GetMainWindow()),
		EventListener<Ray::MouseWheelDownEvent>(GetMainWindow()),
		EventListener<Ray::MouseWheelLeftEvent>(GetMainWindow()),
		EventListener<Ray::MouseWheelRightEvent>(GetMainWindow())
	{
		EventListener<Ray::MouseMoveEvent>::Subscribe(GetMainWindow());

#if RENDER
		button = GetMainWindow().AddChild<Ray::PushButton>("Button");
		//button = Ray::MakeScope<Ray::PushButton>("Button", GetMainWindow());

		button->Resize({ 100, 100 });
		button->Move({ 0, 0 });

		Ray::Size2 s = button->GetSize();
		Ray::Point2 p = button->GetPos();

		ptLight = Ray::MakeScope<Ray::PointLight>(GetMainWindow().GetRenderer3D());

		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] Startup");

		mtEngine.seed((uint32_t)time(0));
		std::uniform_real_distribution<float> colDist(0.0f, 1.0f);

		float face_colors[6][3] =
		{
			{ colDist(mtEngine), colDist(mtEngine), colDist(mtEngine) },
			{ colDist(mtEngine), colDist(mtEngine), colDist(mtEngine) },
			{ colDist(mtEngine), colDist(mtEngine), colDist(mtEngine) },
			{ colDist(mtEngine), colDist(mtEngine), colDist(mtEngine) },
			{ colDist(mtEngine), colDist(mtEngine), colDist(mtEngine) },
			{ colDist(mtEngine), colDist(mtEngine), colDist(mtEngine) }
		};

		//float face_colors[6][3] =
		//{
		//	{ 1.0f, 1.0f, 1.0f },
		//	{ 1.0f, 1.0f, 1.0f },
		//	{ 1.0f, 1.0f, 1.0f },
		//	{ 1.0f, 1.0f, 1.0f },
		//	{ 1.0f, 1.0f, 1.0f },
		//	{ 1.0f, 1.0f, 1.0f }
		//};

		// RAY_TODO: Need to reserve because ConstantBuffer takes reference to Drawable(Cube) parent
		// and the memory address changes when std::vector reallocates

		if constexpr (numCubes != 0)
		{
			cubes.reserve(numCubes);
			for (uint32_t i = 0; i < numCubes - 1; ++i)
			{
				cubes.emplace_back(GetMainWindow().GetRenderer3D()/*, 1.0f, face_colors*/);
			}

			//cubes.emplace_back(*Ray::Application::Get().GetMainWindow().GetRenderer3D()/*, 1.0f, face_colors*/);

			std::uniform_real_distribution<float> posDist(-50.0f, 50.0f);
			std::uniform_real_distribution<float> scaleDist(0.3f, 2.0f);
			for (uint32_t i = 0; i < numCubes - 1; ++i)
			{
				cubes[i].SetRotation(posDist(mtEngine), posDist(mtEngine), posDist(mtEngine));
				cubes[i].SetTranslation(posDist(mtEngine), posDist(mtEngine), posDist(mtEngine) - 20.0f);
				cubes[i].SetScale(scaleDist(mtEngine), scaleDist(mtEngine), scaleDist(mtEngine));
			}
		}

		model = Ray::Model("Resources/nanosuit.obj", face_colors, GetMainWindow().GetRenderer3D());
		GetMainWindow().GetRenderer3D().SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f));

#endif
	}

	Ray::Point2 mousePos{};
	void GUILayer::OnUpdate(Ray::Timestep ts)
	{
#if RENDER

		Ray::Window& window = GetMainWindow();
		Ray::Renderer3D& renderer = window.GetRenderer3D();
		renderer.ClearBuffer(0.07f, 0.0f, 0.12f);

		static float pitch = 0.0f;
		static float yaw = 0.0f;
		static float roll = 0.0f;
		static float xDir = 0.0f;
		static float yDir = 0.0f;
		static float zDir = 0.0f;

		mousePos = window.Mouse.GetPos();

		Ray::KeyboardInput& kbd = window.Keyboard;
		if (kbd.IsKeyPressed(Ray::Key::S))
		{
			zDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed(Ray::Key::W))
		{
			zDir += 3.0f * ts;
		}
		if (kbd.IsKeyPressed(Ray::Key::Space))
		{
			yDir += 3.0f * ts;
		}
		if (kbd.IsKeyPressed(Ray::Key::A))
		{
			xDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed(Ray::Key::LeftControl))
		{
			yDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed(Ray::Key::D))
		{
			xDir += 3.0f * ts;
		}
		if (kbd.IsKeyPressed(Ray::Key::Q))
		{
			cam.roll += 3.0f * ts;
		}
		if (kbd.IsKeyPressed(Ray::Key::E))
		{
			cam.roll -= 3.0f * ts;
		}

		pitch += 0.5f * ts;
		yaw += 0.5f * ts;
		roll += 0.5f * ts;

		renderer.SetCamera(cam.GetMatrix());

		ptLight->Bind();

		if constexpr (numCubes != 0)
		{
			for (uint32_t i = 0; i < cubes.size() - 1; ++i)
			{
				cubes[i].SetRotation(pitch + i, yaw + i, roll + i);
			}

			cubes.back().SetRotation(pitch, yaw, roll);
			//cubes.back().SetTranslation(xDir, yDir, zDir);

			// Takes the most amount of time here!
			for (auto& cube : cubes)
			{
				cube.Draw(&renderer);
			}
		}

		//model.Draw(&renderer);
		ptLight->SetPos({ xDir, yDir, zDir });
		ptLight->Draw();

		// RAY_TODO
		button->Draw();

		renderer.EndDraw();
#endif

		//g_FPS.Update();
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseMoveEvent& e)
	{
		//static std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevTime).count() << '\n';
		//prevTime = std::chrono::high_resolution_clock::now();
		auto& mouse = Ray::Application::Get().GetMainWindow().Mouse;
		auto& kbd = Ray::Application::Get().GetMainWindow().Keyboard;
		if (mouse.IsMiddlePressed())
		{
			auto mouseDiff = mouse.GetPos() - mousePos;
			if (kbd.IsKeyPressed(Ray::Key::LeftShift)) // LSHIFT
			{
				cam.x += mouseDiff.x * 0.03f;
				cam.y -= mouseDiff.y * 0.03f;
			}
			else
			{
				cam.theta -= mouseDiff.x * 0.003f;
				cam.phi += mouseDiff.y * 0.003f;
			}
		}
		if (mouse.IsRightPressed())
		{
			auto mouseDiff = mouse.GetPos() - mousePos;
			cam.yaw -= mouseDiff.x * 0.003f;
			cam.pitch -= mouseDiff.y * 0.003f;
		}

		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		std::cout << "[" << receiver.GetName() << "]: " << e.ToString() << '\n';
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::WindowCloseEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::KeyPressedEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		std::cout << e.ToString() << '\n';
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::KeyReleasedEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		std::cout << e.ToString() << '\n';
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::CharEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		std::cout << e.ToString() << '\n';
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseButtonPressedEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		std::cout << e.ToString() << '\n';
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseButtonReleasedEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		std::cout << e.ToString() << '\n';
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::HoverEnterEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		std::cout << "[" << receiver.GetName() << "]: " << e.ToString() << '\n';
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::HoverLeaveEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		std::cout << "[" << receiver.GetName() << "]: " << e.ToString() << '\n';
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseWheelUpEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		cam.z -= 1.0f;
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseWheelDownEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
		cam.z += 1.0f;
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseWheelLeftEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseWheelRightEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

}
