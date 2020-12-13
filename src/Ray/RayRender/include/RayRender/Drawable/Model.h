#pragma once

#include "../RlRBase.h"
#include "Drawable.h"
#include "Mesh.h"


namespace At0::Ray
{
	class RR_API Model : public Drawable
	{
	public:
		Model(std::string_view filepath);
		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const override { return s_StaticBinds; }

	private:
		void AddStaticBind(Scope<Bindable> bind);
		void AddStaticIndexBuffer(Scope<IndexBuffer> bind);
		void SetIndexFromStatic();

	private:
		Mesh m_Mesh;
		static std::vector<Scope<Bindable>> s_StaticBinds;
	};
}