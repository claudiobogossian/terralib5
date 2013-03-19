#include "AddToolBarDialog.h"
#include <ui_AddToolBarDialogForm.h>
#include "MenuBarModel.h"

#include <QtGui/QToolBar>
#include <QtGui/QMessageBox>

te::qt::widgets::AddToolBarDialog::AddToolBarDialog (QWidget* parent) :
QDialog(parent),
m_ui(new Ui::AddToolBarDialogForm),
m_model(0)
{
  m_ui->setupUi(this);

  m_ui->m_helpPushButton->setEnabled(false);

  connect(m_ui->m_okPushButton, SIGNAL(pressed()), SLOT(onOkButtonPressed()));
  connect(m_ui->m_cancelPushButton, SIGNAL(pressed()), SLOT(reject()));
}

te::qt::widgets::AddToolBarDialog::~AddToolBarDialog()
{
}

void te::qt::widgets::AddToolBarDialog::setAvailableActions(QMenuBar* bar)
{
  m_model = new MenuBarModel(bar, this);
  m_ui->m_actions->setModel(m_model);
}

QToolBar* te::qt::widgets::AddToolBarDialog::getCreatedToolBar() const
{
  QString name = m_ui->m_toolbarName->text();
  std::vector<QAction*> acts = m_model->getActions();
  std::vector<QAction*>::iterator it;

  QToolBar* bar = new QToolBar;
  bar->setObjectName(name);

  for(it=acts.begin(); it!=acts.end(); ++it)
    bar->addAction(*it);

  return bar;
}

void te::qt::widgets::AddToolBarDialog::onOkButtonPressed()
{
  QString name = m_ui->m_toolbarName->text();

  if(name.isEmpty())
  {
    QMessageBox::warning(this, tr("TerraLib Qt Components"), tr("ToolBar name could not be empty!"));
    return;
  }

  std::vector<QAction*> acts = m_model->getActions();

  if(acts.empty())
  {
    QMessageBox::warning(this, tr("TerraLib Qt Components"), tr("ToolBar actions could not be empty!"));
    return;
  }

  accept();
}
