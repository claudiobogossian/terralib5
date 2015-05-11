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
  \file ReaderFactory.h

  \brief This is the concrete factory for XML reader built on top of Xerces-C++ parser.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_READERFACTORY_H
#define __TERRALIB_XERCES_INTERNAL_READERFACTORY_H

// TerraLib
#include "../xml/ReaderFactory.h"
#include "Config.h"

// STL
#include <memory>

namespace te
{
  namespace xerces
  {
    /*!
      \class ReaderFactory

      \brief This is the concrete factory for XML reader built on top of Xerces-C++ parser.

      \sa te::xml::ReaderFactory
    */
    class TEXERCESEXPORT ReaderFactory : public te::xml::ReaderFactory
    {
      public:

        ~ReaderFactory();

      protected:        

        te::xml::Reader* build();

        ReaderFactory();        

        /*! \brief It initializes the factory: it will be registered in the abstract factory ReaderFactory. */
        static void initialize();

        /*! \brief It finalizes the factory: it will be destroyed and will be unregistered from the abstract factory ReaderFactory. */
        static void finalize();

      private:

        static ReaderFactory* sm_factory; //!< A pointer to the global Xerces XML Reader factory.

      friend class Module;
    };

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_READERFACTORY_H

