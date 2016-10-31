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

// Qt
#include <QDialog>

// Forward declaration
namespace Ui { class PluginManagerDialogForm; }

class QTableWidget;
class QTableWidgetItem;

namespace te
{
  namespace core
  {
// Forward declaration
    struct PluginInfo;
  }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT PluginManagerDialog : public QDialog
      {
        Q_OBJECT
        enum class PluginStatus
        {
          loaded,
          unloaded,
          broken,
          to_load,
          to_unload,
          to_remove
        };

        enum PluginHeader
        {
          enabled = 0,
          display_name = 1,
          version = 2,
          license_description = 3,
          site = 4,
          provider_name = 5,
          provider_site = 6,
          provider_email = 7,
          name = 8
        };

        public:

          PluginManagerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~PluginManagerDialog();

        protected slots:
          void onAddPushButtonClicked();
          void onApplyPushButtonClicked();
          void onClosePushButtonClicked();
          void onEnableAllChanged(int state);
          void onEnabledChanged(int state);
          void onHelpPushButtonClicked();
          void onRemovePushButtonClicked();

        protected:
          void addEntry(const te::core::PluginInfo& pinfo, PluginStatus status);
          void changePluginStatus(const std::string& plugin_name);
          void disableRow(const int row);
          void fillInstalledPlugins();
          std::string getPluginName(int row);
          int getPluginRow(const std::string& plugin_name);
          PluginStatus getPluginStatus(const int row);
          void loadPlugins(std::vector<te::core::PluginInfo> v_pInfo);
          void removeEntries(std::vector<te::core::PluginInfo> v_pInfo);
          void setChanged(const int row, bool bold);
          void unloadPlugins(std::vector<core::PluginInfo> v_pInfo);
          void updateBroken();

        private:

          std::unique_ptr<Ui::PluginManagerDialogForm> m_ui;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WDIGETS_PLUGIN_MANAGER_INTERNAL_PLUGINMANAGERDIALOG_H
