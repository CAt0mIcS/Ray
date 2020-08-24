#include "TestApp.h"

#include <QPainter>
#include <QDebug>

struct LineCoordinates
{
    unsigned int x1, y1, x2, y2;
};

bool m_PaintLine = false;
LineCoordinates m_LineCoordinates;

QPoint m_ConStartPos{ 0, 0 };
QPoint m_ConNextPos{ 0, 0 };

bool press1 = true;


TestApp::TestApp(QWidget *parent)
    : QMainWindow(parent)
{
    this->installEventFilter(this);
    setMouseTracking(true);
    ui.setupUi(this);
}

void TestApp::mousePressEvent(QMouseEvent* e)
{
    qDebug() << "MousePos: " << e->localPos();

    //m_ConStartPos = e->pos();
    
    m_PaintLine = true;
    this->update();

    this->setFocus();
    QMainWindow::mousePressEvent(e);
}

void TestApp::mouseMoveEvent(QMouseEvent* e)
{
    if (m_ConStartPos.x() != 0)
    {
        m_ConNextPos = e->pos();
        m_PaintLine = true;
        this->update();
    }
    
}

bool TestApp::eventFiler(QObject* watched, QEvent* e)
{
    if (watched == ui.btnConDot)
    {
        qDebug() << "Event here\n";
    }
    return false;
}

void TestApp::paintEvent(QPaintEvent* e)
{
    if (m_PaintLine && m_ConNextPos.x() != 0 && m_ConStartPos.x() != 0)
    {
        QPainter painter(this);
        QPen pen;
        pen.setColor({ 230, 230, 230 });
        pen.setWidth(2);
        painter.setPen(pen);
        //painter.drawLine(m_LineCoordinates.x1, m_LineCoordinates.y1, m_LineCoordinates.x2, m_LineCoordinates.y2);
        painter.drawLine(m_ConStartPos, m_ConNextPos);

        m_PaintLine = false;
    }

    QMainWindow::paintEvent(e);
}

void TestApp::invokeLinePaint(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    m_PaintLine = true;
    m_ConStartPos = { (int)x2, (int)y2 };
    qDebug() << "StartPos: " << m_ConStartPos << "EndPos: " << m_ConNextPos;
    this->update();
}
