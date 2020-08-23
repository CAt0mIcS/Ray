#include "Node.h"

#include <QtWidgets/QSizeGrip>
#include <QtWidgets/QGridLayout>

#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStylePainter>

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

int runs = 0;
QSize overdueTop{};
void Node::resizeEvent(QResizeEvent* e)
{
	if (runs == 0)
	{
		++runs;
		return;
	}

	TextBox* txtTitle = findChild<TextBox*>("txtTitle");
	EditText* txtInfo = findChild<EditText*>("txtInfo");

	if (!txtInfo || !txtTitle)
		return;
	
	QSize diffSize = e->size() - e->oldSize();

	int spaceBetweenWidgets = 5;
	int txtTitleBottomCorner = txtTitle->pos().y() + txtTitle->size().height();
	int txtInfoTopCorner = txtInfo->pos().y();

	if (txtTitleBottomCorner + spaceBetweenWidgets < txtInfoTopCorner)
	{
		overdueTop += diffSize;
		txtTitle->resize(txtTitle->size() + diffSize);
	}
	else
	{
		txtTitle->resize({ txtTitle->size().width() + diffSize.width(), txtTitle->height() });
	}
	txtInfo->resize(txtInfo->size() + diffSize);

}
