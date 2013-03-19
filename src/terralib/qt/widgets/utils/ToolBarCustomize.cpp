#include "ToolBarCustomize.h"
#include <ui_ToolBarCustomizeForm.h>
#include "MenuBarModel.h"

//Qt
#include <QtGui/QToolBar>

te::qt::widgets::ToolBarCustomize::ToolBarCustomize(QMenuBar* mbar, std::vector<QToolBar*> tbars, QWidget* parent) :
QWidget(parent),
  m_ui(new Ui::ToolBarCustomizeForm)
{
  m_ui->setupUi(this);
  m_bars = tbars;

  for(size_t i=0; i<tbars.size(); i++)
    m_ui->m_toolbarList->addItem(new QListWidgetItem(tbars[i]->objectName()));

  m_ui->m_actionList->setModel(new MenuBarModel(mbar, this));

  connect (m_ui->m_toolbarList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), SLOT(toolbarChanged(QListWidgetItem*, QListWidgetItem*)));
}

te::qt::widgets::ToolBarCustomize::~ToolBarCustomize()
{
}

void te::qt::widgets::ToolBarCustomize::toolbarChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
  int r = m_ui->m_toolbarList->row(current);

  QToolBar* bar = m_bars[r];

  ((MenuBarModel*)m_ui->m_actionList->model())->updateActionsState(bar);
}