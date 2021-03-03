#pragma once

#include "../../RBase.h"
#include "../../Scene/REntity.h"
#include "../../Graphics/Images/RImage2D.h"
#include "../../Core/RColor.h"


namespace At0::Ray
{
	class GraphicsPipeline;
	class VertexLayout;

	class Material
	{
	public:
		enum MaterialEx
		{
			DiffuseBase,
			DiffuseImage,
			Metallic,
			Roughness,
			MaterialImage,
			NormalImage
		};

	private:
		template<MaterialEx Extend, typename FAKE = void>
		struct Map;

		template<typename FAKE>
		struct Map<DiffuseBase, FAKE>
		{
			using SysType = Color;
		};
		template<typename FAKE>
		struct Map<DiffuseImage, FAKE>
		{
			using SysType = Image2D;
		};
		template<typename FAKE>
		struct Map<Metallic, FAKE>
		{
			using SysType = float;
		};
		template<typename FAKE>
		struct Map<Roughness, FAKE>
		{
			using SysType = float;
		};
		template<typename FAKE>
		struct Map<MaterialImage, FAKE>
		{
			using SysType = Image2D;
		};
		template<typename FAKE>
		struct Map<NormalImage, FAKE>
		{
			using SysType = Image2D;
		};

	public:
		Material() {}
		~Material() {}

		template<MaterialEx Extend>
		decltype(auto) Emplace(typename const Map<Extend>::SysType& arg)
		{
			m_Entity.Emplace<Map<Extend>::SysType>(arg);
		}

		const GraphicsPipeline& GetGraphicsPipeline() const { return *m_GraphicsPipeline; }
		const VertexLayout& GetVertexLayout() const;

		/**
		 * Creates the pipeline once all the required Material Extends are added.
		 */
		void Create();

		/**
		 * Gets if Material::Create has already been called
		 */
		bool WasCreated() const { return m_GraphicsPipeline != nullptr; }

	private:
		Entity m_Entity;
		Ref<GraphicsPipeline> m_GraphicsPipeline = nullptr;
	};


}  // namespace At0::Ray
