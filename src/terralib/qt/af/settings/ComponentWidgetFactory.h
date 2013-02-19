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
  \file terralib/qt/af/settings/ComponentWidgetFactory.h

  \brief A factory to build widget objects.
*/

#ifndef __TERRALIB_QT_AF_COMPONENTWIDGETFACTORY_H
#define __TERRALIB_QT_AF_COMPONENTWIDGETFACTORY_H

// QT
#include <QtGui/QWidget>

// TerraLib
#include "../../../common/AbstractFactory.h"
#include "../Config.h"
#include "ComponentWidget.h"

namespace te
{
  namespace qt
  {   
    namespace af
    {
      /*!
        \class ComponentWidgetFactory

        \brief  A factory to build widget objects.
      */
      class TEQTAFEXPORT ComponentWidgetFactory : public te::common::AbstractFactory<te::qt::af::ComponentWidget, std::string>
      {
        public:

          static te::qt::af::ComponentWidget* make(const std::string& cwType);
          
          static te::qt::af::ComponentWidget* make(const std::string& cwType, QWidget* parent = 0);

          /*!
            \brief Virtual destructor.

            \note It will automatically unregister the factory from the dictionary.
          */
          virtual ~ComponentWidgetFactory();

          /*!
            \brief It returns the type of component widget that the factory knows how to create.

            \return The type of component widget that the factory knows how to create.
          */
          virtual const std::string& getType() const = 0;

        protected:

          /*!
            \brief It creates the factory.

            The key of a ComponentWidgetFactory is a string.

            \param factoryKey The key that identifies the factory.
          */
          ComponentWidgetFactory(const std::string& factoryKey);
          
          virtual te::qt::af::ComponentWidget* create(QWidget* Parent = 0) = 0;

      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_COMPONENTWIDGETFACTORY_H