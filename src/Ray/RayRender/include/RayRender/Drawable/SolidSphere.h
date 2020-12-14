#pragma once

#include "Drawable.h"


namespace At0::Ray
{
	class SolidSphere : public Drawable
	{
	public:
		SolidSphere(Renderer3D& renderer, float radius);

		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const { return s_StaticBinds; }
		virtual void Update() override {}

	private:
		void AddStaticBind(Scope<Bindable> bind);
		void AddStaticIndexBuffer(Scope<IndexBuffer> bind);
		void SetIndexFromStatic();

	private:
		static std::vector<Scope<Bindable>> s_StaticBinds;
	};
}