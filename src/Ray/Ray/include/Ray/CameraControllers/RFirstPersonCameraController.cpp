#include "../Rpch.h"
#include "RFirstPersonCameraController.h"


namespace At0::Ray
{
	void FirstPersonCameraController::OnKeyboardUpdate(const KeyboardInput& kbd, Camera& cam, Timestep ts)
	{
		if (kbd.IsKeyPressed(Ray::Key::W))
		{
			cam.Translate({ 0.0f, 0.0f, ts });
		}
		if (kbd.IsKeyPressed(Ray::Key::A))
		{
			cam.Translate({ -ts, 0.0f, 0.0f });
		}
		if (kbd.IsKeyPressed(Ray::Key::S))
		{
			cam.Translate({ 0.0f, 0.0f, -ts });
		}
		if (kbd.IsKeyPressed(Ray::Key::D))
		{
			cam.Translate({ ts, 0.0f, 0.0f });
		}
		if (kbd.IsKeyPressed(Ray::Key::Space))
		{
			cam.Translate({ 0.0f,ts,0.0f });
		}
		if (kbd.IsKeyPressed(Ray::Key::LeftControl))
		{
			cam.Translate({ 0.0f,-ts,0.0f });
		}
		if (kbd.IsKeyPressed(Ray::Key::Q))
		{
			cam.Rotate(0.0f, 0.0f, -1000.0f * ts);
		}
		if (kbd.IsKeyPressed(Ray::Key::E))
		{
			cam.Rotate(0.0f, 0.0f, 1000.0f * ts);
		}


	}

	void FirstPersonCameraController::OnMouseUpdate(const Point2& rawDelta, Camera& cam)
	{
		if (rawDelta.x != 0 || rawDelta.y != 0)
		{
			cam.Rotate(rawDelta.x, rawDelta.y);
		}
	}
}


