#include "glpch.h"
#include "GUILayer.h"

// You need to include this to use the logger
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>

#include <Ray/Ray.h>
#include <RayRender/Renderer3D.h>
#include <RayRender/Drawable/Triangle.h>
#include <RayRender/Drawable/Cube.h>


namespace At0::Layers
{
	std::vector<Ray::Cube> cubes;
	static constexpr uint64_t numCubes = 3000;
	std::mt19937 mtEngine;

	GUILayer::GUILayer(const std::string_view name)
		: Ray::Layer(name),
		EventListener<Ray::MouseMoveEvent>(Ray::Application::Get().GetMainWindow())
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] Startup");

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
	}

	#include <chrono>
	std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
	void GUILayer::OnEvent(Ray::Widget* receiver, Ray::MouseMoveEvent& e)
	{
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevTime).count() << '\n';
		prevTime = std::chrono::high_resolution_clock::now();

		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	//void GUILayer::OnMouseMove(Ray::Widget* receiver, Ray::MouseMoveEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnMousePress(Ray::Widget* receiver, Ray::MouseButtonPressedEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnMouseRelease(Ray::Widget* receiver, Ray::MouseButtonReleasedEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnMouseWheelUp(Ray::Widget* receiver, Ray::MouseWheelUpEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnMouseWheelDown(Ray::Widget* receiver, Ray::MouseWheelDownEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnMouseWheelLeft(Ray::Widget* receiver, Ray::MouseWheelLeftEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnMouseWheelRight(Ray::Widget* receiver, Ray::MouseWheelRightEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnResize(Ray::Widget* receiver, Ray::WindowResizeEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnWindowMove(Ray::Widget* receiver, Ray::WindowMoveEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnPaint(Ray::Widget* receiver, Ray::PaintEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnHoverEnter(Ray::Widget* receiver, Ray::HoverEnterEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnHoverLeave(Ray::Widget* receiver, Ray::HoverLeaveEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnKeyPress(Ray::Widget* receiver, Ray::KeyPressedEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnKeyRelease(Ray::Widget* receiver, Ray::KeyReleasedEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}

	//void GUILayer::OnChar(Ray::Widget* receiver, Ray::CharEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//}
	//
	//bool GUILayer::OnWindowClose(Ray::Widget* receiver, Ray::WindowCloseEvent& e)
	//{
	//	RL_PROFILE_FUNCTION();
	//	RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	//	return false;
	//}
}
