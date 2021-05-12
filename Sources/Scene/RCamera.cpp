#include "Rpch.h"
#include "RCamera.h"

#include "Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"

#include "Utils/RException.h"

#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	bool Camera::IsMoving() const
	{
		return Keys.Left || Keys.Right || Keys.Up || Keys.Down || Keys.Forward || Keys.Backward;
	}

	void Camera::SetPerspective(float fov, float aspect, float nearZ, float farZ)
	{
		m_FoV = fov;
		m_NearZ = nearZ;
		m_FarZ = farZ;
		ShaderData.Projection = glm::perspective(Radians(fov), aspect, nearZ, farZ);
		if (FlipY)
		{
			ShaderData.Projection[1][1] *= -1.0f;
		}
		UpdateViewMatrix();
	}

	void Camera::UpdateAspectRatio(float aspect)
	{
		ShaderData.Projection = glm::perspective(Radians(m_FoV), aspect, m_NearZ, m_FarZ);
		if (FlipY)
		{
			ShaderData.Projection[1][1] *= -1.0f;
		}
		UpdateViewMatrix();
	}

	void Camera::SetPosition(Float3 pos)
	{
		Position = pos;
		UpdateViewMatrix();
	}

	void Camera::SetRotation(Float3 rotation)
	{
		Rotation = rotation;
		UpdateViewMatrix();
	}

	void Camera::Rotate(Float3 delta)
	{
		Rotation += delta;
		UpdateViewMatrix();
	}

	void Camera::Translate(Float3 delta)
	{
		Position += delta;
		UpdateViewMatrix();
	}

	void Camera::Update(Delta dt)
	{
		Updated = false;

		if (Type == CameraType::FirstPerson)
		{
			if (IsMoving())
			{
				Float3 camFront;
				camFront.x = -cos(Radians(Rotation.x)) * sin(Radians(Rotation.y));
				camFront.y = sin(Radians(Rotation.x));
				camFront.z = cos(Radians(Rotation.x)) * cos(Radians(Rotation.y));
				camFront = Normalize(camFront);

				float moveSpeed = dt * MovementSpeed;

				if (Keys.Forward)
				{
					Position += camFront * moveSpeed;
				}
				if (Keys.Backward)
				{
					Position -= camFront * moveSpeed;
				}
				if (Keys.Left)
				{
					Position -=
						Normalize(CrossProduct(camFront, Float3(0.0f, 1.0f, 0.0f))) * moveSpeed;
				}
				if (Keys.Right)
				{
					Position +=
						Normalize(CrossProduct(camFront, Float3(0.0f, 1.0f, 0.0f))) * moveSpeed;
				}
				if (Keys.Up)
				{
					Position += Normalize(CrossProduct(
									camFront, CrossProduct(Float3(0.0f, 1.0f, 0.0f), camFront))) *
								moveSpeed;
				}
				if (Keys.Down)
				{
					Position -= Normalize(CrossProduct(
									camFront, CrossProduct(Float3(0.0f, 1.0f, 0.0f), camFront))) *
								moveSpeed;
				}

				UpdateViewMatrix();
			}
		}
	}

	Camera::Camera() {}

	Camera::~Camera() {}

	void Camera::UpdateViewMatrix()
	{
		Matrix rotM = MatrixIdentity();
		Matrix transM;

		rotM = glm::rotate(
			rotM, Radians(Rotation.x * (FlipY ? -1.0f : 1.0f)), Float3(1.0f, 0.0f, 0.0f));
		rotM = glm::rotate(rotM, Radians(Rotation.y), Float3(0.0f, 1.0f, 0.0f));
		rotM = glm::rotate(rotM, Radians(Rotation.z), Float3(0.0f, 0.0f, 1.0f));

		Float3 translation = Position;
		if (FlipY)
		{
			translation.y *= -1.0f;
		}
		transM = MatrixTranslation(translation);

		if (Type == CameraType::FirstPerson)
		{
			ShaderData.View = rotM * transM;
		}
		else
		{
			ShaderData.View = transM * rotM;
		}

		// ShaderData.ViewPos = Float4(Position, 0.0f) * Float4(-1.0f, 1.0f, -1.0f, 1.0f);
		ShaderData.ViewPos = Position * Float3(-1.0f, 1.0f, -1.0f);

		Updated = true;

		DispatchCameraChanged(CameraChangedEvent{});
	}

	void Camera::DispatchCameraChanged(CameraChangedEvent e)
	{
		for (auto* listener : EventDispatcher<CameraChangedEvent>::Get())
			listener->OnEvent(e);
	}

	void Camera::OnEvent(MouseMovedEvent& e)
	{
		if (!Window::Get().CursorEnabled())
			Rotate(Float3{ (float)e.GetDelta().y, (float)-e.GetDelta().x, 0.0f } * RotationSpeed);
	}

	void Camera::OnEvent(KeyPressedEvent& e)
	{
		if (e.GetKey() == Key::Escape)
		{
			if (Window::Get().CursorEnabled())
				Window::Get().DisableCursor();
			else
				Window::Get().EnableCursor();
		}

		if (e.GetKey() == Key::LeftShift)
			SetMovementSpeed(MovementSpeed * 5.0f);

		if (this->Type == Camera::FirstPerson)
		{
			switch (e.GetKey())
			{
			case Key::W: Keys.Forward = true; break;
			case Key::S: Keys.Backward = true; break;
			case Key::A: Keys.Left = true; break;
			case Key::D: Keys.Right = true; break;
			case Key::Space: Keys.Up = true; break;
			case Key::LeftControl: Keys.Down = true; break;
			}
		}
	}

	void Camera::OnEvent(KeyReleasedEvent& e)
	{
		if (e.GetKey() == Key::LeftShift)
			SetMovementSpeed(MovementSpeed / 5.0f);

		if (this->Type == Camera::FirstPerson)
		{
			switch (e.GetKey())
			{
			case Key::W: Keys.Forward = false; break;
			case Key::S: Keys.Backward = false; break;
			case Key::A: Keys.Left = false; break;
			case Key::D: Keys.Right = false; break;
			case Key::Space: Keys.Up = false; break;
			case Key::LeftControl: Keys.Down = false; break;
			}
		}
	}
}  // namespace At0::Ray
