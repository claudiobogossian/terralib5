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
  \file terralib/qt/widgets/plugin/manager/PluginManagerDialog.h

  \brief A Qt dialog for plugin management.
*/

#ifndef __TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINMANAGERDIALOG_H
#define __TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINMANAGERDIALOG_H

// TerraLib
#include "../../Config.h"

// STL
#include <memory>
#include <set>

// Qt
#include <QDialog>

// Forward declaration
namespace Ui { class PluginManagerDialogForm; }

class QModelIndex;
class QNetworkReply;
class QTableView;
class QTableWidget;
class QTableWidgetItem;

namespace te
{
  namespace plugin
  {
// Forward declaration
    struct PluginInfo;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declaration
      class PluginsModel;

      //struct iPluginInfo;

      class TEQTWIDGETSEXPORT PluginManagerDialog : public QDialog
      {
        Q_OBJECT

        public:

          PluginManagerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~PluginManagerDialog();

        protected slots:

          void applyPushButtonPressed();

          void tableWidgetClicked(QTableWidgetItem* item);

          void replyFinished(QNetworkReply*);

          void addPlugins();

          void removePlugins();

          void dataChanged(const QModelIndex&, const QModelIndex&);

          void onEnableAllChanged(bool st);

        protected:

          void fillInstalledPlugins();
          
          void filliPlugins();

          void addEntry(int i, const te::plugin::PluginInfo& pinfo, bool checked);

        private:

          std::auto_ptr<Ui::PluginManagerDialogForm> m_ui;
          //std::set<iPluginInfo> m_iplugins;
          std::set<int> m_changedInstalledPlugins;
          std::set<int> m_downloadPlugins;
          PluginsModel* m_model;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINMANAGERDIALOG_H
