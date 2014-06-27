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
  \file terralib/qt/widgets/query/QueryDataSourceDialog.h

  \brief This file defines a class for a Query DataSource Dialog Dialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYDATASOURCEDIALOG_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYDATASOURCEDIALOG_H

// TerraLib
#include "../../../dataaccess/query/Where.h"
#include "../../../maptools/AbstractLayer.h"
#include "../table/DataSetTableModel.h"
#include "../mapdisplay/DataSetDisplay.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QColor>
#include <QDialog>
#include <QListWidgetItem>

namespace Ui { class QueryDataSourceDialogForm; }

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSet;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class QueryDataSourceDialog

        \brief This file defines a class for a Query Data Source Dialog Dialog.
      */
      class TEQTWIDGETSEXPORT QueryDataSourceDialog : public QDialog
      {
        Q_OBJECT

        public:

          QueryDataSourceDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~QueryDataSourceDialog();

        public:

          /*!
            \brief This method is used to set the list of layers
            
          */
          void setLayerList(std::list<te::map::AbstractLayerPtr>& layerList);

        protected:

          void loadDataSourcesInformation();

          void buildMap();

        protected slots:

          void onDataSourceSelected(int index);

          void onDataSetItemClicked(QListWidgetItem* item);

          void onApplyPushButtonClicked();

          void onClearPushButtonClicked();

          void onSQLEditorTextChanged();

          void onSaveSqlToolButtonClicked();

          void onOpenSqlToolButtonClicked();

        signals:

          /*! This signal is emitted when the layer objects must be highlighted. */
          void highlightLayerObjects(const te::map::AbstractLayerPtr& layer, te::da::DataSet* dataset, const QColor& color);

        private:

          std::auto_ptr<Ui::QueryDataSourceDialogForm> m_ui;

          te::qt::widgets::DataSetTableModel* m_tableModel;

          te::qt::widgets::DataSetDisplay* m_dataSetDisplay;

          std::map<std::string, Qt::GlobalColor> m_keyWords;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYDIALOG_H
