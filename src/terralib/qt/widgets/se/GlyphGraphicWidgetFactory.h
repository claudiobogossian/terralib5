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
  \file terralib/qt/widgets/se/GlyphGraphicWidgetFactory.h

  \brief A concrete factory that builds a graphic widget used to configure a graphic associate with a glyph mark element.
*/

#ifndef __TERRALIB_QT_SE_INTERNAL_GLYPHGRAPHICWIDGETFACTORY_H
#define __TERRALIB_QT_SE_INTERNAL_GLYPHGRAPHICWIDGETFACTORY_H

// TerraLib
#include "../Config.h"
#include "AbstractGraphicWidgetFactory.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class GlyphGraphicWidgetFactory

        \brief A concrete factory that builds a graphic widget used to configure a graphic associate with a glyph mark element.

        \sa AbstractGraphicWidgetFactory
      */
      class TEQTWIDGETSEXPORT GlyphGraphicWidgetFactory : public AbstractGraphicWidgetFactory
      {
        public:

          /** @name Public Initializer
            *  Methods for the control of the factory singleton.
            */
          //@{

          /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory AbstractGraphicWidgetFactory. */
          static void initialize();

          /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory AbstractGraphicWidgetFactory. */
          static void finalize();

          //@}

          /** @name Public Destructor
            *  Destructor.
            */
          //@{

          /*!
            \brief Destructor.

            \note It will automatically unregister the factory from the dictionary.
          */
          ~GlyphGraphicWidgetFactory();

          //@} 

        protected:

          /** @name Re-implementation of Pure Virtual Method
          *  Concrete factory methods.
          */
          //@{

          AbstractGraphicWidget* build();

          //@}

        protected:

          /** @name Protected Initializers
          *  Initializers.
          */
          //@{

          /*!
            \brief It creates the factory.

            The key of a GlyphGraphicWidgetFactory is a string.

            \param factoryKey The key that identifies the factory.
          */
          GlyphGraphicWidgetFactory();

          //@}

        private:

          static std::string sm_factoryKey;                 //!< The factory key.
          static GlyphGraphicWidgetFactory* sm_factory;     //!< A pointer to the global factory.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_SE_INTERNAL_GLYPHGRAPHICWIDGETFACTORY_H
