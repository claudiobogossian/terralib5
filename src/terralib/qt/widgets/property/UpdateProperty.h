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
  \file UpdateProperty.h

  \brief It updates a property of a data set.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_UPDATEPROPERTY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_UPDATEPROPERTY_H

// TerraLib
#include "../Config.h"
#include "terralib/geometry/Enums.h"

// Qt
#include <QtGui/QDialog>
#include <ui_UpdateProperty.h>

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
      class TEQTWIDGETSEXPORT UpdateProperty : public QDialog, public Ui_UpdatePropertyDialog
      {
        Q_OBJECT

        public:
          UpdateProperty(te::da::DataSource* ds, QWidget* parent = 0);

          ~UpdateProperty();

          te::dt::Property* getOldProperty() const      { return m_oldProperty; }
          te::dt::Property* getUpdatedProperty() const  { return m_updatedProperty; }

        private slots:
          void dataSetComboBoxChanged(const QString& dataSet);
          void propertiesComboBoxActivated(const QString& p);
          void dataTypeComboBoxChanged(const QString& dataType);
          void requiredCheckBoxClicked(bool checked);
          void arrayElementDataTypeComboBoxChanged(const QString& arrayElementDataType);
          void okPushButtonClicked();
          void cancelPushButtonClicked();
          void helpPushButtonClicked();

        protected:
          void setArrayPropertyData();
          void setCompositePropertyData();
          void setDateTimePropertyData();
          void setGeometryPropertyData();
          void setNumericPropertyData();
          void setRasterPropertyData();
          void setStringPropertyData();

          te::dt::Property* buildUpdatedProperty(const QString& dataType);

          te::dt::Property* buildUpdatedArrayProperty();
          te::dt::Property* buildUpdatedBitProperty();
          te::dt::Property* buildUpdatedBooleanProperty();
          te::dt::Property* buildUpdatedByteArrayProperty();
          te::dt::Property* buildUpdatedCharProperty();
          te::dt::Property* buildUpdatedCompositeProperty();
          te::dt::Property* buildUpdatedDateTimeProperty();
          te::dt::Property* buildUpdatedDoubleProperty();
          te::dt::Property* buildUpdatedFloatProperty();
          te::dt::Property* buildUpdatedGeometryProperty();
          te::dt::Property* buildUpdatedInt16Property();
          te::dt::Property* buildUpdatedInt32Property();
          te::dt::Property* buildUpdatedInt64Property();
          te::dt::Property* buildUpdatedNumericProperty();
          te::dt::Property* buildUpdatedRasterProperty();
          te::dt::Property* buildUpdatedStringProperty();
          te::dt::Property* buildUpdatedUcharProperty();
          te::dt::Property* buildUpdatedUint16Property();
          te::dt::Property* buildUpdatedUint32Property();
          te::dt::Property* buildUpdatedUint64Property();
          te::dt::Property* buildUpdatedUnknownProperty();
          te::dt::Property* buildUpdatedVoidProperty();

          void closeEvent(QCloseEvent* e);

        private:
          te::da::DataSource* m_ds;

          te::dt::Property* m_oldProperty;          // The property to be updated
          te::dt::Property* m_updatedProperty;      // The updated property

          std::string  m_propertyName;              // The property name
          std::string* m_defaultValue;              // The property default value

          std::map<int, QString> m_propertyTypeMap; // map of property type to property type text
          std::map<te::gm::GeomType, QString> m_geomTypeTextMap; // map of geometry type to geometry type text
      };
    }
  }
}

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_UPDATEPROPERTY_H
