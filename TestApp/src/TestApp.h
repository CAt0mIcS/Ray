#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestApp.h"

class TestApp : public QMainWindow
{
    Q_OBJECT

public:
    TestApp(QWidget *parent = Q_NULLPTR);

    void mousePressEvent(QMouseEvent* e) override;

private:
    Ui::TestAppClass ui;
};
