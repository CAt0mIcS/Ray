#pragma once

#include "../RlRBase.h"
#include "RayRender/Bindable/Bindable.h"

#include <../../RayDebug/include/RayDebug/RAssert.h>
#include <../../RayUtil/include/RayUtil/RMath.h>

#include <vector>
#include <../../extern/entt/src/entt/entt.hpp>

#include <DirectXMath.h>


namespace At0::Ray
{
	struct TranslationComponent
	{
		Float3 Translation;

		TranslationComponent(float x, float y, float z)
			: Translation{ x, y, z } {}

		TranslationComponent()
			: Translation{ 0.0f, 0.0f, 0.0f } {}
	};

	struct ScaleComponent
	{
		Float3 Scale;

		ScaleComponent(float x, float y, float z)
			: Scale{ x, y, z } {}

		ScaleComponent()
			: Scale{ 1.0f, 1.0f, 1.0f } {}
	};

	struct RotationComponent
	{
		Float4 Rotation;

		RotationComponent()
			: Rotation{ 0.0f, 0.0f, 0.0f, 1.0f } {}

		RotationComponent(float x, float y, float z, float w)
			: Rotation{ x, y, z, w } {}
	};

	struct TransformComponent
	{
		Float3 Translation;
		Float3 Scale;
		Float4 Rotation;

		TransformComponent()
			: Translation{ 0.0f, 0.0f, 0.0f }, Scale{ 1.0f, 1.0f, 1.0f }, Rotation{ 0.0f, 0.0f, 0.0f, 1.0f } {}

		TransformComponent(
			const Float3& transform,
			const Float3& scale,
			const Float4& rotation
		)
			: Translation(transform), Scale(scale), Rotation(rotation) {}
	};



	class IndexBuffer;
	class Renderer3D;

	class RR_API Drawable
	{
	public:
		virtual ~Drawable();
		Drawable(const Drawable&) = delete;
		Drawable(Drawable&&) noexcept = default;
		Drawable& operator=(Drawable&&) noexcept = default;

		virtual void Update() = 0;
		DirectX::XMMATRIX GetTransform() const;

		virtual void Draw(Renderer3D* renderer);
		void Bind();

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			s_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
		}

		template<typename... T>
		void RemoveComponent()
		{
			s_Registry.remove<T...>(m_Entity);
		}

		template<typename... T>
		decltype(auto) GetComponent() const
		{
			RAY_MEXPECTS(s_Registry.has<T...>(m_Entity), "[Drawable::GetComponent] Entity (ID={0}) doesn't have component.", entt::to_integral(m_Entity));
			return s_Registry.get<T...>(m_Entity);
		}

	protected:
		void AddBind(Scope<Bindable> bind);
		void AddIndexBuffer(Scope<IndexBuffer> indexBuffer);
		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const = 0;
		Drawable();

	protected:
		const IndexBuffer* m_pIndexBuffer;

		entt::entity m_Entity;
	private:
		std::vector<Scope<Bindable>> m_Binds;
		static entt::registry s_Registry;
	};
}