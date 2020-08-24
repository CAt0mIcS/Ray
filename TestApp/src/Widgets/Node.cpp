#include "Node.h"

#include <QtWidgets/QSizeGrip>
#include <QtWidgets/QGridLayout>

#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStylePainter>

#include <QDebug>

#include "EditText.h"
#include "Textbox.h"
#include "PushButton.h"


Node::Node(QWidget* parent)
	: QWidget(parent), m_HeightLock(0)
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


void Node::resizeEvent(QResizeEvent* e)
{
	//unsigned char xLimit = 220;
	//unsigned char yLimit = 151;

	qDebug() << e->size();

	if (e->oldSize() == QSize(-1, -1))
		return;

	//Validate size (resizing too fast can cause issues)
	//TODO:

	TextBox* txtTitle = findChild<TextBox*>("txtTitle");
	EditText* txtInfo = findChild<EditText*>("txtInfo");
	PushButton* btnConDot = findChild<PushButton*>("btnConDot");

	if (!txtInfo || !txtTitle || !btnConDot)
		return;
	
	QSize diffSize = e->size() - e->oldSize();

	btnConDot->move({ btnConDot->pos().x() + diffSize.width(), btnConDot->pos().y() });

	int spaceBetweenWidgets = 5;
	int txtTitleBottomCorner = txtTitle->pos().y() + txtTitle->size().height();
	int txtInfoTopCorner = txtInfo->pos().y();

	// Top widget wants to be resized again
	if (!txtTitle->IsPositionLocked())
	{
		if (txtInfo->size().height() < m_HeightLock)
		{
			txtTitle->resize(txtTitle->size() + diffSize);
		}
		else if (txtTitleBottomCorner + spaceBetweenWidgets < txtInfoTopCorner)
		{
			txtTitle->resize(txtTitle->size() + diffSize);
			m_HeightLock = txtInfo->size().height();
		}
		else
		{
			txtTitle->resize({ txtTitle->size().width() + diffSize.width(), txtTitle->height() });
		}
	}
	if(!txtInfo->IsPositionLocked())
		txtInfo->resize(txtInfo->size() + diffSize);

}

void Node::mousePressEvent(QMouseEvent* e)
{
	this->setFocus();
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


