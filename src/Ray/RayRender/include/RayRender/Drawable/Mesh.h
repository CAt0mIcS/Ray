#pragma once

#include "../RlRBase.h"

#include "../Bindable/Bindable.h"
#include "../Drawable/Drawable.h"

#include "../Bindable/VertexBuffer.h"
#include "../Bindable/IndexBuffer.h"


struct aiScene;
struct aiMesh;
struct aiNode;


namespace At0::Ray
{
	class RR_API Mesh : public Drawable
	{
	public:
		template<typename V>
		Mesh(const std::vector<V>& vertices, const std::vector<uint16_t> indices)
		{
			if (s_StaticBinds.empty())
			{
				AddStaticBind(MakeScope<VertexBuffer>(vertices));
				AddStaticIndexBuffer(MakeScope<IndexBuffer>(indices));
			}
			else
			{
				SetIndexFromStatic();
			}
		}

		void AddStaticBind(Scope<Bindable> bind);
		void AddStaticIndexBuffer(Scope<IndexBuffer> bind);
		void SetIndexFromStatic();

		virtual const std::vector<Scope<Bindable>>& GetStaticBinds() const override { return s_StaticBinds; }
		virtual void Update() override;
	private:
		static std::vector<Scope<Bindable>> s_StaticBinds;
	};
}