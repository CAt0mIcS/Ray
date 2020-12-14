#include "glpch.h"
#include "GUILayer.h"

// You need to include this to use the logger (QUESTIONA: But you shouldn't need to!) (everything in .def file?)
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>

#include <Ray/Ray.h>
#include <RayRender/Renderer3D.h>
#include <RayRender/Drawable/Triangle.h>
#include <RayRender/Drawable/Cube.h>
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
		// increase the counter by one
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
	std::vector<Ray::Cube> cubes;
	static constexpr uint64_t numCubes = 3000;
	Ray::Model model;
	std::mt19937 mtEngine;
	Ray::Camera cam;

	// QUESTIONA: The way to get the MainWindow is too long? "Ray::Application::Get().GetMainWindow()" (fn in (maybe) Layer to get or static fn somewhere)
	// I can only listen to events from one window (MouseMoveEvent from only MainWindow)
	GUILayer::GUILayer(std::string_view name)
		: Ray::Layer(name),
		EventListener<Ray::WindowCloseEvent>(Ray::Application::Get().GetMainWindow()),
		EventListener<Ray::KeyPressedEvent>(Ray::Application::Get().GetMainWindow()),

		EventListener<Ray::MouseWheelUpEvent>(Ray::Application::Get().GetMainWindow()),
		EventListener<Ray::MouseWheelDownEvent>(Ray::Application::Get().GetMainWindow()),
		EventListener<Ray::MouseWheelLeftEvent>(Ray::Application::Get().GetMainWindow()),
		EventListener<Ray::MouseWheelRightEvent>(Ray::Application::Get().GetMainWindow())
	{
		EventListener<Ray::MouseMoveEvent>::Subscribe(Ray::Application::Get().GetMainWindow());
		//EventListener<Ray::MouseMoveEvent>::Subscribe(Ray::Application::Get().FindWindowByName("Win0"));

		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[GUILayer] Startup");

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
				cubes.emplace_back(*Ray::Application::Get().GetMainWindow().GetRenderer3D(), 1.0f, face_colors);
			}

			cubes.emplace_back(*Ray::Application::Get().GetMainWindow().GetRenderer3D(), 1.0f, face_colors);

			std::uniform_real_distribution<float> posDist(-50.0f, 50.0f);
			for (uint32_t i = 0; i < numCubes - 1; ++i)
			{
				cubes[i].SetTransform(
					DirectX::XMMatrixRotationRollPitchYaw(posDist(mtEngine), posDist(mtEngine), posDist(mtEngine)) *
					DirectX::XMMatrixTranslation(posDist(mtEngine), posDist(mtEngine), posDist(mtEngine) - 20.0f)
				);
			}
		}

		model = Ray::Model("Resources/nanosuit.obj", face_colors, *Ray::Application::Get().GetMainWindow().GetRenderer3D());
		Ray::Application::Get().GetMainWindow().GetRenderer3D()->SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f));
	}
	
	Ray::Point2 mousePos{};
	void GUILayer::OnUpdate(Ray::Timestep ts)
	{
		Ray::Window& window = Ray::Application::Get().GetMainWindow();
		Ray::Renderer3D& renderer = *window.GetRenderer3D();
		renderer.ClearBuffer(0.07f, 0.0f, 0.12f);
		
		static float pitch = 0.0f;
		static float yaw = 0.0f;
		static float roll = 0.0f;
		static float xDir = 0.0f;
		static float yDir = 0.0f;
		static float zDir = 0.0f;

		mousePos = window.Mouse.GetPos();

		Ray::KeyboardInput& kbd = Ray::Application::Get().GetMainWindow().Keyboard;
		if (kbd.IsKeyPressed('S'))
		{
			zDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed('W'))
		{
			zDir += 3.0f * ts;
		}
		if (kbd.IsKeyPressed(' '))
		{
			yDir += 3.0f * ts;
		}
		if (kbd.IsKeyPressed('A'))
		{
			xDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed(17)) //LCONTROL
		{
			yDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed('D'))
		{
			xDir += 3.0f * ts;
		}
		if (kbd.IsKeyPressed('Q'))
		{
			cam.roll += 3.0f * ts;
		}
		if (kbd.IsKeyPressed('E'))
		{
			cam.roll -= 3.0f * ts;
		}

		pitch += 0.5f * ts;
		yaw += 0.5f * ts;
		roll += 0.5f * ts;

		renderer.SetCamera(cam.GetMatrix());

		if constexpr (numCubes != 0)
		{
			for (uint32_t i = 0; i < cubes.size() - 1; ++i)
			{
				cubes[i].SetTransform(
					DirectX::XMMatrixRotationRollPitchYaw(pitch + i, yaw + i, roll + i) *
					cubes[i].GetTranslation()
				);
			}

			cubes.back().SetTransform(
				DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
				DirectX::XMMatrixTranslation(xDir, yDir, zDir)
			);

			// Takes the most amount of time here!
			for (auto& cube : cubes)
			{
				cube.Draw(&renderer);
			}
		}

		model.SetTransform(
			DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(xDir, yDir, zDir)
		);

		//model.Draw(&renderer);

		renderer.EndDraw();
		g_FPS.Update();
	}

	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::MouseMoveEvent& e)
	{
		//static std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevTime).count() << '\n';
		//prevTime = std::chrono::high_resolution_clock::now();
		auto& mouse = Ray::Application::Get().GetMainWindow().Mouse;
		auto& kbd = Ray::Application::Get().GetMainWindow().Keyboard;
		if (mouse.IsMiddlePressed())
		{
			auto mouseDiff = mouse.GetPos() - mousePos;
			if (kbd.IsKeyPressed(16)) // LSHIFT
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

		RAY_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::WindowCloseEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}
	
	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::KeyPressedEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::MouseWheelUpEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
		cam.z -= 1.0f;
	}

	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::MouseWheelDownEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
		cam.z += 1.0f;
	}

	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::MouseWheelLeftEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::MouseWheelRightEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		RAY_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

}
