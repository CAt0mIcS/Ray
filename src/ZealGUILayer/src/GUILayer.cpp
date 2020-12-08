#include "glpch.h"
#include "GUILayer.h"

// You need to include this to use the logger
#include <RlDebug/ReyalLogger.h>
#include <RlDebug/Instrumentor.h>

#include <Reyal/Reyal.h>
#include <RlRender/Renderer3D.h>
#include <RlRender/Drawable/Triangle.h>
#include <RlRender/Drawable/Cube.h>


namespace At0::Layers
{
	std::vector<Reyal::Cube> cubes;
	static constexpr uint64_t numCubes = 3000;
	std::mt19937 mtEngine;

	GUILayer::GUILayer(const std::string_view name)
		: Reyal::Layer(name)
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
			cubes.emplace_back(*Reyal::Application::Get().GetMainWindow().GetRenderer3D(), 1.0f, face_colors);
		}

		cubes.emplace_back(*Reyal::Application::Get().GetMainWindow().GetRenderer3D(), 1.0f, face_colors);

	}

	void GUILayer::OnMouseMove(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}
	
	void GUILayer::OnUpdate(Reyal::Timestep ts)
	{
		RL_PROFILE_FUNCTION();
		auto& renderer = *Reyal::Application::Get().GetMainWindow().GetRenderer3D();
		
		static float pitch = 0.0f;
		static float yaw = 0.0f;
		static float roll = 0.0f;
		static float xDir = 0.0f;
		static float yDir = 0.0f;
		static float zDir = 5.0f;

		renderer.ClearBuffer(0.07f, 0.0f, 0.12f);

		Reyal::KeyboardInput& kbd = Reyal::Application::Get().GetMainWindow().Keyboard;
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

	void GUILayer::OnMousePress(Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnMouseRelease(Reyal::Widget* receiver, Reyal::MouseButtonReleasedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnMouseWheelUp(Reyal::Widget* receiver, Reyal::MouseWheelUpEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnMouseWheelDown(Reyal::Widget* receiver, Reyal::MouseWheelDownEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnMouseWheelLeft(Reyal::Widget* receiver, Reyal::MouseWheelLeftEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnMouseWheelRight(Reyal::Widget* receiver, Reyal::MouseWheelRightEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnResize(Reyal::Widget* receiver, Reyal::WindowResizeEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnWindowMove(Reyal::Widget* receiver, Reyal::WindowMoveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnPaint(Reyal::Widget* receiver, Reyal::PaintEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnHoverEnter(Reyal::Widget* receiver, Reyal::HoverEnterEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnHoverLeave(Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnKeyPress(Reyal::Widget* receiver, Reyal::KeyPressedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnKeyRelease(Reyal::Widget* receiver, Reyal::KeyReleasedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}

	void GUILayer::OnChar(Reyal::Widget* receiver, Reyal::CharEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}
	
	bool GUILayer::OnWindowClose(Reyal::Widget* receiver, Reyal::WindowCloseEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
		return false;
	}
}
