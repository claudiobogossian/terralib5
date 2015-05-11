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
  \file terralib/qt/widgets/property/PropertyConverterDialog.h

  \brief A class used to define the PropertyConverterDialog class.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_PROPERTYCONVERTERDIALOG_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_PROPERTYCONVERTERDIALOG_H

// TerraLib
#include "../../../dataaccess/dataset/AttributeConverters.h"
#include "../Config.h"

// Qt
#include <QDialog>
#include <QGridLayout>

//STL
#include <map>

namespace Ui { class PropertyConverterDialogForm; }

namespace te
{
  namespace da 
  { 
    class DataSetTypeConverter;
    class DataSetType; 
  }

  namespace qt
  {
    namespace widgets
    {
      class NewPropertyWidget;

      /*!
        \class PropertyConverterDialog

        \brief A dialog for creating a adaptor property converter
      */
      class TEQTWIDGETSEXPORT PropertyConverterDialog : public QDialog
      {
        Q_OBJECT

        public:

          PropertyConverterDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~PropertyConverterDialog();

        public:

          void set(te::da::DataSetType* dsType);

          void set(std::string dataSourceId);

          void adapt(te::da::DataSetTypeConverter* converter);

        public slots:

          void onOkPushButtonClicked();

        protected:

          void buidTypeMap();

        private:

          std::auto_ptr<Ui::PropertyConverterDialogForm> m_ui;
          std::map<int, std::string> m_typeMap;
          te::qt::widgets::NewPropertyWidget* m_propWidget;
          te::da::DataSetType* m_dsType;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_PROPERTYCONVERTERDIALOG_H

