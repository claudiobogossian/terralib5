/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/property/SimplePropertyWidget.h

  \brief A base widget for simple properties.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_SIMPLEPROPERTYWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_SIMPLEPROPERTYWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// QT
#include <QWidget>

namespace te
{
  namespace dt { class SimpleProperty; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class SimplePropertyWidget

        \brief A base widget for simple properties.
      */
      class TEQTWIDGETSEXPORT SimplePropertyWidget : public QWidget
      {
        public:

          SimplePropertyWidget(QWidget* parent = 0);

          virtual ~SimplePropertyWidget();

        public:

          virtual void fillProperty(te::dt::SimpleProperty* sp) = 0;

          virtual bool hasAttributeRequired();

          virtual bool hasAttributeAutoNumber();

          virtual bool hasAttributeDefaultValue();
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_SIMPLEPROPERTYWIDGET_H
