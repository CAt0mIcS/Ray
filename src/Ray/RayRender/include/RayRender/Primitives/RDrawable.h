#pragma once

#include "../RRBase.h"
#include "../Core/REntity.h"
#include "../RConstantBuffers.h"

#ifdef __GNUC__
#include "../RVertexShader.h"
#include "../RPixelShader.h"
#include "../RInputLayout.h"
#include "../RIndexBuffer.h"
#include "../RVertexBuffer.h"
#include "../RTopology.h"
#endif

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

		virtual VertexBuffer* GetVertexBuffer() = 0;
		virtual IndexBuffer* GetIndexBuffer() = 0;
		virtual VertexShader* GetVertexShader() = 0;
		virtual PixelShader* GetPixelShader() = 0;
		virtual InputLayout* GetInputLayout() = 0;
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
		virtual VertexBuffer* GetVertexBuffer() override { return s_VertexBuffer.get(); }
		virtual IndexBuffer* GetIndexBuffer() override { return s_IndexBuffer.get(); }
		virtual VertexShader* GetVertexShader() override { return s_VertexShader.get(); }
		virtual PixelShader* GetPixelShader() override { return s_PixelShader.get(); }
		virtual InputLayout* GetInputLayout() override { return s_InputLayout.get(); }
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
		inline static Scope<VertexShader> s_VertexShader = nullptr;
		inline static Scope<PixelShader> s_PixelShader = nullptr;
		inline static Scope<InputLayout> s_InputLayout = nullptr;
		inline static Scope<Topology> s_Topology = nullptr;
	};
}
