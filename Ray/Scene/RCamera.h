#pragma once

#include "../RBase.h"
#include "../Events/REventListener.h"
#include "../Events/REventDispatcher.h"
#include "../Events/RMouseEvents.h"
#include "../Events/RKeyboardEvents.h"
#include "../Events/RCustomEvents.h"
#include "../Core/RTime.h"
#include "../Core/RMath.h"
#include "../Core/RFrustum.h"


namespace At0::Ray
{
	class BufferUniform;

	class RAY_EXPORT Camera :
		public EventDispatcher<CameraChangedEvent>,
		EventListener<MouseMovedEvent>,
		EventListener<KeyPressedEvent>,
		EventListener<KeyReleasedEvent>,
		EventListener<ScrollUpEvent>,
		EventListener<ScrollDownEvent>
	{
	public:
		enum CameraType
		{
			LookAt,
			FirstPerson
		};

	public:
		Float3 Rotation{};
		Float3 Position{};
		Float3 Pivot{};

		float RotationSpeed = 1.0f, MovementSpeed = 1.0f;

		bool Updated = false;
		bool FlipY = true;
		bool FreezeFrustum = false;
		CameraType Type = CameraType::FirstPerson;

		struct Data
		{
			Matrix View = MatrixIdentity();
			Matrix Projection = MatrixIdentity();
			Float3 ViewPos{};
		} ShaderData{};

	public:
		Camera();
		~Camera();
		bool IsMoving() const;
		float GetNearClip() const { return m_NearZ; }
		float GetFarClip() const { return m_FarZ; }
		void SetPerspective(float fov, float aspect, float nearZ, float farZ);
		void UpdateAspectRatio(float aspect);
		void SetPosition(Float3 pos);
		void SetRotation(Float3 rotation);
		void Rotate(Float3 delta);
		void Translate(Float3 delta);
		void SetRotationSpeed(float speed) { RotationSpeed = speed; }
		void SetMovementSpeed(float speed) { MovementSpeed = speed; }
		void Update(Delta dt);

		const Frustum& GetFrustum() const { return m_Frustum; }

	private:
		void UpdateViewMatrix();
		void DispatchCameraChanged(CameraChangedEvent e);

		void OnEvent(MouseMovedEvent& e) override;
		void OnEvent(KeyPressedEvent& e) override;
		void OnEvent(KeyReleasedEvent& e) override;
		void OnEvent(ScrollUpEvent& e) override;
		void OnEvent(ScrollDownEvent& e) override;

	private:
		float m_FoV;
		float m_NearZ, m_FarZ;
		Frustum m_Frustum;
	};
}  // namespace At0::Ray
