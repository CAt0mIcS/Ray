#pragma once

#include "RComponent.h"
#include "../Core/RMath.h"

#include "../Events/REventListener.h"
#include "RTransform.h"


namespace At0::Ray
{
	class RAY_EXPORT PointLight : public Component, EventListener<TransformChangedEvent>
	{
	public:
		PointLight(Entity entity, Float4 color = { 1.f, 1.f, 1.f, 1.f });

		const Float4& GetColor() const { return m_Color; }
		void SetColor(Float4 color);

		uint32_t GetID() const { return m_ID; }

	private:
		void SetTranslation(Float3 trans);
		void OnEvent(TransformChangedEvent& e);

	private:
		Float4 m_Color;
		uint32_t m_ID;

		static uint32_t s_ID;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(PointLight);
