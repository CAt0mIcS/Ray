#include "Node.h"

#include <QtWidgets/QSizeGrip>
#include <QtWidgets/QGridLayout>

#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStylePainter>

#include <QtWidgets/QGraphicsView>

#include <QTimer>

#include "EditText.h"
#include "Textbox.h"

Node::Node(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlags(Qt::SubWindow);
	QSizeGrip* sizeGrip = new QSizeGrip(this);
	QGridLayout* layout = new QGridLayout(this);
	layout->addWidget(sizeGrip, 0, 0, 1, 1, Qt::AlignBottom | Qt::AlignRight);
}

void Node::paintEvent(QPaintEvent* e)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

int heightPrev = 0;
void Node::resizeEvent(QResizeEvent* e)
{

	if (e->oldSize() == QSize(-1, -1))
		return;

	TextBox* txtTitle = findChild<TextBox*>("txtTitle");
	EditText* txtInfo = findChild<EditText*>("txtInfo");

	if (!txtInfo || !txtTitle)
		return;
	
	QSize diffSize = e->size() - e->oldSize();

	int spaceBetweenWidgets = 5;
	int txtTitleBottomCorner = txtTitle->pos().y() + txtTitle->size().height();
	int txtInfoTopCorner = txtInfo->pos().y();

	// Top widget wants to be resized again
	if (txtInfo->size().height() < heightPrev)
	{
		if(!txtTitle->IsPositionLocked())
			txtTitle->resize(txtTitle->size() + diffSize);
	}
	else if (txtTitleBottomCorner + spaceBetweenWidgets < txtInfoTopCorner)
	{
		if(!txtTitle->IsPositionLocked())
			txtTitle->resize(txtTitle->size() + diffSize);
		heightPrev = txtInfo->size().height();
	}
	else
	{
		if(!txtTitle->IsPositionLocked())
			txtTitle->resize({ txtTitle->size().width() + diffSize.width(), txtTitle->height() });
	}
	if(!txtInfo->IsPositionLocked())
		txtInfo->resize(txtInfo->size() + diffSize);

}

void Node::mousePressEvent(QMouseEvent* e)
{
	m_MoveStartPos = e->pos();
}

void Node::mouseMoveEvent(QMouseEvent* e)
{
	QPoint delta = e->pos() - m_MoveStartPos;
	this->move(this->pos() + delta);
}

void Node::wheelEvent(QWheelEvent* e)
{
	
}


