#pragma once

#include "../../RBase.h"
#include "../../Graphics/Core/RBindable.h"
#include "../../Graphics/Pipelines/Uniforms/RBufferUniform.h"
#include "../../Graphics/Pipelines/Uniforms/RSamplerUniform.h"
#include "../../Graphics/Images/RTexture2D.h"

#include "../../Utils/RException.h"
#include "../../Utils/RAssert.h"

#include <type_traits>
#include <vector>
#include <optional>


namespace At0::Ray
{
	class GraphicsPipeline;
	class VertexLayout;

	template<typename T1, typename... T2>
	concept DisableCopy = (std::negation_v<std::is_same<T1, T2>> && ...);

	class RAY_EXPORT Material : public Bindable
	{

	public:
		struct Shaders
		{
			template<typename... Args>
			Shaders(Args&&... args) requires(std::is_convertible_v<Args, std::string>&&...)
				: value(args...)
			{
			}
			Shaders(std::vector<std::string> val) : value(std::move(val)) {}
			operator const std::vector<std::string>&() const { return value; }

			std::vector<std::string> value;
		};

		struct CullMode
		{
			CullMode(VkCullModeFlags val) : value(val) {}
			operator VkCullModeFlags() const { return value; }

			VkCullModeFlags value;
		};

		struct PolygonMode
		{
			PolygonMode(VkPolygonMode val) : value(val) {}
			operator VkPolygonMode() const { return value; }

			VkPolygonMode value;
		};

		struct LineWidth
		{
			LineWidth(float val) : value(val) {}
			operator float() const { return value; }

			float value;
		};

		struct Color
		{
			Color(Float3 val) : value(std::move(val)) {}
			operator const Float3&() const { return value; }

			Float3 value;
		};

		struct Texture2D
		{
			Texture2D(Ref<Ray::Texture2D> val) : value(std::move(val)) {}
			operator const Ref<Ray::Texture2D>&() const { return value; }

			Ref<Ray::Texture2D> value;
		};

		struct SpecularMap
		{
			SpecularMap(Ref<Ray::Texture2D> val) : value(std::move(val)) {}
			operator const Ref<Ray::Texture2D>&() const { return value; }

			Ref<Ray::Texture2D> value;
		};

		struct DiffuseMap
		{
			DiffuseMap(Ref<Ray::Texture2D> val) : value(std::move(val)) {}
			operator const Ref<Ray::Texture2D>&() const { return value; }

			Ref<Ray::Texture2D> value;
		};

		struct NormalMap
		{
			NormalMap(Ref<Ray::Texture2D> val) : value(std::move(val)) {}
			operator const Ref<Ray::Texture2D>&() const { return value; }

			Ref<Ray::Texture2D> value;
		};

	protected:
		struct Config
		{
			Shaders shaders = {};
			CullMode cullMode = VK_CULL_MODE_BACK_BIT;
			PolygonMode polygonMode = VK_POLYGON_MODE_FILL;
			LineWidth lineWidth = 1.0f;
			Color color = { { 1.0f, 1.0f, 1.0f } };
			Texture2D texture2D = { nullptr };
			SpecularMap specularMap = { nullptr };
			DiffuseMap diffuseMap = { nullptr };
			NormalMap normalMap = { nullptr };
		};

	public:
		template<typename... Args>
		Material(Args&&... args) requires(
			DisableCopy<Args, const Material&, Material&, Material&&>&&...);
		virtual ~Material();

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		const VertexLayout& GetVertexLayout() const;

		void CmdBind(const CommandBuffer& cmdBuff) const override;

		/**
		 * Adds a uniform to the list of uniforms which are automatically bound in CmdBind
		 * @param tag Unique tag to identify the uniform
		 * @param uniform Uniform to add
		 */
		Uniform& AddUniform(std::string_view tag, Scope<Uniform> uniform);

		/**
		 * @returns If the uniform with tag has been added
		 */
		bool HasUniform(std::string_view tag) const;

		/**
		 * @param tag The tag used when adding the uniform with "AddUniform"
		 * @returns The uniform which was added using "AddUniform"
		 */
		template<typename U>
		U& GetUniform(std::string_view tag);

		Material& operator=(Material&& other) noexcept;
		Material(Material&& other) noexcept;

		Material& operator=(const Material& other);
		Material(const Material& other);

	private:
		void CreatePipeline(Material::Config& config);
		void Setup(Material::Config& config);

		static void FillConfig(Config& config, Shaders shaders) { config.shaders = shaders; }
		static void FillConfig(Config& config, CullMode cullMode) { config.cullMode = cullMode; }
		static void FillConfig(Config& config, PolygonMode polygonMode);
		static void FillConfig(Config& config, LineWidth lineWidth);
		static void FillConfig(Config& config, Color color) { config.color = color; }
		static void FillConfig(Config& config, Texture2D tex) { config.texture2D = tex; }
		static void FillConfig(Config& config, SpecularMap spec) { config.specularMap = spec; }
		static void FillConfig(Config& config, DiffuseMap diff) { config.diffuseMap = diff; }
		static void FillConfig(Config& config, NormalMap norm) { config.normalMap = norm; }

		static std::vector<std::string> GetShaders(Material::Config& config);

	private:
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;
		std::vector<Scope<Uniform>> m_Uniforms;
	};


	template<typename... Args>
	inline Material::Material(Args&&... args) requires(
		DisableCopy<Args, const Material&, Material&, Material&&>&&...)
	{
		Material::Config config{};
		(FillConfig(config, args), ...);

		CreatePipeline(config);
		Setup(config);
	}


	template<typename U>
	U& Material::GetUniform(std::string_view tag)
	{
		for (auto& uniform : m_Uniforms)
			if (tag == uniform->GetName())
			{
				RAY_MEXPECTS(dynamic_cast<U*>(uniform.get()),
					"[Material] Type \"{0}\" is not compatible with uniform \"{1}\"",
					typeid(U).name(), tag);
				return *(U*)uniform.get();
			}

		RAY_THROW_RUNTIME("[Material] Failed to get uniform with tag {0}", tag);
	}

}  // namespace At0::Ray
