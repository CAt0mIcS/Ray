#include "Rpch.h"
#include "RWidget.h"

#include "Scene/REntity.h"
#include "Geometry/RMesh.h"


namespace At0::Ray
{
	Float2 Widget::GetTranslation() const
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		return NDCSpaceToScreenSpace(tform.Translation());
	}

	Float2 Widget::GetScale() const
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		return NDCSpaceToScreenSpace(tform.Scale());
	}

	float Widget::GetWidth() const
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		return NDCSpaceToScreenSpace(tform.Translation() + tform.Scale()).x;
	}

	float Widget::GetHeight() const
	{
		Transform& tform = GetEntity().Get<Mesh>().GetTransform();
		return NDCSpaceToScreenSpace(tform.Translation() + tform.Scale()).y;
	}

	Widget::Widget(Entity entity) : Component(entity) {}
}  // namespace At0::Ray
