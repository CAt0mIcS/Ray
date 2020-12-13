#pragma once

#include "../RlRBase.h"
#include "RayRender/Bindable/Bindable.h"

#include <DirectXMath.h>
#include <vector>

namespace At0::Ray
{
	class IndexBuffer;
	class Renderer3D;

	class RR_API Drawable
	{
	public:
		virtual ~Drawable() = default;
		Drawable(const Drawable&) = delete;
		Drawable(Drawable&&) noexcept = default;
		Drawable& operator=(Drawable&&) noexcept = default;

		virtual void Update() = 0;
		void SetTransform(const DirectX::XMMATRIX& transform) { m_TransformMatrix = transform; }
		const DirectX::XMMATRIX& GetTransform() const { return m_TransformMatrix; }
		virtual void Draw(Renderer3D* renderer);
		void Bind();

	protected:
		void AddBind(Scope<Bindable> bind);
		void AddIndexBuffer(Scope<IndexBuffer> indexBuffer);
		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const = 0;
		Drawable();

	protected:
		DirectX::XMMATRIX m_TransformMatrix;
		const IndexBuffer* m_pIndexBuffer;

	private:
		std::vector<Scope<Bindable>> m_Binds;
	};
}