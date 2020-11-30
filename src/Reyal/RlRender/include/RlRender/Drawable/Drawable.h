#pragma once

#include "RlRender/RlRBase.h"
#include "RlRender/Bindable/Bindable.h"

#include <DirectXMath.h>
#include <vector>

namespace At0::Reyal
{
	class IndexBuffer;
	class Renderer3D;

	class RLR_API Drawable
	{
	public:
		Drawable(const Drawable&) = delete;
		Drawable(Drawable&&) noexcept = default;
		Drawable& operator=(Drawable&&) noexcept = default;

		virtual void Update() = 0;
		void SetTransform(const DirectX::XMMATRIX& transform) { m_TransformMatrix = transform; }
		const DirectX::XMMATRIX& GetTransform() const { return m_TransformMatrix; }
		void Draw(Renderer3D* renderer);

	protected:
		void AddBind(Scope<Bindable> bind);
		void AddIndexBuffer(Scope<IndexBuffer> indexBuffer);
		Drawable();

	protected:
		DirectX::XMMATRIX m_TransformMatrix;

	private:
		const IndexBuffer* m_pIndexBuffer;
		std::vector<Scope<Bindable>> m_Binds;
	};
}