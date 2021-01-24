#pragma once

#include "../RBase.h"
#include "../Input/RKeyboard.h"
#include "../Input/RMouse.h"
#include "../Core/RTimestep.h"

#include <../../RayRender/include/RayRender/Core/RCamera.h>


namespace At0::Ray
{
	class RAY_API EditorCameraController
	{
	public:
		EditorCameraController() = default;
		void OnKeyboardUpdate(const KeyboardInput& kbd, Camera& cam, Timestep ts);
		void OnMouseUpdate(const MouseInput& mouse, Camera& cam);
	};
}