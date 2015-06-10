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
  \file terralib/qt/widgets/dataset/selector/DataSetSelectorWidget.h

  \brief A wizard for selecting datasets from a data source.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORWIDGET_H
#define __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORWIDGET_H

// TerraLib
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../Config.h"

// STL
#include <list>
#include <memory>
#include <set>

// Qt
#include <QWidget>

namespace Ui { class DataSetSelectorWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSetDisplay;
      class DataSetItem;
      class DataSetGroupItem;
      class DataSetTableView;
      class DataSetTreeView;

      /*!
        \class DataSetSelectorWidget

        \brief A wizard for selecting datasets from a data source.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT DataSetSelectorWidget : public QWidget
      {
        Q_OBJECT

        public:

          DataSetSelectorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DataSetSelectorWidget();

          void set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems = false);

          void setMapPreviewVisible(bool visible);

          void setMapPreviewEnabled(bool enabled);

          void setDataPreviewVisible(bool visible);

          void setDataPreviewEnabled(bool enabled);

          void setNumPreviewRows(int nrows);

          std::list<te::da::DataSetTypePtr> getCheckedDataSets() const;

          bool hasCheckedDataSets() const;

          std::list<std::string> getCheckedGeomProperties() const;

          std::list<te::da::DataSetTypePtr> getSelectedDataSets() const;

          bool hasSelectedDataSets() const;

          std::list<std::string> getSelectedGeomProperties() const;

          DataSetTreeView* getTreeView() const;

        protected:

          void previewMap(const te::da::DataSetTypePtr& dataset);

          void previewData(const te::da::DataSetTypePtr& dataset);

        protected slots:

          void onDataSetToggled(DataSetItem* item);

          void onDataSetClicked(DataSetItem* item);

          void onDataSetGroupToggled(DataSetGroupItem* item);

          void onMapPreviewToggled(bool on);

          void onDataPreviewToggled(bool on);

        private:

          struct DataSetComparer
          {
            bool operator()(const te::da::DataSetTypePtr& first, const te::da::DataSetTypePtr& second) const;
          };

          std::map<te::da::DataSetTypePtr, std::string, DataSetComparer> m_checkedDatasets;
          std::auto_ptr<Ui::DataSetSelectorWidgetForm> m_ui;
          std::auto_ptr<te::qt::widgets::DataSetDisplay> m_mapPreview;
          std::auto_ptr<DataSetTableView> m_tblView;
          std::auto_ptr<DataSetTreeView> m_datasetTreeView;
          te::da::DataSourceInfoPtr m_datasource;
          int m_nPreviewRows;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASET_SELECTOR_INTERNAL_DATASETSELECTORWIDGET_H

