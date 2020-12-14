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
		DirectX::XMMATRIX GetTransform() const;
		void SetRotation(float pitch, float yaw, float roll) { m_Pitch = pitch; m_Yaw = yaw; m_Roll = roll; }
		void SetTranslation(float x, float y, float z) { m_Translation = { x, y, z }; }
		void SetScale(float x, float y, float z) { m_Scale = { x, y, z }; }
		virtual void Draw(Renderer3D* renderer);
		void Bind();

	protected:
		void AddBind(Scope<Bindable> bind);
		void AddIndexBuffer(Scope<IndexBuffer> indexBuffer);
		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const = 0;
		Drawable();

	protected:
		DirectX::XMFLOAT3 m_Translation{};
		DirectX::XMFLOAT3 m_Scale{};
		
		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
		float m_Roll = 0.0f;

		const IndexBuffer* m_pIndexBuffer;

	private:
		std::vector<Scope<Bindable>> m_Binds;
	};
}