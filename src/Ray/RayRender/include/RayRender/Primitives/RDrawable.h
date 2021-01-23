#pragma once

#include "../RRBase.h"
#include "../Core/REntity.h"
#include "../RConstantBuffers.h"

#ifdef __GNUC__
#include "../RIndexBuffer.h"
#include "../RVertexBuffer.h"
#include "../RTopology.h"
#include "../RShader.h"
#endif

#include <string>


namespace At0::Ray
{
	class Scene;

	class Shader;
	class IndexBuffer;
	class VertexBuffer;
	class Topology;

	class RR_API Drawable
	{
	public:
		Drawable(Entity e);
		Drawable() = default;

		template<typename Component, typename... Args>
		decltype(auto) AddComponent(Args&&... args)
		{
			m_Entity.Emplace<Component>(std::forward<Args>(args)...);
		}

		template<typename... Component>
		decltype(auto) GetComponent()
		{
			return m_Entity.Get<Component...>();
		}

		template<typename... Component>
		void RemoveComponent()
		{
			m_Entity.Remove<Component...>();
		}

		template<typename... Component>
		bool HasComponent() const
		{
			return m_Entity.Has<Component...>();
		}

		virtual ~Drawable() = default;

		virtual VertexBuffer* GetVertexBuffer() = 0;
		virtual IndexBuffer* GetIndexBuffer() = 0;
		virtual Shader* GetShader() = 0;
		virtual Topology* GetTopology() = 0;
		virtual ConstantBuffer* GetVertexConstantBuffer() = 0;

	private:
		Entity m_Entity;
	};

	// Static bindables. Generate templated class for every type of drawable (DrawableBase<Quad> && DrawableBase<Triangle>)
	// RAY_TODO: Virtual function calls, possible performance overhead
	template<typename T>
	class RR_API DrawableBase : public Drawable
	{
	public:
		virtual VertexBuffer* GetVertexBuffer() override { return s_VertexBuffer.get(); };
		virtual IndexBuffer* GetIndexBuffer() override { return s_IndexBuffer.get(); };
		virtual Shader* GetShader() override { return s_Shader.get(); }
		virtual Topology* GetTopology() override { return s_Topology.get(); }

		virtual ~DrawableBase() = default;

	protected:
		DrawableBase(Entity e)
			: Drawable(e)
		{
		}

		DrawableBase() = default;

	protected:
		inline static Scope<VertexBuffer> s_VertexBuffer = nullptr;
		inline static Scope<IndexBuffer> s_IndexBuffer = nullptr;
		inline static Scope<Shader> s_Shader = nullptr;
		inline static Scope<Topology> s_Topology = nullptr;
	};
}
