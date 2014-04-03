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
  \file terralib/qt/widgets/datasource/selector/DataSourceSelectorWidget.h

  \brief A dialog for selecting a data source.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASOURCE_SELECTOR_INTERNAL_DATASOURCESELECTORWIDGET_H
#define __TERRALIB_QT_WIDGETS_DATASOURCE_SELECTOR_INTERNAL_DATASOURCESELECTORWIDGET_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../Config.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QAbstractItemView>
#include <QWidget>

namespace Ui { class DataSourceSelectorWidgetForm; }

class QListWidgetItem;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataSourceSelectorWidget

        \brief A dialog for selecting a data source.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT DataSourceSelectorWidget : public QWidget
      {
        Q_OBJECT

        public:

          enum
          {
            BUTTON_ADD_DATASOURCE,
            BUTTON_REMOVE_DATASOURCE,
            BUTTON_EDIT_DATASOURCE,
            BUTTON_CREATE_DATASOURCE
          };

          DataSourceSelectorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DataSourceSelectorWidget();

          Ui::DataSourceSelectorWidgetForm* getForm() const;

          void setSelectionMode(QAbstractItemView::SelectionMode mode);

          void setButtonsEnabled(bool enabled);

          void setButtonEnabled(int button, bool enabled);

          //void selectDataSourceType(int row);

          std::list<te::da::DataSourceInfoPtr> getSelecteds() const;

          void setDataSource(const QString& dsType);

          void showDataSourceWithDatabaseSupport(bool visible);

          void showDataSourceWithFileSupport(bool visible);

          void showDataSourceWithRasterSupport(bool visible);

          void showDataSourceWithVectorialSupport(bool visible);

        //signals:

          //void hasSelectedDataSource(bool selected);

          //void created(const DataSourcePtr& ds);

          //void updated(const DataSourcePtr& ds);

          //void removing(const DataSourcePtr& ds);

          //void selected(const DataSourcePtr& ds);

        protected slots:

          void addDataSourcePushButtonPressed();

          void removeDataSourcePushButtonPressed();

          void editDataSourcePushButtonPressed();

          void createDataSourcePushButtonPressed();

          void dataSourceTypePressed(QListWidgetItem* item);

          void dataSourcePressed(QListWidgetItem* item);
          
          void dataSourceDoubleClicked(QListWidgetItem * item);

        private:

          std::auto_ptr<Ui::DataSourceSelectorWidgetForm> m_ui;
          bool m_addButtonEnabled;
          bool m_removeButtonEnabled;
          bool m_editButtonEnabled;
          bool m_createButtonEnabled;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASOURCE_SELECTOR_INTERNAL_DATASOURCESELECTORWIDGET_H

