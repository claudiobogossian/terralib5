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
  \file AddProperty.h

  \brief It adds a property to a data set.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ADDPROPERTY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ADDPROPERTY_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>
#include <ui_AddProperty.h>

// TerraLib
namespace te
{
  namespace da
  {
    class DataSource;
  }

  namespace dt { class Property; }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT AddProperty : public QDialog, public Ui_AddPropertyDialog
      {
        Q_OBJECT

        public:
          AddProperty(te::da::DataSource* ds, QWidget* parent = 0);

          ~AddProperty();

          te::dt::Property* getProperty() { return m_property; }

        private slots:
          void dataSetComboBoxChanged(const QString& dataSetName);
          void dataTypeComboBoxChanged(const QString& dataType);
          void autoNumberCheckBoxClicked(bool checked);
          void requiredCheckBoxClicked(bool checked);
          void arrayElementDataTypeComboBoxChanged(const QString& arrayElementDataType);
          void stringTypeComboBoxChanged(const QString& stringType);
          void okPushButtonClicked();
          void cancelPushButtonClicked();
          void helpPushButtonClicked();

        protected:
          void setArrayPropertyParams();
          void setDateTimePropertyParams();
          void setGeometryPropertyParams();
          void setStringPropertyParams();

          te::dt::Property* buildProperty(const QString& dataType);
          te::dt::Property* buildArrayProperty();
          te::dt::Property* buildBitProperty();
          te::dt::Property* buildBooleanProperty();
          te::dt::Property* buildByteArrayProperty();
          te::dt::Property* buildCharProperty();
          te::dt::Property* buildCompositeProperty();
          te::dt::Property* buildDateTimeProperty();
          te::dt::Property* buildDoubleProperty();
          te::dt::Property* buildFloatProperty();
          te::dt::Property* buildGeometryProperty();
          te::dt::Property* buildInt16Property();
          te::dt::Property* buildInt32Property();
          te::dt::Property* buildInt64Property();
          te::dt::Property* buildNumericProperty();
          te::dt::Property* buildRasterProperty();
          te::dt::Property* buildStringProperty();
          te::dt::Property* buildUcharProperty();
          te::dt::Property* buildUint16Property();
          te::dt::Property* buildUint32Property();
          te::dt::Property* buildUint64Property();
          te::dt::Property* buildUnknownProperty();
          te::dt::Property* buildVoidProperty();

          void closeEvent(QCloseEvent* e);

        private:
          te::da::DataSource* m_ds;
          te::dt::Property* m_property;             // the property to be built
          std::string m_propertyName;               // the property name
          std::string* m_defaultValue;              // the default value for the property
      };
    }
  }
}

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ADDPROPERTY_H
