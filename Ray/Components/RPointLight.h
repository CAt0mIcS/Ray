﻿#pragma once

#include "RComponent.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT PointLight : public Component
	{
	public:
		PointLight(Entity entity, Float4 color = { 1.f, 1.f, 1.f, 1.f });

		const Float4& GetColor() const { return m_Color; }
		void SetColor(Float4 color);

		void SetTranslation(Float3 trans);

		uint32_t GetID() const { return m_ID; }

	private:
		Float4 m_Color;
		uint32_t m_ID;

		static uint32_t s_ID;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(PointLight);