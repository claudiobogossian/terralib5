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
  \file RemoveProperty.h

  \brief It removes a property from a data set.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_REMOVEPROPERTY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_REMOVEPROPERTY_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>
#include <ui_RemoveProperty.h>

// TerraLib
namespace te
{
  namespace da
  {
    class DataSetType;
    class DataSource;
    class DataSourceTransactor;
    class DataSourceCatalogLoader;
  }

  namespace dt { class Property; }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT RemoveProperty : public QDialog, public Ui_RemovePropertyDialog
      {
        Q_OBJECT

        public:
          RemoveProperty(te::da::DataSource* ds, QWidget* parent = 0);

          ~RemoveProperty();

          te::dt::Property* getProperty() const  { return m_property; }
          te::da::DataSetType* getPropertyParent() const { return m_propertyParent; }

        private slots:
          void dataSetComboBoxChanged(const QString& dataSet); 
          void okPushButtonClicked();
          void cancelPushButtonClicked();
          void helpPushButtonClicked();

        protected:
          void closeEvent(QCloseEvent* e); 
      
        private:
          te::da::DataSource* m_ds;
          te::da::DataSourceTransactor* m_transactor;
          te::da::DataSourceCatalogLoader* m_catalogLoader;

          te::dt::Property* m_property;
          te::da::DataSetType* m_propertyParent;
      };
    }
  }
}

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_REMOVEPROPERTY_H

  
