#pragma once

#include <QtWidgets/QWidget>


class Node : public QWidget
{
	Q_OBJECT;
public:
	Node(QWidget* parent = nullptr);

	void paintEvent(QPaintEvent* e) override;
	void resizeEvent(QResizeEvent* e) override;
};
