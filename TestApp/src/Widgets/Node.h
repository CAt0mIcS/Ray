#pragma once

#include <QtWidgets/QWidget>

#include <vector>


class Node : public QWidget
{
	Q_OBJECT
public:
	Node(QWidget* parent = nullptr);

	void paintEvent(QPaintEvent* e) override;
	void resizeEvent(QResizeEvent* e) override;

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;

private:

private:
	QPoint m_MoveStartPos;
};
