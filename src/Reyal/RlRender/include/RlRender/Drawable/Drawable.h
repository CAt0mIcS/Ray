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
		void Draw(Renderer3D* renderer);
		virtual DirectX::XMMATRIX GetTransformXM() const = 0;

	protected:
		void AddBind(Scope<Bindable> bind);
		void AddIndexBuffer(Scope<IndexBuffer> indexBuffer);

	protected:
		Drawable();

	private:
		const IndexBuffer* m_pIndexBuffer;
		std::vector<Scope<Bindable>> m_Binds;
	};
}