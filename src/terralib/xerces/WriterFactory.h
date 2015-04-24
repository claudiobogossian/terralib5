/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file WriterFactory.h

  \brief This is the concrete factory for XML writer built on top of Xerces-C++ parser.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_WRITERFACTORY_H
#define __TERRALIB_XERCES_INTERNAL_WRITERFACTORY_H

// TerraLib
#include "../xml/AbstractWriterFactory.h"
#include "Config.h"

// STL
#include <memory>

namespace te
{
  namespace xerces
  {
    /*!
      \class WriterFactory

      \brief This is the concrete factory for XML writer built on top of Xerces-C++ parser.

      \sa te::xml::AbstractWriter, te::xml::AbstractWriterFactory
    */
    class TEXERCESEXPORT WriterFactory : public te::xml::AbstractWriterFactory
    {
      public:

        ~WriterFactory();

      protected:        

        te::xml::AbstractWriter* build();

        WriterFactory();

        /*! \brief It initializes the factory: it will be registered in the abstract factory WriterFactory. */
        static void initialize();

        /*! \brief It finalizes the factory: it will be destroyed and will be unregistered from the abstract factory WriterFactory. */
        static void finalize();

      private:

        static WriterFactory* sm_factory; //!< A pointer to the global Xerces XML Writer factory.

      friend class Module;
    };

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_WRITERFACTORY_H

