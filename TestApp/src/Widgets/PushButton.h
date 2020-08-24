#pragma once

#include <QtWidgets/QPushButton>
#include <QMenu>

#include "Interfaces/Widget.h"


class PushButton : public QPushButton
{
	Q_OBJECT;

public:
	PushButton(QWidget* parent = nullptr);

private:
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void paintEvent(QPaintEvent* e) override;

private:
	QPoint m_ConStartPos;
	QPoint m_ConNextPos;
	bool m_DrawLine;
};
