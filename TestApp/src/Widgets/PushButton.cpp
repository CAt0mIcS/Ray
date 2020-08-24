#include "PushButton.h"

#include <QMouseEvent>

#include <QPainter>


PushButton::PushButton(QWidget* parent)
	: QPushButton(parent), m_DrawLine(false)
{
}

void PushButton::mousePressEvent(QMouseEvent* e)
{
	m_ConStartPos = e->pos();
	m_DrawLine = true;
	QPushButton::mousePressEvent(e);
}

void PushButton::mouseMoveEvent(QMouseEvent* e)
{
	//Draw line from startPos to cursor
	if (m_DrawLine)
	{
		m_ConNextPos = e->pos();
		this->update();
	}
	QPushButton::mouseMoveEvent(e);
}

void PushButton::mouseReleaseEvent(QMouseEvent* e)
{
	//m_DrawLine = false;
}

void PushButton::paintEvent(QPaintEvent* e)
{
	//Called from mouseMoveEvent
	if (m_DrawLine)
	{
		QPainter painter(this);
		painter.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
		painter.drawLine(0, 0, 200, 200);
		m_DrawLine = false;
	}
	QPushButton::paintEvent(e);
}
