#pragma once

#include <QtWidgets/QWidget>

#include <vector>

class TextBox;
class EditText;
class PushButton;


class Node : public QWidget
{
	Q_OBJECT
public:
	Node(QWidget* parent = nullptr);

	void paintEvent(QPaintEvent* e) override;
	void resizeEvent(QResizeEvent* e) override;

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;

	void Resize(int x, int y, int width, int height);

	static void CreateNode(QWidget* parent, QPointF spawnPoint);

private:
	static TextBox* CreateTextbox(Node* parent);
	static EditText* CreateEditText(Node* parent);
	static PushButton* CreateButton(Node* parent);

private:
	QPoint m_MoveStartPos;
	unsigned int m_HeightLock;
};
