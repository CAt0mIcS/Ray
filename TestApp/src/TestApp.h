#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestApp.h"


class TestApp : public QMainWindow
{
    Q_OBJECT

public:
    TestApp(QWidget *parent = Q_NULLPTR);

    void mousePressEvent(QMouseEvent* e) override;

    void paintEvent(QPaintEvent* e) override;
    void invokeLinePaint(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

private:
    Ui::TestAppClass ui;

};
