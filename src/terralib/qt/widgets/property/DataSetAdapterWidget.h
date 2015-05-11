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
  \file terralib/qt/widgets/property/DataSetAdapterWidget.h

  \brief A class used to define the DataSetAdapterWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_DATASETADAPTERWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_DATASETADAPTERWIDGET_H

// TerraLib
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <map>

namespace Ui { class DataSetAdapterWidgetForm; }

namespace te
{
  namespace da
  {
    class DataSource;
    class DataSet;
    class DataSetType;
    class DataSetTypeConverter;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataSetAdapterWidget

        \brief A dialog for creating a data set adapter
      */
      class TEQTWIDGETSEXPORT DataSetAdapterWidget : public QWidget
      {
        Q_OBJECT

        public:

          DataSetAdapterWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~DataSetAdapterWidget();

        public:

          /*!
            \brief Used to set the needed information to create a data set adapter.

            \param targetDS
            \param sourceDataSet
          */
          void setAdapterParameters(te::da::DataSetType* sourceDataSetType, te::da::DataSetTypeConverter* converter, const te::da::DataSourceInfoPtr& targetDatasource);

          te::da::DataSetTypeConverter* getConverter();


        public slots:

          void onAddToolButtonClicked();

          void onRemoveToolButtonClicked();

          void onEditToolButtonClicked();

          void onUpToolButtonClicked();

          void onDownToolButtonClicked();

          void onCellClicked(int row, int col);

        
        protected:

          void buidTypeMap();

          void fillDataSetTable();

        private:

          std::auto_ptr<Ui::DataSetAdapterWidgetForm> m_ui;
          std::map<int, std::string> m_typeMap;
          te::da::DataSourcePtr m_targetDataSource;
          te::da::DataSetType* m_sourceDataSetType;
          te::da::DataSetTypeConverter* m_dataSetConverter;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_DATASETADAPTERWIDGET_H

