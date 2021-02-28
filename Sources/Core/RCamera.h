#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../RBase.h"
#include "../Events/REventListener.h"
#include "../Events/RMouseEvents.h"
#include "../Events/RKeyboardEvents.h"
#include "RTime.h"


namespace At0::Ray
{
	class RAY_EXPORT Camera :
		EventListener<MouseMovedEvent>,
		EventListener<KeyPressedEvent>,
		EventListener<KeyReleasedEvent>
	{
	public:
		enum CameraType
		{
			LookAt,
			FirstPerson
		};

	public:
		static Camera& Get()
		{
			static Camera cam;
			return cam;
		}

		glm::vec3 Rotation{};
		glm::vec3 Position{};
		glm::vec4 ViewPos{};

		float RotationSpeed = 1.0f, MovementSpeed = 1.0f;

		bool Updated = false;
		bool FlipY = true;
		CameraType Type = CameraType::FirstPerson;

		struct
		{
			glm::mat4 Perspective{};
			glm::mat4 View{};
		} Matrices{};

		struct
		{
			bool Left = false;
			bool Right = false;
			bool Forward = false;
			bool Backward = false;
			bool Up = false;
			bool Down = false;
		} Keys{};

	public:
		bool IsMoving() const;
		float GetNearClip() const { return m_NearZ; }
		float GetFarClip() const { return m_FarZ; }
		void SetPerspective(float fov, float aspect, float nearZ, float farZ);
		void UpdateAspectRatio(float aspect);
		void SetPosition(glm::vec3 pos);
		void SetRotation(glm::vec3 rotation);
		void Rotate(glm::vec3 delta);
		void Translate(glm::vec3 delta);
		void SetRotationSpeed(float speed) { RotationSpeed = speed; }
		void SetMovementSpeed(float speed) { MovementSpeed = speed; }
		void Update(Delta dt);

	private:
		Camera() = default;
		void UpdateViewMatrix();

		void OnEvent(MouseMovedEvent& e) override;
		void OnEvent(KeyPressedEvent& e) override;
		void OnEvent(KeyReleasedEvent& e) override;

	private:
		float m_FoV;
		float m_NearZ, m_FarZ;
	};
}  // namespace At0::Ray
