#pragma once

#include "../RlRBase.h"
#include "Drawable.h"


namespace At0::Ray
{
	class RR_API ShadedCube : public Drawable
	{
	public:
		ShadedCube(Renderer3D& renderer);
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
