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
	GUILayer::GUILayer(const std::string_view name)
		: Reyal::Layer(name)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] Startup");
	}

	void GUILayer::OnMouseMove(Reyal::Widget* receiver, Reyal::MouseMoveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] [{0}]: {1}", receiver->GetName(), e.ToString());
	}
	
	void GUILayer::OnUpdate(Reyal::Timestep ts)
	{
		auto& renderer = *Reyal::Application::Get().GetMainWindow().GetRenderer3D();

		renderer.ClearBuffer(0.07f, 0.0f, 0.12f);
		//Reyal::Triangle triangle;
		//triangle.Draw(&renderer);
		static float pitch = 0.0f;
		static float yaw = 0.0f;
		static float roll = 0.0f;
		static float zDir = 5.0f;


		Reyal::KeyboardInput& kbd = Reyal::Application::Get().GetMainWindow().Keyboard;
		if (kbd.IsKeyPressed(17)) //LCONTROL
		{
			zDir -= 0.1f;
		}
		else if (kbd.IsKeyPressed(16)) //LSHIFT
		{
			zDir += 0.1f;
		}


		std::mt19937 mtEngine;
		std::uniform_real_distribution<float> pitchDist(0.0f, 0.01f);
		std::uniform_real_distribution<float> yawDist(0.0f, 0.01f);
		std::uniform_real_distribution<float> rollDist(0.0f, 0.01f);

		pitch += pitchDist(mtEngine);
		yaw += yawDist(mtEngine);
		roll += rollDist(mtEngine);

		mtEngine.seed((uint32_t)time(0));
		std::uniform_real_distribution<float> colDist01(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist02(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist03(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist04(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist05(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist06(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist07(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist08(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist09(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist10(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist11(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist12(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist13(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist14(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist15(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist16(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist17(0.0f, 1.0f);
		std::uniform_real_distribution<float> colDist18(0.0f, 1.0f);

		float face_colors[6][3] =
		{
			{ colDist01(mtEngine), colDist02(mtEngine), colDist03(mtEngine) },
			{ colDist04(mtEngine), colDist05(mtEngine), colDist06(mtEngine) },
			{ colDist07(mtEngine), colDist08(mtEngine), colDist09(mtEngine) },
			{ colDist10(mtEngine), colDist11(mtEngine), colDist12(mtEngine) },
			{ colDist13(mtEngine), colDist14(mtEngine), colDist15(mtEngine) },
			{ colDist16(mtEngine), colDist17(mtEngine), colDist18(mtEngine) }
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

		Reyal::Cube cube(renderer, 1.0f, face_colors);
		cube.SetTransform(
			DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, zDir)
		);
		cube.Draw(&renderer);

		Reyal::Cube cube2(renderer, 1.0f, face_colors);
		cube2.SetTransform(
			DirectX::XMMatrixRotationRollPitchYaw(pitch + 1.0f, yaw + 1.0f, roll + 1.0f) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f)
		);
		cube2.Draw(&renderer);
		
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
