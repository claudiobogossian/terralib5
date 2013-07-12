/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file AddIndex.h

  \brief It adds an index associated to a DataSetType.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ADDINDEX_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ADDINDEX_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>
#include <ui_AddIndex.h>

// TerraLib
namespace te
{
  namespace da
  {
    class DataSetType;
    class DataSource;
    class DataSourceCatalogLoader;
    class DataSourceTransactor;
    class Index;
  }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT AddIndex : public QDialog, public Ui_AddIndexDialog
      {
        Q_OBJECT

        public:
          AddIndex(te::da::DataSource* ds, QWidget* parent = 0);

          ~AddIndex();

          te::da::DataSetType* getDataSetType() const  { return m_dataSetType; }
          te::da::Index* getIndex() const { return m_index; }

        private slots:
          void dataSetComboBoxChanged(const QString& dataSet);
          void addToIndexPropertiesPushButtonClicked();
          void removeFromIndexPropertiesPushButtonClicked();
          void okPushButtonClicked();
          void cancelPushButtonClicked();
          void helpPushButtonClicked();

        protected:
          void closeEvent(QCloseEvent* e); 
      
        private:
          te::da::DataSource* m_ds;
          te::da::DataSourceTransactor* m_transactor;
          te::da::DataSourceCatalogLoader* m_catalogLoader;

          te::da::DataSetType* m_dataSetType;
          te::da::Index* m_index;
      };
    }
  }
}

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ADDINDEX_H

  
