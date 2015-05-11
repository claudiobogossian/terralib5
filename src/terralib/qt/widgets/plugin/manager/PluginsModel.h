/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file PluginsModel.h

  \brief 
*/

#ifndef __TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINSMODEL_H
#define __TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINSMODEL_H

//Qt
#include <QtCore/QAbstractTableModel>
#include <QtCore/QFlags>

//STL
#include <vector>

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
            \enum PluginStatus

            \brief Define possible states for the plugin.
          */
          enum PluginStatus
          {
            No_operation = 0,   //!< No operation.
            To_add = 1,         //!< Plugin to be added.
            To_remove = 2,      //!< Plugin to be removed.
            To_enable = 4,      //!< Plugin to be loaded.
            To_disable = 8,     //!< Plugin to be unloaded.
            Loaded = 16,        //!< Plugin is loaded.
            Unloaded = 32,      //!< Plugin is unloaded.
            Broked = 64         //!< Plugin haas missed dependencies.
          };

          Q_DECLARE_FLAGS( PluginsStatus, PluginStatus )

        /*!
          \name Constructor and destructor.
          \brief Methods used to construct and destruct the object.
        */
        //@{

          /*!
            \brief Constructor.
            
            \param parent Qt object parent.
          */
          PluginsModel(QObject* parent=0);

          /*!
            \brief Destructor.
          */
          virtual ~PluginsModel();
        //@}

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
          int rowCount(const QModelIndex & parent) const;

          /*!
            \brief Returns the number of columns.    
            \param parent Parent index. (For this model it is not used)
            \return 8.
          */
          int columnCount(const QModelIndex & parent) const;

          /*!
            \brief Returns data identified by \a index.    
            \param index Data index.
            \param role Data role.
            \return Data as QVariant object.
          */
          QVariant data(const QModelIndex & index, int role) const;

          /*!
            \brief Returns the data to be presented as header.    
            \param section Column / Row number.
            \param orientation Horizontal / Vertical orientation options.
            \param role Data role.
            \return Data to be presented as header.
          */
          QVariant headerData(int section, Qt::Orientation orientation, int role) const;

          /*!
            \brief Returns the flags associated with a given item.    
            \param index Index of the cell.
            \return Flags related to the item.
          */
          Qt::ItemFlags flags(const QModelIndex & index) const;

          bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
          //@}

          /*!
            \brief Adds information about plugin.

            \param info Information of the plugin.

            \param status Status of plugin.

            \sa PluginStatus
          */
          void addPlugin(const te::plugin::PluginInfo* info, const PluginsStatus& status);

          /*!
            \brief Remove the selected plugins.

            \param plgs Indexes of the plugins to be removed.
          */
          void removePlugins(const QModelIndexList& plgs);

          /*!
            \brief Returns plugins informations.

            \param[out] plgs List of plugins.

            \param[out] status List of the status of the plugins.
          */
          void getPluginsInfo(std::vector<te::plugin::PluginInfo*>& plgs, std::vector<PluginsStatus>& status);

          /*!
            \brief Clear the list of plugins and status.
          */
          void clear();

        protected:

          std::vector<te::plugin::PluginInfo*> m_plugins;       //!< List of informations about plugins.
          std::vector<PluginsStatus>           m_pluginsStatus; //!< List of status of each plugin.
      };

      Q_DECLARE_OPERATORS_FOR_FLAGS( te::qt::widgets::PluginsModel::PluginsStatus )
    }
  }
}



#endif //__TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINSMODEL_H
