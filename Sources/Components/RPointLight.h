#pragma once

#include "../RBase.h"

#include "RComponent.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT PointLight : public Component
	{
	public:
		PointLight(Entity entity, Float3 color = Float3{ 1.0f }, float power = 10.0f);

		const Float3& GetTranslation() const;
		void SetTranslation(Float3 translation);

	private:
	};
}  // namespace At0::Ray
