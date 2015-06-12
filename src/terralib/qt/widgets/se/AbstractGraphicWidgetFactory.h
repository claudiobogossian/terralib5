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
  \file terralib/qt/widgets/se/AbstractGraphicWidgetFactory.h

  \brief The abstract factory of widgets used to configure Symbology Enconding Graphic element.
*/

#ifndef __TERRALIB_QT_SE_INTERNAL_ABSTRACTGRAPHICWIDGETFACTORY_H
#define __TERRALIB_QT_SE_INTERNAL_ABSTRACTGRAPHICWIDGETFACTORY_H

// TerraLib
#include "../../../common/AbstractFactory.h"
#include "../Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class AbstractGraphicWidget;

      /*!
        \class AbstractGraphicWidgetFactory

        \brief The abstract factory of widgets used to configure Symbology Enconding Graphic element.

        \sa AbstractFactory
      */
      class TEQTWIDGETSEXPORT AbstractGraphicWidgetFactory : public te::common::AbstractFactory<AbstractGraphicWidget, std::string>
      {
        public:

          /** @name Public Destructor
           *  Destructor.
           */
          //@{

          /*!
            \brief Virtual destructor.

            \note It will automatically unregister the factory from the dictionary.
          */
          virtual ~AbstractGraphicWidgetFactory();

          //@}

        /*!
          \brief It informs the key of each registered graphic widget factory.

          \param keys A pre-created vector of string that will be filled with the keys of each registered graphic widget factory.

          \note All registered factories will be consulted.
        */
        static void RegisteredWidgets(std::vector<std::string>& keys);

        protected:

          /** @name Protected Initializers
           *  Initializers.
           */
          //@{

          /*!
            \brief It creates the factory.

            The key of a AbstractGraphicWidgetFactory is a string.

            \param factoryKey The key that identifies the factory.
          */
          AbstractGraphicWidgetFactory(const std::string& factoryKey);

          //@}
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_SE_INTERNAL_ABSTRACTGRAPHICWIDGETFACTORY_H
