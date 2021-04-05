#pragma once

#include "RBase.h"
#include "RWidget.h"

#include "../Scene/REntity.h"


namespace At0::Ray
{
	class RAY_EXPORT Button : public Widget
	{
	public:
		Button(Entity& entity);

	private:
		Entity m_PlaneEntity;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Button);
