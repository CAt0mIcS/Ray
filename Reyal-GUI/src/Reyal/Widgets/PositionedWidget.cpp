#include "rlpch.h"
#include "PositionedWidget.h"

#include "Reyal/Events/ApplicationEvent.h"
#include "Reyal/Events/KeyboardEvent.h"
#include "Reyal/Events/MouseEvent.h"


namespace Zeal::Reyal
{
	PositionedWidget::PositionedWidget(const std::wstring_view name, Widget* parent)
		: Widget(name, parent)
	{

	}
}
