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
  \file terralib/qt/widgets/se/GraphicFillWidgetFactory.h

  \brief A concrete factory that builds a fill widget used to configure a fill element associated with a graphic.
*/

#ifndef __TERRALIB_QT_SE_INTERNAL_GRAPHICFILLWIDGETFACTORY_H
#define __TERRALIB_QT_SE_INTERNAL_GRAPHICFILLWIDGETFACTORY_H

// TerraLib
#include "../Config.h"
#include "AbstractFillWidgetFactory.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class GraphicFillWidgetFactory

        \brief A concrete factory that builds a fill widget used to configure a fill element associated with a graphic.

        \sa AbstractFillWidgetFactory
      */
      class TEQTWIDGETSEXPORT GraphicFillWidgetFactory : public AbstractFillWidgetFactory
      {
        public:

          /** @name Public Initializer
            *  Methods for the control of the factory singleton.
            */
          //@{

          /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory AbstractFillWidgetFactory. */
          static void initialize();

          /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory AbstractFillWidgetFactory. */
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
          ~GraphicFillWidgetFactory();

          //@} 

        protected:

          /** @name Re-implementation of Pure Virtual Method
          *  Concrete factory methods.
          */
          //@{

          AbstractFillWidget* build();

          //@}

        protected:

          /** @name Protected Initializers
          *  Initializers.
          */
          //@{

          /*!
            \brief It creates the factory.

            The key of a GraphicFillWidgetFactory is a string.

            \param factoryKey The key that identifies the factory.
          */
          GraphicFillWidgetFactory();

          //@}

        private:

          static std::string sm_factoryKey;            //!< The factory key.
          static GraphicFillWidgetFactory* sm_factory; //!< A pointer to the global factory.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_SE_INTERNAL_GRAPHICFILLWIDGETFACTORY_H
