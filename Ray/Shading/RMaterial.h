#pragma once

#include "Ray/RBase.h"
#include "Ray/Core/RMath.h"


namespace At0::Ray
{
	class GraphicsPipeline;
	class Texture2D;
	class CommandBuffer;

	/**
	 * Specifies strings which identify the different shader configurations
	 */
	struct ShaderFileCode
	{
		static constexpr const char* AlbedoMap = "_Alb";
		static constexpr const char* DiffuseMap = "_Diff";
		static constexpr const char* Metallic = "_Metal";
		static constexpr const char* SpecularMap = "_Spec";
		static constexpr const char* NormalMap = "_Norm";
		static constexpr const char* HeightMap = "_Height";
		static constexpr const char* Occlusion = "_Occl";
		static constexpr const char* DetailMask = "_DtlMsk";
		static constexpr const char* Emission = "_Emi";

		static constexpr const char* Color = "_Col";
	};

	class RAY_EXPORT Material
	{
	public:
		virtual ~Material() = default;

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		GraphicsPipeline& GetGraphicsPipeline() { return *m_GraphicsPipeline; }
		Ref<GraphicsPipeline> GetSharedGraphicsPipeline() const { return m_GraphicsPipeline; }

		/**
		 * Bind the graphics pipeline
		 */
		void CmdBind(const CommandBuffer& cmdBuff) const;

		virtual Float4 GetColor() const { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
		virtual Ref<Texture2D> GetAlbedoMap() const { return nullptr; }
		virtual Ref<Texture2D> GetDiffuseMap() const { return nullptr; }
		virtual Ref<Texture2D> GetSpecularMap() const { return nullptr; }
		virtual Ref<Texture2D> GetNormalMap() const { return nullptr; }
		virtual Ref<Texture2D> GetHeightMap() const { return nullptr; }
		virtual Ref<Texture2D> GetOcclusion() const { return nullptr; }
		virtual Ref<Texture2D> GetDetailMask() const { return nullptr; }

	protected:
		Material();

	protected:
		Ref<GraphicsPipeline> m_GraphicsPipeline;
	};

}  // namespace At0::Ray
