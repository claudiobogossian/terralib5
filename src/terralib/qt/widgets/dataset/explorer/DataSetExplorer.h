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
  \file terralib/qt/widgets/dataset/explorer/DataSetExplorer.h

  \brief A widget designed to explore the datasets of a data source.
*/

#ifndef __TERRALIB_QT_WIDGETS_DATASET_EXPLORER_INTERNAL_DATASETEXPLORER_H
#define __TERRALIB_QT_WIDGETS_DATASET_EXPLORER_INTERNAL_DATASETEXPLORER_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../Config.h"

// STL
#include <memory>

// Qt
#include <QDockWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class DataSetTreeView;

      /*!
        \class DataSetExplorer

        \brief A widget designed to explore the datasets of a data source.

        \ingroup widgets

        \sa DataSetExplorerTreeView, DataSetExplorerTreeModel
      */
      class TEQTWIDGETSEXPORT DataSetExplorer : public QDockWidget
      {
        Q_OBJECT

        public:

          DataSetExplorer(QWidget* parent = 0, Qt::WindowFlags flags = 0);

          ~DataSetExplorer();

          void set(const te::da::DataSourceInfoPtr& ds);

          DataSetTreeView* getTreeView() const;

        private:

          std::auto_ptr<DataSetTreeView> m_treeView;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_DATASET_EXPLORER_INTERNAL_DATASETEXPLORER_H
