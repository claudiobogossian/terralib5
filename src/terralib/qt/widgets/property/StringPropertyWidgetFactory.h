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
  \file terralib/qt/widget/property/StringPropertyWidgetFactory.h

  \brief Defines the class StringPropertyWidgetFactory.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_STRINGPROPERTYWIDGETFACTORY_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_STRINGPROPERTYWIDGETFACTORY_H

// TerraLib
#include "../../../common/AbstractFactory.h"
#include "../Config.h"
#include "SimplePropertyWidgetFactory.h"
#include "SimplePropertyWidget.h"


// QT
#include <QWidget>

namespace te
{
  namespace qt
  {   
    namespace widgets
    {
      /*!
        \class StringPropertyWidgetFactory

        \brief A factory to build the string property widget object.
      */
      class TEQTWIDGETSEXPORT StringPropertyWidgetFactory : public te::qt::widgets::SimplePropertyWidgetFactory
      {
        public:

          static void initialize();

          static void finalize();

          int getType() const;

          ~StringPropertyWidgetFactory();

        protected:

          te::qt::widgets::SimplePropertyWidget* build();

          StringPropertyWidgetFactory();

          virtual te::qt::widgets::SimplePropertyWidget* create(QWidget* parent = 0);

        private:

          static StringPropertyWidgetFactory* sm_factory;

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_STRINGPROPERTYWIDGETFACTORY_H