#include "glpch.h"
#include "GUILayer.h"

// You need to include this to use the logger (QUESTION: But you shouldn't need to!)
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>

#include <Ray/Ray.h>
#include <RayRender/Renderer3D.h>
#include <RayRender/Drawable/Triangle.h>
#include <RayRender/Drawable/Cube.h>




class Interval
{
public:
	// Ctor
	Interval()
		: m_Initial(GetTickCount64())
	{
	}

	uint64_t Value() const
	{
		return GetTickCount64() - m_Initial;
	}

private:
	uint64_t m_Initial;
};


class FPS
{
protected:
	uint64_t m_FPS;
	uint64_t m_FPSCount;
	Interval m_FPSInterval;

public:
	FPS()
		: m_FPS(0), m_FPSCount(0)
	{
	}

	void Update()
	{
		// increase the counter by one
		m_FPSCount++;

		// one second elapsed? (= 1000 milliseconds)
		if (m_FPSInterval.Value() > 1000)
		{
			// save the current counter value to m_fps
			m_FPS = m_FPSCount;

			// reset the counter and the interval
			m_FPSCount = 0;
			m_FPSInterval = Interval();

			std::ostringstream oss;
			oss << Get() << " FPS";
			At0::Ray::Application::Get().GetMainWindow().SetTitle(oss.str());
		}
	}

	uint64_t Get() const
	{
		return m_FPS;
	}
};

FPS g_FPS;




namespace At0::Layers
{
	std::vector<Ray::Cube> cubes;
	static constexpr uint64_t numCubes = 10000;
	std::mt19937 mtEngine;

	// QUESTION: The way to get the MainWindow is too long? "Ray::Application::Get().GetMainWindow()"
	// I can only listen to events from one window (MouseMoveEvent from only MainWindow
	GUILayer::GUILayer(const std::string_view name)
		: Ray::Layer(name),
		EventListener<Ray::MouseMoveEvent>(Ray::Application::Get().GetMainWindow()),
		EventListener<Ray::WindowCloseEvent>(Ray::Application::Get().GetMainWindow()),
		EventListener<Ray::KeyPressedEvent>(Ray::Application::Get().GetMainWindow())

		//EventListener<Ray::MouseMoveEvent>(Ray::Application::Get().GetMainWindow()) // !!!!!!!!!!!!1
	{
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

		// TODO: Need to reserve because ConstantBuffer takes reference to Drawable(Cube) parent
		// and the memory address changes when std::vector reallocates
		cubes.reserve(numCubes);
		for (uint32_t i = 0; i < numCubes - 1; ++i)
		{
			cubes.emplace_back(*Ray::Application::Get().GetMainWindow().GetRenderer3D(), 1.0f, face_colors);
		}

		cubes.emplace_back(*Ray::Application::Get().GetMainWindow().GetRenderer3D(), 1.0f, face_colors);
	}
	
	void GUILayer::OnUpdate(Ray::Timestep ts)
	{
		auto& renderer = *Ray::Application::Get().GetMainWindow().GetRenderer3D();
		
		static float pitch = 0.0f;
		static float yaw = 0.0f;
		static float roll = 0.0f;
		static float xDir = 0.0f;
		static float yDir = 0.0f;
		static float zDir = 5.0f;

		renderer.ClearBuffer(0.07f, 0.0f, 0.12f);

		Ray::KeyboardInput& kbd = Ray::Application::Get().GetMainWindow().Keyboard;
		if (kbd.IsKeyPressed(17)) //LCONTROL
		{
			zDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed(16)) //LSHIFT
		{
			zDir += 3.0f * ts;
		}
		if (kbd.IsKeyPressed('W'))
		{
			yDir += 3.0f * ts;
		}
		if (kbd.IsKeyPressed('A'))
		{
			xDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed('S'))
		{
			yDir -= 3.0f * ts;
		}
		if (kbd.IsKeyPressed('D'))
		{
			xDir += 3.0f * ts;
		}

		pitch += 0.5f * ts;
		yaw += 0.5f * ts;
		roll += 0.5f * ts;

		for (uint32_t i = 0; i < cubes.size() - 1; ++i)
		{
			cubes[i].SetTransform(
				DirectX::XMMatrixRotationRollPitchYaw(pitch + i, yaw + i, roll + i) *
				DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f)
			);
		}
		
		cubes.back().SetTransform(
			DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(xDir, yDir, zDir)
		);

		for (auto& cube : cubes)
		{
			cube.Draw(&renderer);
		}

		renderer.EndDraw();
		g_FPS.Update();
	}

	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::MouseMoveEvent& e)
	{
		//static std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevTime).count() << '\n';
		//prevTime = std::chrono::high_resolution_clock::now();

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
}
