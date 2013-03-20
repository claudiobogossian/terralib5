#include "MenuBarModel.h"

//Qt
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>

std::vector<QAction*> GetActions(QList<QAction*> acts)
{
  std::vector<QAction*> res;
  QList<QAction*>::iterator it;

  for(it=acts.begin(); it != acts.end(); ++it)
    if((*it)->menu() == 0 && !((*it)->text().isEmpty()))
      res.push_back(*it);

  return res;
}

te::qt::widgets::MenuBarModel::MenuBarModel(QMenuBar* mnuBar, QObject* parent) :
QAbstractTableModel(parent)
{
  setMenuBar(mnuBar);
}

te::qt::widgets::MenuBarModel::~MenuBarModel()
{
}

Qt::ItemFlags	te::qt::widgets::MenuBarModel::flags (const QModelIndex & index) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}
          
QVariant te::qt::widgets::MenuBarModel::data (const QModelIndex & index, int role) const
{
  if(!index.isValid())
    return QVariant();

  switch(role)
  {
    case Qt::TextAlignmentRole:
      return int(Qt::AlignVCenter | Qt::AlignLeft);
    break;

    case Qt::DisplayRole:
      {
        QString name = m_actions[index.row()]->objectName();
        int pos = name.lastIndexOf(".");

        return (pos < 0) ? m_actions[index.row()]->text() : name.mid(pos+1); 
      }
    break;

    case Qt::DecorationRole:
      return m_actions[index.row()]->icon();
    break;

    case Qt::CheckStateRole:
      return (m_actionsActivation[index.row()]) ? Qt::Checked : Qt::Unchecked;
    break;

    default:
      return QVariant();
    break;
  }
}
          
QVariant te::qt::widgets::MenuBarModel::headerData (int section, Qt::Orientation orientation, int role) const
{
  return QVariant();
}
      
int te::qt::widgets::MenuBarModel::columnCount (const QModelIndex & parent) const
{
  return 1;
}
          
int te::qt::widgets::MenuBarModel::rowCount (const QModelIndex & parent) const
{
  return (int)m_actions.size();
}

bool te::qt::widgets::MenuBarModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
  if (!index.isValid())
    return false;

  if(role == Qt::CheckStateRole)
    m_actionsActivation[index.row()] = (value.toInt() == Qt::Checked) ?  true : false;

  return true;
}

void te::qt::widgets::MenuBarModel::updateActionsState(QToolBar* toolBar)
{
  m_actionsActivation.clear();
  m_actionsActivation = std::vector<bool>(m_actions.size(), false);

  QList<QAction*> acts = toolBar->actions();
  QList<QAction*>::iterator it;

  for(it = acts.begin(); it != acts.end(); ++it)
  {
    for(size_t i=0; i<m_actions.size(); i++)
      if((*it) == m_actions[i])
        m_actionsActivation[i] = true;
  }

  reset();
}

std::vector<QAction*> te::qt::widgets::MenuBarModel::getActions() const
{
  std::vector<QAction*> res;

  for (size_t i=0; i<m_actions.size(); i++)
    if(m_actionsActivation[i])
      res.push_back(m_actions[i]);

  return res;
}

void te::qt::widgets::MenuBarModel::setMenuBar(QMenuBar* mnuBar)
{
  QList<QAction*> acts = mnuBar->findChildren<QAction*>();
  m_actions = GetActions(acts);

  m_actionsActivation = std::vector<bool>(m_actions.size(), false);

  reset();
}
