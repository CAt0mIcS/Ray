#pragma once

#include "Ray/RBase.h"
#include "Ray/Utils/RNonCopyable.h"
#include "../Core/RSharedBindable.h"

#include <RayRenderer/Pipeline/RPipeline.h>
#include <RayRenderer/Pipeline/RDescriptor.h>


namespace At0::Ray
{
	class Shader;

	class RAY_EXPORT Pipeline : public SharedBindable
	{
	public:
		enum class BindPoint
		{
			Graphics = RrPipelineBindPointGraphics,
			Compute = RrPipelineBindPointCompute,
			RayTracing = RrPipelineBindPointRayTracingKHR
		};

	public:
		virtual ~Pipeline();

		virtual RrDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const;
		virtual RrDescriptorPool GetDescriptorPool() const { return nullptr; };
		virtual Pipeline::BindPoint GetBindPoint() const = 0;
		void CmdBind(const CommandBuffer& cmdBuff) const override;

		const Shader& GetShader() const { return *m_Shader; }
		RrPipelineLayout GetLayout() const { return m_Layout; }
		operator RrPipeline() const { return m_Pipeline; }

	protected:
		Pipeline(Ref<Shader> shader);

	protected:
		RrPipeline m_Pipeline = nullptr;
		RrPipelineLayout m_Layout = nullptr;

		Ref<Shader> m_Shader;
	};
}  // namespace At0::Ray
