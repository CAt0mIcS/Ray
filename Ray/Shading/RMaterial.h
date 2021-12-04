#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include "RMaterialDataContainer.h"
#include "../Graphics/Pipelines/Shader/RShaderReflection.h"

#include <unordered_map>
#include <vector>
#include <functional>


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
		void* GetRaw(const std::string& dataPath) const { return m_Container.Get(dataPath); }
		ShaderDataType GetType(const std::string& dataPath) const
		{
			return m_Container.GetType(dataPath);
		}

		template<typename T>
		void Set(const std::string& name, T&& data)
		{
			Builder::ValidateUniformExistence(*m_GraphicsPipeline, name);
			Builder::Set(name, std::move(data), m_Container);
			CallListeners(name, UniformType::UniformBuffer);
		}

		void Set(const std::string& name, Ref<Texture2D> texture);

		template<typename T>
		T& Get(const std::string& name)
		{
			return const_cast<T&>(std::as_const(*this).Get<T>(name));
		}

		template<typename T>
		const T& Get(const std::string& name) const
		{
			return *(T*)m_Container.Get(name);
		}

		uint32_t AddOnDirtyListener(std::function<void(const std::string&, UniformType)> fun);
		void RemoveOnDirtyListener(uint32_t index);

	private:
		void CallListeners(const std::string& name, UniformType type) const;

	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline;
		MaterialDataContainer m_Container;

		// RAY_TODO: Test performance overhead
		std::vector<std::function<void(const std::string&, UniformType)>> m_OnDirtyListeners;

	public:
		class RAY_EXPORT Builder
		{
			friend class Material;

		public:
			Builder(Ref<GraphicsPipeline> pipeline);

			template<typename T>
			Builder& Set(const std::string& name, T&& data)
			{
				ValidateUniformExistence(*m_GraphicsPipeline, name);
				Set(name, std::move(data), m_Container);
				return *this;
			}

			template<typename T>
			static void Set(const std::string& name, T&& data, MaterialDataContainer& container)
			{
				container.Set(name, std::move(data));
			}

			Builder& Set(const std::string& name, Ref<Texture2D> data);
			static void Set(
				const std::string& name, Ref<Texture2D> data, MaterialDataContainer& container);

			Ref<Material> Acquire();

		private:
			static void ValidateUniformExistence(
				const GraphicsPipeline& pipeline, const std::string& name);

		private:
			Ref<GraphicsPipeline> m_GraphicsPipeline;
			MaterialDataContainer m_Container;
		};
	};

}  // namespace At0::Ray
