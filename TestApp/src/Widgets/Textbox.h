#pragma once


#include <QtWidgets/QLineEdit>


class TextBox : public QLineEdit
{
	Q_OBJECT;
public:
	TextBox(QWidget* parent = nullptr);

	void leaveEvent(QEvent* e) override;
	void enterEvent(QEvent* e) override;
};
