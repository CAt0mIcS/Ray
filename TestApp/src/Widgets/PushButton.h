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

private:
};
