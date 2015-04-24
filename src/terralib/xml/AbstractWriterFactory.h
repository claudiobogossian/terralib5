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
  \file terralib/xml/AbstractWriterFactory.h

  \brief This is the abstract factory for XML writers.
*/

#ifndef __TERRALIB_XML_INTERNAL_ABSTRACTWRITERFACTORY_H
#define __TERRALIB_XML_INTERNAL_ABSTRACTWRITERFACTORY_H

// TerraLib
#include "../common/AbstractFactory.h"
#include "AbstractWriter.h"

// STL
#include <string>

namespace te
{
  namespace xml
  {   

    /*!
      \class AbstractWriterFactory

      \brief This is the abstract factory for XML writers.

      This is an abstract factory, so you will need to call the static method make
      as follow:

      \code
      AbstractWriter* writer = AbstractWriterFactory::make("XERCES");
      \endcode

      or

      \code
      // for a default writer
      AbstractWriter* writer = AbstractWriterFactory::make();
      \endcode

      \ingroup xml

      \sa Writer
     */
    class TEXMLEXPORT AbstractWriterFactory : public te::common::AbstractFactory<te::xml::AbstractWriter, std::string>
    {
      public:

        /*!
          \brief It creates a new XML writer using the dafault implementation.

          \return A new XML writer using the dafault implementation.
         */
        static te::xml::AbstractWriter* make();

        /*!
          \brief It creates a new XML writer.

          \param writerType The type of XML writer to be created.

          \return A new XML writer.
         */
        static te::xml::AbstractWriter* make(const std::string& writerType);

        /*!
          \brief It sets the default driver used to create the XML writer objects.

          \param writerType The default driver name for creating XML writer objects.
         */
        static void setDefaultWriter(const std::string& writerType);

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        virtual ~AbstractWriterFactory() {}

      protected:

        /*!
          \brief It creates the factory.

          The key of a AbstractWriterFactory is a string.

          \param factoryKey The key that identifies the factory.
         */
        AbstractWriterFactory(const std::string& factoryKey);

      private:

        static std::string sm_defaultWriterType;  //!< The type of the writer to be created if no type is informed.
    };    

  } // end namespace xml
}   // end namespace te

#endif  // __TERRALIB_XML_INTERNAL_ABSTRACTWRITERFACTORY_H

