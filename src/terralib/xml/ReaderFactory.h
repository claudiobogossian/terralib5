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
  \file terralib/xml/ReaderFactory.h

  \brief This is the abstract factory for XML readers.
*/

#ifndef __TERRALIB_XML_INTERNAL_READERFACTORY_H
#define __TERRALIB_XML_INTERNAL_READERFACTORY_H

// TerraLib
#include "../common/AbstractFactory.h"
#include "Reader.h"

namespace te
{
  namespace xml
  {   

    /*!
      \class ReaderFactory

      \brief This is the abstract factory for XML readers.

      This is an abstract factory, so you will need to call the static method make
      as follow:

      \code
      Reader* reader = ReaderFactory::make("XERCES");
      \endcode

      or

      \code
      // for a default reader
      Reader* reader = ReaderFactory::make();
      \endcode

      \ingroup xml

      \sa Reader
     */
    class TEXMLEXPORT ReaderFactory : public te::common::AbstractFactory<te::xml::Reader, std::string>
    {
      public:

        /*!
          \brief It creates a new XML reader using the dafault implementation.

          \return A new XML reader using the dafault implementation.
         */
        static te::xml::Reader* make();

        /*!
          \brief It creates a new XML reader.

          \param readerType The type of XML reader to be created.

          \return A new XML reader.
         */
        static te::xml::Reader* make(const std::string& readerType);

        /*!
          \brief It sets the default driver used to create the XML reader objects.

          \param readerType The default driver name for creating XML reader objects.
         */
        static void setDefaultReader(const std::string& readerType);

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        virtual ~ReaderFactory() {}

      protected:

        /*!
          \brief It creates the factory.

          The key of a ReaderFactory is a string.

          \param factoryKey The key that identifies the factory.
         */
        ReaderFactory(const std::string& factoryKey);

      private:

        static std::string sm_defaultReaderType;  //!< The type of the reader to be created if no type is informed.
    };    

  } // end namespace xml
}   // end namespace te

#endif  // __TERRALIB_XML_INTERNAL_READERFACTORY_H

