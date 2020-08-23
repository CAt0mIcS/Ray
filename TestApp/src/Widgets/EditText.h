#pragma once

#include <QtWidgets/QPlainTextEdit>


class EditText : public QPlainTextEdit
{
	Q_OBJECT;
public:
	EditText(QWidget* parent = nullptr);
};
