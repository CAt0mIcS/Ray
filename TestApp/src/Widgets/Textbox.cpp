#include "Textbox.h"

#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStylePainter>

#include <QContextMenuEvent>
#include <QtWidgets/QMenu>

#include <QtWidgets/QSizeGrip>
#include <QtWidgets/QGridLayout>


TextBox::TextBox(QWidget* parent)
    : QLineEdit(parent), m_ActionLocked(new QAction("Lock position", this))
{
    this->addAction(m_ActionLocked);
    connect(m_ActionLocked, &QAction::triggered, this, [this](bool triggered = false) { OnLockPosition(m_ActionLocked); });
}



