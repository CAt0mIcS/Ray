#include "glpch.h"
#include "GUILayer.h"

// QUESTION: Should I add all includes that the client might need into Ray.h or let them include them on their own (but they will need to set additional include paths to other libraries)
#include <RayDebug/RLogger.h>
#include <RayDebug/RInstrumentor.h>

#include <Ray/Ray.h>
#include <Ray/Window/ROpenGLWindow.h>

#ifdef __linux__
#include <time.h>
#endif


class FPS
{
public:
	FPS()
		: m_FPS(0), m_FPSCount(0)
#ifdef _WIN32
		, m_InitialInterval(GetTickCount64())
#endif
	{
#ifdef __linux__
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		m_InitialInterval = ts.tv_nsec;
#endif
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
#ifdef _WIN32
			m_InitialInterval = GetTickCount64();
#elif defined(__linux__)
			timespec ts;
			clock_gettime(CLOCK_MONOTONIC, &ts);
			m_InitialInterval = ts.tv_nsec / 1000000;
#endif

			std::ostringstream oss;
			oss << GetFPS() << " FPS";
			// RAY_TODO: Abort when trying to set the title on an unopened window!
#ifdef _WIN32
			At0::Ray::Application::Get().GetMainWindow().SetTitle(oss.str());
#endif
		}
	}

	uint32_t GetFPS() const
	{
		return m_FPS;
	}

	uint32_t IntervalValue() const
	{
#ifdef _WIN32
		return GetTickCount64() - m_InitialInterval;
#elif defined(__linux__)
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return (ts.tv_nsec / 1000000) - m_InitialInterval;
#endif
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

	static constexpr uint64_t AmountOfCubes = 1;
	Ray::Ref<Ray::Quad> Quad;

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
		EventListener<Ray::MouseWheelRightEvent>(GetMainWindow()),
		EventListener<Ray::MouseMoveEvent>(GetMainWindow())
	{
		RAY_PROFILE_FUNCTION();

#if RENDER
		if constexpr (AmountOfCubes > 0)
		{
			for (uint64_t i = 0; i < AmountOfCubes; ++i)
			{
				// Create quad with name Quad[i]
				Ray::Ref<Ray::Quad> quad = Ray::MakeRef<Ray::Quad>(m_CubeScene.CreateEntity("Quad" + std::to_string(i)));
				Quad = quad;
				// Set transformation (RAY_TODO: TransformConstantBuffers)
				// This step is optional
				Ray::TransformComponent& tform = quad->GetComponent<Ray::TransformComponent>();
				tform.Rotation = Ray::Quaternion::RotationRollPitchYaw(0.0f, 1.0f, 0.0f);
				tform.Translation.z = 1.0f;

				//// Overwrite existing basic vertex shader with my own.
				//// This step is optional
				//Ray::VertexShaderComponent& vShader = quad.GetComponent<Ray::VertexShaderComponent>();
				//vShader.Shader = Ray::VertexShader::CreateFromSource("shaderFilePath");

				//// Overwrite existing basic pixel shader with my own.
				//// This step is optional
				//Ray::PixelShaderComponent& pShader = quad.GetComponent<Ray::PixelShaderComponent>();
				//pShader.Shader = Ray::PixelShader::CreateFromSource("shaderFilePath");


				// Add texture to the quad using ecs
				//quad.AddComponent<TextureComponent>("path");

				// Add it to the scene for rendering later on
				m_CubeScene.Submit(quad);
			}
		}
#endif
	}

	Ray::Point2 mousePos{};
	void GUILayer::OnUpdate(Ray::Timestep ts)
	{
#if RENDER
		Ray::Ref<Ray::Renderer3D> renderer = GetMainWindow().GetRenderer3D();
		renderer->ClearBuffer(0.07f, 0.0f, 0.12f);

		renderer->Draw(m_CubeScene);
		renderer->EndDraw();

#endif

		g_FPS.Update();
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseMoveEvent& e)
	{
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
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
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::KeyReleasedEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::CharEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseButtonPressedEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseButtonReleasedEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::HoverEnterEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::HoverLeaveEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseWheelUpEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());

		Ray::TransformComponent& tform = Quad->GetComponent<Ray::TransformComponent>();
		--tform.Translation.z;
	}

	void GUILayer::OnEvent(Ray::Widget& receiver, Ray::MouseWheelDownEvent& e)
	{
		RAY_PROFILE_FUNCTION();
		Ray::Log::Debug("[GUILayer] [{0}]: {1}", receiver.GetName(), e.ToString());

		Ray::TransformComponent& tform = Quad->GetComponent<Ray::TransformComponent>();
		++tform.Translation.z;
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
