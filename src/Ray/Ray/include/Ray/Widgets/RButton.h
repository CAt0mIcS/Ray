#pragma once

#include "../RBase.h"
#include "RWidget.h"

#include "../../RayEvent/include/RayEvent/RayEvent.h"


namespace At0::Ray
{
	class RAY_API Button : public Widget,
		EventListener<PaintEvent>
	{
	public:
		Button(const std::string_view name, Widget& parent);
		void AddPaintEventListener(EventDispatcher<PaintEvent>& dispatcher);

		virtual void SetPos(const Point2& pos) override;
		virtual void SetSize(const Size2& size) override;

		~Button();

	private:
		virtual void OnEvent(Widget* receiver, PaintEvent& e) override;

	private:

	};
}