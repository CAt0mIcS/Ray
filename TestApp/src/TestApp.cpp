#include "TestApp.h"

#include <QPainter>
#include <QDebug>

struct LineCoordinates
{
    unsigned int x1, y1, x2, y2;
};

bool m_PaintLine;
LineCoordinates m_LineCoordinates;


TestApp::TestApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


}

void TestApp::mousePressEvent(QMouseEvent* e)
{
    qDebug() << "MousePos: " << e->localPos();

    this->setFocus();
    QMainWindow::mousePressEvent(e);
}

void TestApp::paintEvent(QPaintEvent* e)
{
    if (!m_PaintLine)
    {
        QPainter painter(this);
        QPen pen;
        pen.setColor({ 230, 230, 230 });
        pen.setWidth(2);
        painter.setPen(pen);
        //painter.drawLine(m_LineCoordinates.x1, m_LineCoordinates.y1, m_LineCoordinates.x2, m_LineCoordinates.y2);
        painter.drawLine(850, 345, 300, 300);

        m_PaintLine = false;
    }

    QMainWindow::paintEvent(e);
}

void TestApp::invokeLinePaint(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    m_PaintLine = true;
    m_LineCoordinates = { x1, y1, x2, y2 };
    qDebug() << "x1: " << m_LineCoordinates.x1 << "\ny1: " << m_LineCoordinates.y1 << "\nx2: " << m_LineCoordinates.x2 << "\ny2: " << m_LineCoordinates.y2;
    this->paintEvent(nullptr);
}
