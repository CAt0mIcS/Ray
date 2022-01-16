#include "RCamera.h"

#include "Devices/RWindow.h"
#include "Devices/RKeyboard.h"
#include "Devices/RMouse.h"


namespace At0::Ray
{
	bool Camera::IsMoving() const
	{
		return Keyboard::IsKeyPressed(Key::W) || Keyboard::IsKeyPressed(Key::A) ||
			   Keyboard::IsKeyPressed(Key::S) || Keyboard::IsKeyPressed(Key::D) ||
			   Keyboard::IsKeyPressed(Key::Space) || Keyboard::IsKeyPressed(Key::LeftControl);
	}

	void Camera::SetPerspective(float fov, float aspect, float nearZ, float farZ)
	{
		m_FoV = fov;
		m_NearZ = nearZ;
		m_FarZ = farZ;

		ShaderData.Projection = glm::perspective(Radians(fov), aspect, nearZ, farZ);
		if (FlipY)
			ShaderData.Projection[1][1] *= -1.0f;
		UpdateViewMatrix();
	}

	void Camera::SetOrthographic(
		float left, float right, float top, float bottom, float nearZ, float farZ)
	{
		m_FoV = -1.f;
		m_Left = left;
		m_Right = right;
		m_Bottom = bottom;
		m_Top = top;
		m_NearZ = nearZ;
		m_FarZ = farZ;

		UpdateOrthographic();

		UpdateViewMatrix();
	}

	void Camera::UpdateAspectRatio(float aspect)
	{
		if (IsOrthographic())
			UpdateOrthographic();
		else
		{
			ShaderData.Projection = glm::perspective(Radians(m_FoV), aspect, m_NearZ, m_FarZ);
			if (FlipY)
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

				if (Keyboard::IsKeyPressed(Key::W))
				{
					Position += camFront * moveSpeed;
				}
				if (Keyboard::IsKeyPressed(Key::S))
				{
					Position -= camFront * moveSpeed;
				}
				if (Keyboard::IsKeyPressed(Key::A))
				{
					Position -=
						Normalize(CrossProduct(camFront, Float3(0.0f, 1.0f, 0.0f))) * moveSpeed;
				}
				if (Keyboard::IsKeyPressed(Key::D))
				{
					Position +=
						Normalize(CrossProduct(camFront, Float3(0.0f, 1.0f, 0.0f))) * moveSpeed;
				}
				if (Keyboard::IsKeyPressed(Key::Space))
				{
					Position += Normalize(CrossProduct(
									camFront, CrossProduct(Float3(0.0f, 1.0f, 0.0f), camFront))) *
								moveSpeed;
				}
				if (Keyboard::IsKeyPressed(Key::LeftControl))
				{
					Position -= Normalize(CrossProduct(
									camFront, CrossProduct(Float3(0.0f, 1.0f, 0.0f), camFront))) *
								moveSpeed;
				}

				UpdateViewMatrix();
			}
		}
	}

	Camera::Camera()
		: EventListener<MouseMovedEvent>(Window::Get()),
		  EventListener<KeyPressedEvent>(Window::Get()), EventListener<KeyReleasedEvent>(
															 Window::Get()),
		  EventListener<ScrollUpEvent>(Window::Get()), EventListener<ScrollDownEvent>(Window::Get())
	{
	}

	Camera::~Camera() {}

	void Camera::UpdateViewMatrix()
	{
		if (IsOrthographic())
		{
			// Float3 translation = Position;
			// if (FlipY)
			//	translation.y *= -1.f;

			ShaderData.View =
				glm::lookAt(Float3{ 0.f, 0.f, 1.f }, Float3{ 0.f }, Float3{ 0.f, 1.f, 0.f });
			// ShaderData.View = glm::lookAt(Float3(translation.x, translation.y, 1.f),
			//	Float3(0.f, 0.f, 0.f), Float3(0.f, 1.f, 0.f));
		}
		else
		{
			Matrix rotM = MatrixIdentity();
			Matrix transM;

			rotM = glm::rotate(
				rotM, Radians(Rotation.x * (FlipY ? -1.0f : 1.0f)), Float3(1.0f, 0.0f, 0.0f));
			rotM = glm::rotate(rotM, Radians(Rotation.y), Float3(0.0f, 1.0f, 0.0f));
			rotM = glm::rotate(rotM, Radians(Rotation.z), Float3(0.0f, 0.0f, 1.0f));
			if (Type == CameraType::LookAt)
			{
				Matrix transToPivot = glm::translate(MatrixIdentity(), -Pivot);
				Matrix transFromPivot = glm::translate(glm::mat4(1.0f), Pivot);
				rotM = transFromPivot * rotM * transToPivot;
			}

			Float3 translation = Position;
			if (FlipY)
				translation.y *= -1.0f;
			transM = MatrixTranslation(translation);

			if (Type == CameraType::FirstPerson)
				ShaderData.View = rotM * transM;
			else
				ShaderData.View = transM * rotM;
		}

		// ShaderData.ViewPos = Float4(Position, 0.0f) * Float4(-1.0f, 1.0f, -1.0f, 1.0f);
		ShaderData.ViewPos = Position * Float3(-1.0f, 1.0f, -1.0f);

		Updated = true;
		if (!FreezeFrustum)
			m_Frustum.Update(ShaderData.Projection * ShaderData.View);

		DispatchCameraChanged(CameraChangedEvent{});
	}

	void Camera::DispatchCameraChanged(CameraChangedEvent e)
	{
		for (auto* listener : EventDispatcher<CameraChangedEvent>::Get())
			listener->OnEvent(e);
	}

	void Camera::OnEvent(MouseMovedEvent& e)
	{
		if (Type == CameraType::LookAt && Mouse::IsMiddlePressed())
		{
			if (Keyboard::IsKeyPressed(Key::LeftShift))
			{
				Pivot += Float3{ e.GetDelta().x, -e.GetDelta().y, 0.0f } * RotationSpeed;
				Translate(Float3{ -e.GetDelta(), 0.0f } * RotationSpeed);
				return;
			}
			else
				Rotate(Float3{ e.GetDelta().y, -e.GetDelta().x, 0.0f } * RotationSpeed);
		}
		else if (!Window::Get().CursorEnabled())
			Rotate(Float3{ e.GetDelta().y, -e.GetDelta().x, 0.0f } * RotationSpeed);
	}

	void Camera::OnEvent(KeyPressedEvent& e)
	{
		if (Type == CameraType::FirstPerson && e.GetKey() == Key::Escape)
		{
			if (Window::Get().CursorEnabled())
				Window::Get().DisableCursor();
			else
				Window::Get().EnableCursor();
		}

		if (e.GetKey() == Key::LeftShift && !Mouse::IsMiddlePressed())
			SetMovementSpeed(MovementSpeed * 5.0f);
	}

	void Camera::OnEvent(KeyReleasedEvent& e)
	{
		if (e.GetKey() == Key::LeftShift)
			SetMovementSpeed(MovementSpeed / 5.0f);
	}

	void Camera::OnEvent(ScrollUpEvent& e)
	{
		if (Type == Camera::LookAt)
			Translate(Float3(0.0f, 0.0f, (float)e.GetOffset().y * 0.3f) * MovementSpeed);
	}

	void Camera::OnEvent(ScrollDownEvent& e)
	{
		if (Type == Camera::LookAt)
			Translate(Float3(0.0f, 0.0f, (float)e.GetOffset().y * 0.3f) * MovementSpeed);
	}

	void Camera::UpdateOrthographic()
	{
		ShaderData.Projection = Matrix{ 1.f };
		ShaderData.Projection[0][0] = 2.f / (m_Right - m_Left);
		ShaderData.Projection[1][1] = 2.f / (m_Bottom - m_Top);
		ShaderData.Projection[2][2] = 1.f / (m_FarZ - m_NearZ);
		ShaderData.Projection[3][0] = -(m_Right + m_Left) / (m_Right - m_Left);
		ShaderData.Projection[3][1] = -(m_Bottom + m_Top) / (m_Bottom - m_Top);
		ShaderData.Projection[3][2] = -m_NearZ / (m_FarZ - m_NearZ);
	}
}  // namespace At0::Ray
