#include "CustomizeToolBarsDialog.h"
#include <ui_CustomizeToolBarsDialogForm.h>
//#include "MenuBarModel.h"

//Qt
#include <QtGui/QToolBar>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

void UpdateToolBar(QToolBar* bar, const std::vector<QAction*>& acts)
{
  //std::vector<QAction*>::const_iterator it;

  //bar->clear();

  //for(it=acts.begin(); it!=acts.end(); ++it)
  //  bar->addAction(*it);
}

te::qt::widgets::CustomizeToolBarsDialog::CustomizeToolBarsDialog(QMenuBar* mbar, std::vector<QToolBar*> tbars, QWidget* parent) :
  QDialog(parent),
  m_removing(false),
  m_ui(new Ui::CustomizeToolBarsDialogForm)
{
  m_ui->setupUi(this);

  //m_bars = tbars;

  //for(size_t i=0; i<tbars.size(); i++)
  //  m_ui->m_toolbarList->addItem(new QListWidgetItem(tbars[i]->objectName()));


  //MenuBarModel* mdel = new MenuBarModel(mbar, this);
  //m_ui->m_actionList->setModel(mdel);

  //m_ui->m_toolbarList->setCurrentRow(0);
  //mdel->updateActionsState(tbars[0]);

  ////Setting icons
  //m_ui->m_addToolButton->setIcon(QIcon::fromTheme("list-add"));
  //m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));
  //m_ui->m_toolbarList->setSelectionMode(QAbstractItemView::SingleSelection);

  //// Signal / slots connection
  //connect (m_ui->m_toolbarList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), SLOT(onToolbarChanged(QListWidgetItem*, QListWidgetItem*)));
  //connect (m_ui->m_addToolButton, SIGNAL(clicked()), SLOT(onAddToolButtonClicked()));
  //connect (m_ui->m_removeToolButton,SIGNAL(clicked()), SLOT(onRemoveToolButtonClicked()));
  //connect (m_ui->m_closePushButton, SIGNAL(clicked()), SLOT(close()));
}

te::qt::widgets::CustomizeToolBarsDialog::~CustomizeToolBarsDialog()
{
}

std::vector<QToolBar*> te::qt::widgets::CustomizeToolBarsDialog::getCreatedToolBars() const
{
  return m_createdBars;
}

std::vector<QString> te::qt::widgets::CustomizeToolBarsDialog::getRemovedBars() const
{
  return m_removedBars;
}

void te::qt::widgets::CustomizeToolBarsDialog::onAddToolButtonClicked()
{
  //bool ok;
  //QString text = QInputDialog::getText(this, tr("Creating tool bar"), tr("Tool bar name:"), QLineEdit::Normal, tr("Name of the new toolbar"), &ok);

  //if (!ok || text.isEmpty())
  //{
  //  QMessageBox::warning(this, tr("Creating tool bar"), tr("Empty tool bar name not allowed!"));
  //  return;
  //}

  //QToolBar* bar = new QToolBar;
  //bar->setObjectName(text);

  //m_bars.push_back(bar);
  //m_createdBars.push_back(bar);

  //m_ui->m_toolbarList->addItem(new QListWidgetItem(bar->objectName()));

  //size_t sz = m_bars.size();

  //m_ui->m_toolbarList->setCurrentRow(sz-1);
  //((MenuBarModel*)m_ui->m_actionList->model())->updateActionsState(m_bars[sz-1]);
}

void te::qt::widgets::CustomizeToolBarsDialog::onRemoveToolButtonClicked()
{
  //m_removing = true;

  //int r = m_ui->m_toolbarList->row(m_ui->m_toolbarList->currentItem());
  //QToolBar* bar = m_bars[r];

  //m_removedBars.push_back(bar->objectName());
  //m_bars.erase(m_bars.begin()+r);

  //delete m_ui->m_toolbarList->takeItem(r);
  //delete bar;

  //if(!m_bars.empty())
  //{
  //   m_ui->m_toolbarList->setCurrentRow(0);
  //   ((MenuBarModel*)m_ui->m_actionList->model())->updateActionsState(m_bars[0]);
  //}

  //m_removing = false;
}

void te::qt::widgets::CustomizeToolBarsDialog::onToolbarChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
  //// Check if has remotion
  //if(m_removing)
  //{
  //  if(!m_bars.empty())
  //  {
  //     m_ui->m_toolbarList->setCurrentRow(0);
  //     ((MenuBarModel*)m_ui->m_actionList->model())->updateActionsState(m_bars[0]);
  //  }

  //  return;
  //}

  //// Updating previous
  //int r = m_ui->m_toolbarList->row(previous);

  //if(r>=0)
  //{
  //  QToolBar* bar = m_bars[r];
  //  UpdateToolBar(bar, ((MenuBarModel*)m_ui->m_actionList->model())->getActions());
  //}

  //// Setting the current
  //r = m_ui->m_toolbarList->row(current);

  //QToolBar* bar = m_bars[r];

  //((MenuBarModel*)m_ui->m_actionList->model())->updateActionsState(bar);
}

void te::qt::widgets::CustomizeToolBarsDialog::closeEvent (QCloseEvent * event)
{
  //int r = m_ui->m_toolbarList->row(m_ui->m_toolbarList->currentItem());
  //QToolBar* bar = m_bars[r];

  //UpdateToolBar(bar, ((MenuBarModel*)m_ui->m_actionList->model())->getActions());
}
