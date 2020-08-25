#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestApp.h"


class TestApp : public QMainWindow
{
    Q_OBJECT

public:
    TestApp(QWidget *parent = Q_NULLPTR);

    void invokeLinePaint(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

protected:
    bool eventFiler(QObject* watched, QEvent* e);

    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;

    void paintEvent(QPaintEvent* e) override;

    bool event(QEvent* e) override;


private:
    Ui::TestAppClass ui;
    QPointF m_MousePos;

};
