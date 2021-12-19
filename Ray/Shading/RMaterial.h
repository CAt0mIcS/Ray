#pragma once

#include "../RBase.h"
#include "RMaterialDataContainer.h"
#include "../Events/REventDispatcher.h"
#include "../Events/RCustomEvents.h"
#include "../Utils/RTypeTraits.h"

#include <string>
#include <type_traits>


namespace At0::Ray
{
	class GraphicsPipeline;
	class Texture2D;
	class Texture2DAtlas;
	class CommandBuffer;

	class RAY_EXPORT Material : public EventDispatcher<MaterialBecameDirtyEvent>
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
			Builder::ValidateUniformExistenceAndSize(
				*m_GraphicsPipeline, name, Builder::GetTypeSize<T>());
			m_Container.Set(name, std::forward<T>(data));
			CallListeners(name, GetUniformType(name));
		}

		void Set(const std::string& name, Ref<Texture2D> texture);
		void Set(const std::string& name, Ref<Texture2DAtlas> texture);

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

	private:
		void CallListeners(const std::string& name, UniformType type);
		UniformType GetUniformType(std::string_view name) const;

	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline;
		MaterialDataContainer m_Container;

	public:
		class RAY_EXPORT Builder
		{
			friend class Material;

		public:
			Builder(Ref<GraphicsPipeline> pipeline);

			template<typename T>
			Builder& Set(const std::string& name, T&& data)
			{
				Builder::ValidateUniformExistenceAndSize(
					*m_GraphicsPipeline, name, GetTypeSize<T>());
				m_Container.Set(name, std::forward<T>(data));
				return *this;
			}

			Builder& Set(const std::string& name, Ref<Texture2D> data);
			Builder& Set(const std::string& name, Ref<Texture2DAtlas> data);

			Ref<Material> Acquire();
			Ref<Material> Build();

		private:
			static void ValidateUniformExistenceAndSize(
				const GraphicsPipeline& pipeline, const std::string& name, uint32_t size);

			template<typename T>
			static uint32_t GetTypeSize()
			{
				// Bools in vulkan are 4 bytes each
				if constexpr (IsSameType<T, bool>)
					return sizeof(VkBool32);
				else if constexpr (IsSameType<T, Bool2>)
					return sizeof(UIBool2);
				else if constexpr (IsSameType<T, Bool3>)
					return sizeof(UIBool3);
				else if constexpr (IsSameType<T, Bool4>)
					return sizeof(UIBool4);
				return sizeof(T);
			}

		private:
			Ref<GraphicsPipeline> m_GraphicsPipeline;
			MaterialDataContainer m_Container;
		};
	};

}  // namespace At0::Ray
