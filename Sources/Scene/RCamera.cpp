#include "Rpch.h"
#include "RCamera.h"

#include "Graphics/Pipelines/RUniform.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Buffers/RBufferSynchronizer.h"

#include "Utils/RException.h"


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

	void Camera::CmdBind(const CommandBuffer& cmdBuff) const { m_Uniform->CmdBind(cmdBuff); }

	Camera::Camera()
	{
		VkDescriptorSetLayoutBinding cameraBinding{};
		cameraBinding.binding = 0;
		cameraBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		cameraBinding.descriptorCount = 1;
		cameraBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		cameraBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = 1;
		descriptorSetLayoutCreateInfo.pBindings = &cameraBinding;

		RAY_VK_THROW_FAILED(vkCreateDescriptorSetLayout(Graphics::Get().GetDevice(),
								&descriptorSetLayoutCreateInfo, nullptr, &m_DescriptorSetLayout),
			"[Camera] Failed to create descriptor set layout for camera");

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.flags = 0;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		RAY_VK_THROW_FAILED(vkCreatePipelineLayout(Graphics::Get().GetDevice(),
								&pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout),
			"[Camera] Failed to create pipeline layout for camera");

		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.maxSets = 1;
		descriptorPoolCreateInfo.poolSizeCount = 1;
		descriptorPoolCreateInfo.pPoolSizes = &poolSize;

		RAY_VK_THROW_FAILED(vkCreateDescriptorPool(Graphics::Get().GetDevice(),
								&descriptorPoolCreateInfo, nullptr, &m_DescriptorPool),
			"[Camera] Failed to create descriptor pool");

		m_Uniform = MakeScope<Uniform>(m_DescriptorSetLayout, m_DescriptorPool,
			Pipeline::BindPoint::Graphics, m_PipelineLayout, uint32_t(sizeof(ShaderData)),
			0,	// Using descriptor set 0 for per-scene data (set=0 in vs)
			std::vector{ 0u });
	}

	Camera::~Camera()
	{
		vkDestroyDescriptorSetLayout(Graphics::Get().GetDevice(), m_DescriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(Graphics::Get().GetDevice(), m_DescriptorPool, nullptr);
		vkDestroyPipelineLayout(Graphics::Get().GetDevice(), m_PipelineLayout, nullptr);
	}

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

		UpdateUniform();
	}

	void Camera::UpdateUniform()
	{
		BufferSynchronizer::Get().Update(ShaderData, m_Uniform->GetGlobalBufferOffset());
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
