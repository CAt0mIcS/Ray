#pragma once

#include "../RRBase.h"
#include "../Core/REntity.h"

#include <string>


namespace At0::Ray
{
	class Scene;

	class VertexShader;
	class PixelShader;
	class InputLayout;
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

		virtual Ref<VertexBuffer>	GetVertexBuffer() = 0;
		virtual Ref<IndexBuffer>	GetIndexBuffer() = 0;
		virtual Ref<VertexShader>	GetVertexShader() = 0;
		virtual Ref<PixelShader>	GetPixelShader() = 0;
		virtual Ref<InputLayout>	GetInputLayout() = 0;
		virtual Ref<Topology>		GetTopology() = 0;

	private:
		Entity m_Entity;
	};

	// Static bindables. Generate templated class for every type of drawable (DrawableBase<Quad> && DrawableBase<Triangle>)
	// RAY_TODO: Virtual function call, possible performance overhead
	template<typename T>
	class RR_API DrawableBase : public Drawable
	{
	public:
		virtual Ref<VertexBuffer>	GetVertexBuffer() override { return s_VertexBuffer; }
		virtual Ref<IndexBuffer>	GetIndexBuffer() override { return s_IndexBuffer; }
		virtual Ref<VertexShader>	GetVertexShader() override { return s_VertexShader; }
		virtual Ref<PixelShader>	GetPixelShader() override { return s_PixelShader; }
		virtual Ref<InputLayout>	GetInputLayout() override { return s_InputLayout; }
		virtual Ref<Topology>		GetTopology() override { return s_Topology; }

		virtual ~DrawableBase() = default;

	protected:
		DrawableBase(Entity e)
			: Drawable(e)
		{
		}

		DrawableBase() = default;

	protected:
		inline static Ref<VertexBuffer> s_VertexBuffer = nullptr;
		inline static Ref<IndexBuffer> s_IndexBuffer = nullptr;
		inline static Ref<VertexShader> s_VertexShader = nullptr;
		inline static Ref<PixelShader> s_PixelShader = nullptr;
		inline static Ref<InputLayout> s_InputLayout = nullptr;
		inline static Ref<Topology> s_Topology = nullptr;
	};
}