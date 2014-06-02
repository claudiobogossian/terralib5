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
  \file terralib/qt/widgets/property/NewPropertyWidget.h

  \brief A class used to define the NewPropertyWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_NEWPROPERTYWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_NEWPROPERTYWIDGET_H

// TerraLib
#include "../Config.h"


// Qt
#include <QWidget>
#include <QGridLayout>

#include <memory>

namespace Ui { class NewPropertyWidgetForm; }

namespace te
{
  namespace dt { class SimpleProperty; }

  namespace qt
  {
    namespace widgets
    {
      class SimplePropertyWidget;

      /*!
        \class NewPropertyDialog

        \brief A dialog for creating a new property
      */
      class TEQTWIDGETSEXPORT NewPropertyWidget : public QWidget
      {
        Q_OBJECT

        public:

          NewPropertyWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~NewPropertyWidget();

        public:

          /*!
            \brief Used to get all data types supported by this data source.

            \param id String used to define the data source id
          */
          void setDataSourceId(std::string id);

          /*!
            \brief It returns a simple property class object.

            \return The caller of this method will TAKE the ownership of the returned SimpleProperty pointer.
          */
          te::dt::SimpleProperty* getProperty();

          bool buildProperty();

        public slots:

          void onDataTypeComboBoxActivated(int index);

        private:

          std::auto_ptr<Ui::NewPropertyWidgetForm> m_ui;
          te::qt::widgets::SimplePropertyWidget* m_spWidget;
          te::dt::SimpleProperty* m_simpleProperty;

          QGridLayout* m_layout;                                      //!< Qt Layout object
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_NEWPROPERTYDIALOG_H

