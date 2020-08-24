#pragma once


#include <QtWidgets/QLineEdit>

#include "Interfaces/Widget.h"


class TextBox : public QLineEdit, public Widget
{
	Q_OBJECT;
public:
	TextBox(QWidget* parent = nullptr);

private:

private:
	QAction* m_ActionLocked;

};
