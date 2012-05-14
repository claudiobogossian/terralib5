#include "HelpPushButton.h"
#include "HelpManager.h"

//Qt
#include <QMouseEvent>

te::qt::widgets::HelpPushButton::HelpPushButton(QWidget* parent) :
QPushButton(tr("&Help"), parent)
{
}

void te::qt::widgets::HelpPushButton::setPageReference(const QString& ref)
{
  m_pg_ref = ref;
}

void te::qt::widgets::HelpPushButton::mousePressEvent(QMouseEvent* e)
{
  QPushButton::mousePressEvent(e);

  if(e->button() == Qt::LeftButton)
    te::qt::widgets::HelpManager::getInstance().showHelp(m_pg_ref);
}
