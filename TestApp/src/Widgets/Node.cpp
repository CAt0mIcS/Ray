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

void Node::Resize(int x, int y, int width, int height)
{
	this->move(this->pos() + QPoint{x, y});
	this->resize(this->size() + QSize{ width, height });
}

void Node::CreateNode(QWidget* parent, QPointF spawnPoint)
{
	Node* node = new Node(parent);
	//node->move(spawnPoint);

	node->setStyleSheet("QWidget{\nbackground-color: rgb(17, 18, 20);\nborder-color: rgb(115, 255, 0);\nborder: none;\nborder-radius: 50px;\n}\n\nQPlainTextEdit{\nborder-style: outset;\nborder-width: 2px;\nborder-radius: 10px;\nborder-color: rgb(35, 38, 40);\nmin-width: 10em;\npadding: 6px;\ncolor: rgb(230, 230, 230);\n}\n\nTextBox{\nborder-style: outset;\nborder-width: 2px;\nborder-radius: 10px;\nborder-color: rgb(35, 38, 40);\nmin-width: 10em;\npadding: 6px;\ncolor: rgb(230, 230, 230);\n}\n\nQMenu{\n	color: rgb(230, 230, 230);\n	background-color: rgb(35, 38, 40);\n}\n\nQMenu::item:selected{\n	background-color: rgb(120, 120, 120);\n}\n");
	node->setGeometry(spawnPoint.x(), spawnPoint.y(), 321, 171);

	TextBox* txtTitle = CreateTextbox(node);
	EditText* txtInfo = CreateEditText(node);
	PushButton* btnCon = CreateButton(node);

	txtTitle->raise();
	btnCon->raise();
	txtInfo->raise();


	node->show();
	txtTitle->show();
	txtInfo->show();
	btnCon->show();
}

TextBox* Node::CreateTextbox(Node* parent)
{
	TextBox* txtTitle = new TextBox(parent);
	txtTitle->setObjectName(QString::fromUtf8("txtTitle"));
	txtTitle->setGeometry(QRect(42, 20, 250, 40));
	QFont font;
	font.setFamily(QString::fromUtf8("Consolas"));
	font.setPointSize(15);
	font.setBold(false);
	font.setItalic(false);
	font.setWeight(50);
	txtTitle->setFont(font);
	txtTitle->setContextMenuPolicy(Qt::ActionsContextMenu);
	txtTitle->setStyleSheet(QString::fromUtf8(""));

	return txtTitle;
}

EditText* Node::CreateEditText(Node* parent)
{
	EditText* txtInfo = new EditText(parent);
	txtInfo->setObjectName(QString::fromUtf8("txtInfo"));
	txtInfo->setGeometry(QRect(42, 80, 250, 71));
	QFont font1;
	font1.setFamily(QString::fromUtf8("Consolas"));
	font1.setPointSize(10);
	txtInfo->setFont(font1);
	txtInfo->setContextMenuPolicy(Qt::ActionsContextMenu);
	txtInfo->setStyleSheet(QString::fromUtf8(""));
	txtInfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	txtInfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	return txtInfo;
}

PushButton* Node::CreateButton(Node* parent)
{
	PushButton* btnConDot = new PushButton(parent);
	btnConDot->setObjectName(QString::fromUtf8("btnConDot"));
	btnConDot->setGeometry(QRect(260, 0, 16, 16));
	btnConDot->setStyleSheet(QString::fromUtf8("border-image: url(:/TestApp/circle.png);"));
	return btnConDot;
}
