#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestApp.h"

class TestApp : public QMainWindow
{
    Q_OBJECT

public:
    TestApp(QWidget *parent = Q_NULLPTR);

private:
    Ui::TestAppClass ui;
};
