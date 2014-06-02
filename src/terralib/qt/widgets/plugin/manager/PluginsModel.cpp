#include "PluginsModel.h"


// TerraLib
#include "../../../../plugin/PluginInfo.h"
#include "../../../../plugin/PluginManager.h"

// Qt
#include <QFont>

std::string GetData(const int& column, const te::plugin::PluginInfo& info)
{
  std::string result;

  switch(column)
  {
    case 1:
      result = info.m_displayName;
    break;

    case 2:
      result = info.m_version;
    break;

    case 3:
      result = info.m_licenseDescription;
    break;

    case 4:
      result = info.m_category;
    break;

    case 5:
      result = info.m_site;
    break;

    case 6:
      result = info.m_provider.m_name;
    break;

    case 7:
      result = info.m_provider.m_site;
    break;

    case 8:
      result = info.m_provider.m_email;
    break;

    case 9:
      result = info.m_name;
    break;

    default:
      break;
  };

  return result;
}

QString GetHeader(const int& section)
{
  QString header;

  switch(section)
  {
    case 0:
      header = QObject::tr("Enabling");
    break;

    case 1:
      header = QObject::tr("Plugin");
    break;

    case 2:
      header = QObject::tr("Version");
    break;

    case 3:
      header = QObject::tr("License");
    break;

    case 4:
      header = QObject::tr("Category");
    break;

    case 5:
      header = QObject::tr("Site");
    break;

    case 6:
      header = QObject::tr("Provider");
    break;

    case 7:
      header = QObject::tr("Provider site");
    break;

    case 8:
      header = QObject::tr("Provider email");
    break;

    case 9:
      header = QObject::tr("Name");
    break;

  };

  return header;
}

te::qt::widgets::PluginsModel::PluginsModel(QObject* parent) :
  QAbstractTableModel(parent)
{
}

te::qt::widgets::PluginsModel::~PluginsModel()
{
}

int te::qt::widgets::PluginsModel::rowCount(const QModelIndex & parent) const
{
  return (int) m_plugins.size();
}

int te::qt::widgets::PluginsModel::columnCount(const QModelIndex & parent) const
{
  return 10;
}

QVariant te::qt::widgets::PluginsModel::data(const QModelIndex & index, int role) const
{
  if(!index.isValid())
    return QVariant();

  switch(role)
  {
    case Qt::TextAlignmentRole:
      return int(Qt::AlignVCenter | Qt::AlignLeft);
    break;

    case Qt::DisplayRole:
      return (index.column() > 0) ? 
        QString(GetData(index.column(), *(m_plugins[index.row()])).c_str()) :
        QVariant();
    break;

    case Qt::FontRole:
      {
        QFont f;
        PluginsStatus st =  m_pluginsStatus[index.row()];

        if(st.testFlag(To_add) || st.testFlag(To_enable) || st.testFlag(To_disable))
          f.setBold(true);
        else if(st.testFlag(To_remove))
          f.setItalic(true);

        return f;
      }
    break;

    case Qt::CheckStateRole:
      {
        PluginsStatus st =  m_pluginsStatus[index.row()];

        if(!st.testFlag(Broked) && index.column() == 0)
          return ((st.testFlag(Loaded) && !(st.testFlag(To_disable))) || st.testFlag(To_enable)) ? Qt::Checked : Qt::Unchecked;

        return QVariant();
      }
    break;

    default:
      return QVariant();
    break;
  }
}

QVariant te::qt::widgets::PluginsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  switch (role)
  {
    case Qt::DisplayRole:
      return (orientation == Qt::Horizontal) ?
        GetHeader(section) :
        QVariant::fromValue<int>(section+1);
    break;

    default:
      return QAbstractTableModel::headerData(section, orientation, role);
  };
}

Qt::ItemFlags te::qt::widgets::PluginsModel::flags(const QModelIndex & index) const
{
  if (!index.isValid())
    return 0;

  Qt::ItemFlags flags(0);
  
  if(!m_pluginsStatus[index.row()].testFlag(To_remove))
  {
    flags = flags | Qt::ItemIsEnabled;

    if(index.column() == 0)
      flags = flags | Qt::ItemIsUserCheckable | Qt::ItemIsEditable;

    flags = flags | Qt::ItemIsSelectable;
  }
    
  return flags;
}

bool te::qt::widgets::PluginsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
  if(!index.isValid())
    return false;

  if(role == Qt::CheckStateRole && !m_pluginsStatus[index.row()].testFlag(Broked))
  {
    if(value.toInt() == Qt::Checked)
    {
      m_pluginsStatus[index.row()] &= ~To_disable;
      m_pluginsStatus[index.row()] |= To_enable;
    }
    else
    {
      m_pluginsStatus[index.row()] &= ~To_enable;
      m_pluginsStatus[index.row()] |= To_disable;
    }

    QModelIndex idx;

    emit dataChanged (idx, idx);
  }

  return true;
}

void te::qt::widgets::PluginsModel::addPlugin(const te::plugin::PluginInfo* info, const PluginsStatus& status)
{
  m_plugins.push_back(new te::plugin::PluginInfo(*info));
  m_pluginsStatus.push_back(status);

#if (QT_VERSION < 0x050000)
  reset();
#else
  beginResetModel();
  endResetModel();
#endif
}

void te::qt::widgets::PluginsModel::removePlugins(const QModelIndexList& plgs)
{
  QModelIndexList::ConstIterator it;

  for(it=plgs.constBegin(); it!=plgs.constEnd(); ++it)
    m_pluginsStatus[(*it).row()] = To_remove;

#if (QT_VERSION < 0x050000)
  reset();
#else
  beginResetModel();
  endResetModel();
#endif

}

void te::qt::widgets::PluginsModel::getPluginsInfo(std::vector<te::plugin::PluginInfo*>& plgs, std::vector<PluginsStatus>& status)
{
  plgs = m_plugins;
  status = m_pluginsStatus;
}

void te::qt::widgets::PluginsModel::clear()
{
  std::vector<te::plugin::PluginInfo*>::iterator it;

  for(it = m_plugins.begin(); it != m_plugins.end(); ++it)
    delete *it;

  m_plugins.clear();
  m_pluginsStatus.clear();
}
