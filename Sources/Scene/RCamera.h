﻿#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../RBase.h"
#include "../Events/REventListener.h"
#include "../Events/RMouseEvents.h"
#include "../Events/RKeyboardEvents.h"
#include "../Core/RTime.h"
#include "../Core/RMath.h"

#include "../Graphics/Core/RBindable.h"


namespace At0::Ray
{
	class Uniform;

	class RAY_EXPORT Camera :
		public Bindable,
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
		Float3 Rotation{};
		Float3 Position{};
		Float4 ViewPos{};

		float RotationSpeed = 1.0f, MovementSpeed = 1.0f;

		bool Updated = false;
		bool FlipY = true;
		CameraType Type = CameraType::FirstPerson;

		struct
		{
			Matrix Projection = MatrixIdentity();
			Matrix View = MatrixIdentity();
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

		void CmdBind(const CommandBuffer& cmdBuff) const override;

	private:
		void UpdateViewMatrix();
		void UpdateUniform();

		void OnEvent(MouseMovedEvent& e) override;
		void OnEvent(KeyPressedEvent& e) override;
		void OnEvent(KeyReleasedEvent& e) override;

	private:
		float m_FoV;
		float m_NearZ, m_FarZ;

		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorPool m_DescriptorPool;
		VkPipelineLayout m_PipelineLayout;

		Scope<Uniform> m_Uniform;
	};
}  // namespace At0::Ray