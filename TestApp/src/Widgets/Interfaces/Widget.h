#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QAction>


class Widget
{
public:
	Widget(QWidget* parent = nullptr) : m_PosLocked(false) {}

	void OnLockPosition(QAction* action)
	{
		m_PosLocked = !m_PosLocked;
		if (m_PosLocked)
			action->setText("Unlock position");
		else
			action->setText("Lock position");
	}

	virtual bool IsPositionLocked() const { return m_PosLocked; }
protected:
	bool m_PosLocked;

private:
};
