#pragma once

#include "RComponent.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT SpotLight : Component
	{
	public:
		SpotLight(Entity entity, Float4 color = { 1.f, 1.f, 1.f, 1.f });

		const Float4& GetColor() const { return m_Color; }
		void SetColor(Float4 color);

		void SetTranslation(Float3 trans);

		void SetDirection(Float3 dir);
		Float3 GetDirection() const { return m_Direction; }

		uint32_t GetID() const { return m_ID; }

	private:
		Float4 m_Color;
		uint32_t m_ID;

		Float3 m_Direction{ 0.f, 1.f, 0.f };

		static uint32_t s_ID;
	};

}  // namespace At0::Ray

RAY_EXPORT_COMPONENT(SpotLight)
