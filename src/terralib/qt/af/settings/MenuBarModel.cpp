#include "MenuBarModel.h"

//Qt
#include <QMenuBar>
#include <QToolBar>

std::vector<QAction*> GetActions(QList<QAction*> acts)
{
  std::vector<QAction*> res;
  QList<QAction*>::iterator it;

  for(it=acts.begin(); it != acts.end(); ++it)
    if((*it)->menu() == 0 && !((*it)->text().isEmpty()) && !(*it)->icon().isNull())
      res.push_back(*it);

  return res;
}

te::qt::af::MenuBarModel::MenuBarModel(QMenuBar* mnuBar, QObject* parent) :
QAbstractTableModel(parent)
{
  setMenuBar(mnuBar);
}

te::qt::af::MenuBarModel::~MenuBarModel()
{
}

Qt::ItemFlags	te::qt::af::MenuBarModel::flags (const QModelIndex & index) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}
          
QVariant te::qt::af::MenuBarModel::data (const QModelIndex & index, int role) const
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
          
QVariant te::qt::af::MenuBarModel::headerData (int section, Qt::Orientation orientation, int role) const
{
  return QVariant();
}
      
int te::qt::af::MenuBarModel::columnCount (const QModelIndex & parent) const
{
  return 1;
}
          
int te::qt::af::MenuBarModel::rowCount (const QModelIndex & parent) const
{
  return (int)m_actions.size();
}

bool te::qt::af::MenuBarModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
  if (!index.isValid())
    return false;

  int r = index.row();

  if(role == Qt::CheckStateRole)
    m_actionsActivation[r] = (value.toInt() == Qt::Checked) ?  true : false;

  QAction* act = m_actions[r];
  bool toAdd = m_actionsActivation[r];

  emit updateAction(act, toAdd);

  return true;
}

void te::qt::af::MenuBarModel::updateActionsState(const QList<QAction*>& acts)
{
  QList<QAction*>::const_iterator it;

  m_actionsActivation.clear();
  m_actionsActivation = std::vector<bool>(m_actions.size(), false);

  for(it = acts.begin(); it != acts.end(); ++it)
  {
    for(size_t i=0; i<m_actions.size(); i++)
      if((*it) == m_actions[i])
        m_actionsActivation[i] = true;
  }

#if (QT_VERSION < 0x050000)
  reset();
#else
  beginResetModel();
  endResetModel();
#endif
}

void te::qt::af::MenuBarModel::setMenuBar(QMenuBar* mnuBar)
{
  QList<QAction*> acts = mnuBar->findChildren<QAction*>();
  m_actions = GetActions(acts);

  m_actionsActivation = std::vector<bool>(m_actions.size(), false);

#if (QT_VERSION < 0x050000)
  reset();
#else
  beginResetModel();
  endResetModel();
#endif
}
