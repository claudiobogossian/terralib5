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
  \file terralib/qt/widget/property/SimplePropertyWidgetFactory.h

  \brief A factory to build widget objects.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_SIMPLEPROPERTYWIDGETFACTORY_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_SIMPLEPROPERTYWIDGETFACTORY_H

// TerraLib
#include "../../../common/AbstractFactory.h"
#include "../../../datatype/Enums.h"
#include "../Config.h"
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
        \class SimplePropertyWidgetFactory

        \brief  A factory to build widget objects.
      */
      class TEQTWIDGETSEXPORT SimplePropertyWidgetFactory : public te::common::AbstractFactory<te::qt::widgets::SimplePropertyWidget, int>
      {
        public:

          static te::qt::widgets::SimplePropertyWidget* make(const int& type);
          
          static te::qt::widgets::SimplePropertyWidget* make(const int& type, QWidget* parent = 0);

          /*!
            \brief Virtual destructor.

            \note It will automatically unregister the factory from the dictionary.
          */
          virtual ~SimplePropertyWidgetFactory();

          /*!
            \brief It returns the type of component widget that the factory knows how to create.

            \return The type of component widget that the factory knows how to create.
          */
          virtual int getType() const = 0;

        protected:

          /*!
            \brief It creates the factory.

            The key of a ComponentWidgetFactory is a string.

            \param factoryKey The key that identifies the factory.
          */
          SimplePropertyWidgetFactory(const int& factoryKey);
          
          virtual te::qt::widgets::SimplePropertyWidget* create(QWidget* Parent = 0) = 0;

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_SIMPLEPROPERTYWIDGETFACTORY_H
