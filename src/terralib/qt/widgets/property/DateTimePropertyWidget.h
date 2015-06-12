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
  \file terralib/qt/widgets/property/DateTimePropertyWidget.h

  \brief Defines the DateTimePropertyWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_DATETIMEPROPERTYWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_DATETIMEPROPERTYWIDGET_H

// TerraLib
#include "../Config.h"
#include "SimplePropertyWidget.h"

#include <memory>

namespace Ui { class DateTimePropertyWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DateTimePropertyWidget

        \brief A widget used to define a string property.
      */
      class TEQTWIDGETSEXPORT DateTimePropertyWidget : public SimplePropertyWidget
      {
        Q_OBJECT

        public:

          DateTimePropertyWidget(QWidget* parent = 0);

          ~DateTimePropertyWidget();

        public:

          virtual void fillProperty(te::dt::SimpleProperty* sp);

        private:

          std::auto_ptr<Ui::DateTimePropertyWidgetForm> m_ui;
      };
    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_DATETIMEPROPERTYWIDGET_H

