#pragma once

#include "RBase.h"
#include "RWidget.h"

#include "../Scene/REntity.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT Button : public Widget
	{
	public:
		Button(Entity entity, std::string_view name, Float2 pos, float width, float height);

		void SetColor(const Float3& color);
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Button);
