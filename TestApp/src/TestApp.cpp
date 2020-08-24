#include "TestApp.h"

#include <QPainter>

TestApp::TestApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


}

void TestApp::mousePressEvent(QMouseEvent* e)
{
    QMainWindow::mousePressEvent(e);
}

void TestApp::paintEvent(QPaintEvent* e)
{
    QPainter painter;
    painter.begin(this);
    QPen pen;
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(20);
    painter.setPen(pen);

    painter.drawLine(QPoint(80, 100), QPoint(200, 300));
    QMainWindow::paintEvent(e);
}
