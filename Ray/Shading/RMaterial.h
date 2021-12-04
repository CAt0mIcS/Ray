#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include "RMaterialDataContainer.h"

#include <unordered_map>


namespace At0::Ray
{
	class GraphicsPipeline;
	class Texture2D;
	class CommandBuffer;

	/**
	 * Predefined uniform block tags in the shader
	 */
	struct UniformBlockTag
	{
		static constexpr const char* PerObjectData = "PerObjectData";
		static constexpr const char* Shading = "Shading";
	};

	/**
	 * Predefined uniform tags in the shader
	 */
	struct UniformTag
	{
		static constexpr const char* DiffuseMapSampler = "samplerDiffuse";
		static constexpr const char* SpecularMapSampler = "samplerSpecular";
		static constexpr const char* NormalMapSampler = "samplerNormal";
		static constexpr const char* HeightMapSampler = "samplerHeight";

		static constexpr const char* Color = "color";
	};

	class RAY_EXPORT Material
	{
	public:
		Material(Ref<GraphicsPipeline> pipeline, MaterialDataContainer container);
		~Material() = default;

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		GraphicsPipeline& GetGraphicsPipeline() { return *m_GraphicsPipeline; }
		Ref<GraphicsPipeline> GetSharedGraphicsPipeline() const { return m_GraphicsPipeline; }

		/**
		 * Bind the graphics pipeline
		 */
		void CmdBind(const CommandBuffer& cmdBuff) const;

		bool HasUniformBlock(const std::string& name) const;
		bool HasUniform(const std::string& name) const { return m_Container.HasUniform(name); }
		Ref<Texture2D> GetTexture(const std::string& dataPath) const;
		void* Get(const std::string& dataPath) const { return m_Container.Get(dataPath); }
		ShaderDataType GetType(const std::string& dataPath) const
		{
			return m_Container.GetType(dataPath);
		}

	protected:
		Ref<GraphicsPipeline> m_GraphicsPipeline;
		MaterialDataContainer m_Container;

	public:
		class RAY_EXPORT Builder
		{
		public:
			Builder(Ref<GraphicsPipeline> pipeline);

			template<typename T>
			Builder& Set(const std::string& name, T&& data)
			{
				ValidateUniformExistence(name);
				m_Container.Set(name, std::move(data));
				return *this;
			}

			Builder& Set(const std::string& name, Ref<Texture2D> data);

			Ref<Material> Acquire();

		private:
			void ValidateUniformExistence(const std::string& name);

		private:
			Ref<GraphicsPipeline> m_GraphicsPipeline;
			MaterialDataContainer m_Container;
		};
	};

}  // namespace At0::Ray
