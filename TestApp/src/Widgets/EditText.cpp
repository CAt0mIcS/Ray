#include "EditText.h"

#include <QtWidgets/QAction>


EditText::EditText(QWidget* parent)
	:QPlainTextEdit(parent), m_ActionLocked(new QAction("Lock position"))
{
	this->addAction(m_ActionLocked);
	connect(m_ActionLocked, &QAction::triggered, this, [this](bool triggered = false) { OnLockPosition(m_ActionLocked); });
}
