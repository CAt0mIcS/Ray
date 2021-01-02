#pragma once

#include "../RlRBase.h"
#include "RayRender/Bindable/Bindable.h"

#include <../../RayDebug/include/RayDebug/RAssert.h>
#include <../../RayUtil/include/RayUtil/RMath.h>

#include <vector>
#include <../../extern/entt/src/entt/entt.hpp>


namespace At0::Ray
{
	struct TranslationComponent
	{
		float x, y, z;

		TranslationComponent(float x, float y, float z)
			: x(x), y(y), z(z) {}

		TranslationComponent()
			: x(0.0f), y(0.0f), z(0.0f) {}
	};

	struct ScaleComponent
	{
		float x, y, z;

		ScaleComponent(float x, float y, float z)
			: x(x), y(y), z(z) {}

		ScaleComponent()
			: x(1.0f), y(1.0f), z(1.0f) {}
	};

	struct RotationComponent
	{
		float x, y, z, w;

		RotationComponent()
			: x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

		RotationComponent(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w) {}
	};

	struct TransformComponent
	{
		TranslationComponent Translation;
		ScaleComponent Scale;
		RotationComponent Rotation;

		TransformComponent()
			: Translation{}, Scale{}, Rotation{} {}

		TransformComponent(
			const TranslationComponent& transform,
			const ScaleComponent& scale,
			const RotationComponent& rotation
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
		Matrix GetTransform() const;

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