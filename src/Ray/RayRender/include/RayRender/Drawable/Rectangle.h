#pragma once

#include "Drawable.h"


namespace At0::Ray
{
	class RR_API Rectangle : public Drawable
	{
	public:
		Rectangle(const Renderer3D& renderer, float size, const float color[3]);
		virtual void Update() override;

		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const override { return s_StaticBinds; }

	private:
		void AddStaticBind(Scope<Bindable> bind);
		void AddStaticIndexBuffer(Scope<IndexBuffer> bind);
		void SetIndexFromStatic();

	private:
		static std::vector<Scope<Bindable>> s_StaticBinds;
	};
}