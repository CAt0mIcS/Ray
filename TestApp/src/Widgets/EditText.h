#pragma once

#include <QtWidgets/QPlainTextEdit>

#include "Interfaces/Widget.h"


class EditText : public QPlainTextEdit, public Widget
{
	Q_OBJECT;
public:
	EditText(QWidget* parent = nullptr);

private:
	QAction* m_ActionLocked;
};
