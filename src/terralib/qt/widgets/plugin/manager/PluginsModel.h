/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/qt/widgets/plugin/manager/PluginsModel.h

  \brief 
*/

#ifndef __TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINSMODEL_H
#define __TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINSMODEL_H

//STL
#include <vector>

//Qt
#include <QtCore/QAbstractTableModel>
#include <QtCore/QFlags>

namespace te
{
// Forward declarations
  namespace plugin
  {
    struct PluginInfo;
  }

  namespace qt
  {
    namespace widgets
    {
      class PluginsModel : public QAbstractTableModel
      {
        public:
          
          /*!
            \brief Define possible states for the plugin.
          */
          enum PluginStatus
          {
            No_operation = 0,
            To_add = 1,
            To_remove = 2,
            To_enable = 4,
            To_disable = 8,
            Loaded = 16,
            Unloaded = 32,
            Broked = 64
          };

          Q_DECLARE_FLAGS( te::qt::widgets::PluginsModel::PluginsStatus, te::qt::widgets::PluginsModel::PluginStatus )

          /*!
            \brief Constructor.
            
            \param parent Qt object parent.
          */
          PluginsModel(QObject* parent = 0);

          /*!
            \brief Destructor.
          */
          virtual ~PluginsModel();

          /*!
            \name QAbstractTableModel re-implementation methods.
            \brief Re-implementation of QAbstractTableModel methods.
          */
          //@{

          /*!
            \brief Returns the number of plugins regitered in plugins manager.

            \param parent Parent. (This is not used for this model).

            \return 8.
          */
          int rowCount(const QModelIndex& parent) const;

          /*!
            \brief Returns the number of columns.

            \param parent Parent index. (For this model it is not used)

            \return 8.
          */
          int columnCount(const QModelIndex& parent) const;

          /*!
            \brief Returns data identified by \a index.

            \param index Data index.
            \param role  Data role.

            \return Data as QVariant object.
          */
          QVariant data(const QModelIndex& index, int role) const;

          /*!
            \brief Returns the data to be presented as header.

            \param section     Column / Row number.
            \param orientation Horizontal / Vertical orientation options.
            \param role        Data role.

            \return Data to be presented as header.
          */
          QVariant headerData(int section, Qt::Orientation orientation, int role) const;

          /*!
            \brief Returns the flags associated with a given item.

            \param index Index of the cell.

            \return Flags related to the item.
          */
          Qt::ItemFlags flags(const QModelIndex & index) const;

          /*!
            \brief
          */
          bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

          void addPlugin(const te::plugin::PluginInfo* info, const PluginsStatus& status, const std::string& fileName);

          void removePlugins(const QModelIndexList& plgs);

          void getPluginsInfo(std::vector<te::plugin::PluginInfo*>& plgs, std::vector<PluginsStatus>& status, std::vector<std::string>& files);

          void clear();

        protected:

          std::vector<te::plugin::PluginInfo*> m_plugins;
          std::vector<PluginsStatus> m_pluginsStatus;
          std::vector<std::string> m_pluginsFiles; 
      };

      Q_DECLARE_OPERATORS_FOR_FLAGS( te::qt::widgets::PluginsModel::PluginsStatus )
    }
  }
}



#endif //__TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINSMODEL_H
