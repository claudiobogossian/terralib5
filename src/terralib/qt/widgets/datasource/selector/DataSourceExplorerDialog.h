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
  \file terralib/qt/widgets/datasource/selector/DataSourceExplorerDialog.h

  \brief A dialog for selecting a data source.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_SELECTOR_INTERNAL_DATASOURCEEXPLORERDIALOG_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_SELECTOR_INTERNAL_DATASOURCEEXPLORERDIALOG_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../Config.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class DataSourceExplorerDialogForm; }

class QListWidgetItem;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSourceSelectorWidget;

      /*!
        \class DataSourceExplorerDialog

        \brief A dialog for selecting a data source.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT DataSourceExplorerDialog : public QDialog
      {
        Q_OBJECT

        public:

          DataSourceExplorerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DataSourceExplorerDialog();

          void setDataSourceToUse(const QString& dsType);

          const std::list<te::da::DataSourceInfoPtr>& getSelecteds() const;

          DataSourceSelectorWidget* getSelectorWidget() const;

        protected slots:

          void okPushButtonPressed();

        private:

          std::auto_ptr<Ui::DataSourceExplorerDialogForm> m_ui;
          std::auto_ptr<DataSourceSelectorWidget> m_selectorWidget;
          std::list<te::da::DataSourceInfoPtr> m_selecteds;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_SELECTOR_INTERNAL_DATASOURCEEXPLORERDIALOG_H

