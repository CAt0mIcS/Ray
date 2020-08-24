#include "PushButton.h"
#include "TestApp.h"

#include <QMouseEvent>

#include <QPainter>


PushButton::PushButton(QWidget* parent)
	: QPushButton(parent), m_DrawLine(false)
{
	setMouseTracking(true);
}

void PushButton::mousePressEvent(QMouseEvent* e)
{
	m_ConStartPos = e->globalPos();
	m_DrawLine = true;
	QPushButton::mousePressEvent(e);
}

void PushButton::mouseMoveEvent(QMouseEvent* e)
{
	if (m_DrawLine)
	{
		Node* parentNode = (Node*)parentWidget();
		TestApp* parent = (TestApp*)parentNode->parentWidget();
		if (parent)
		{
			m_ConNextPos = e->globalPos();
			parent->invokeLinePaint(m_ConStartPos.x(), m_ConStartPos.y(), m_ConNextPos.x(), m_ConNextPos.y());
		}
	}
}

void PushButton::mouseReleaseEvent(QMouseEvent* e)
{
	QPushButton::mouseReleaseEvent(e);
}

void PushButton::paintEvent(QPaintEvent* e)
{
	QPushButton::paintEvent(e);
}
