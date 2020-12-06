#pragma once

#include "RlRender/RlRBase.h"
#include "Drawable.h"


namespace At0::Reyal
{
	class RLR_API Cube : public Drawable
	{
	public:
		Cube(const Renderer3D& renderer, float size, const float colors[6][3]);
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