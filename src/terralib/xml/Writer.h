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
  \file terralib/xml/Writer.h
   
  \brief This class models a XML writer object.
*/

#ifndef __TERRALIB_XML_INTERNAL_WRITER_H
#define __TERRALIB_XML_INTERNAL_WRITER_H

// TerraLib
#include "../common/Holder.h"
#include "Config.h"

// STL
#include <iosfwd>
#include <string>

// Boost
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>

namespace te
{
  namespace xml
  {
    /*!
      \class Writer

      \brief This class models a XML writer object.

      \ingroup xml
    */
    class TEXMLEXPORT Writer : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        Writer(std::ostream& ostr);

        /*! \brief Virtual destructor. */
        virtual ~Writer();

        virtual void writeStartDocument(const std::string& encoding, const std::string& standalone);

        virtual void writeStartElement(const std::string& qName);

        virtual void writeElement(const std::string& qName, const std::string& value);

        virtual void writeElement(const std::string& qName, const double& value);

        virtual void writeElement(const std::string& qName, boost::int32_t value);

        virtual void writeElement(const std::string& qName, boost::uint32_t value);

        virtual void writeElement(const std::string& qName, boost::int64_t value);

        virtual void writeElement(const std::string& qName, boost::uint64_t value);

        virtual void writeAttribute(const std::string& attName, const std::string& value);

        virtual void writeAttribute(const std::string& attName, const double& value);

        virtual void writeAttribute(const std::string& attName, boost::int32_t value);

        virtual void writeAttribute(const std::string& attName, boost::uint32_t value);

        virtual void writeAttribute(const std::string& attName, boost::int64_t value);

        virtual void writeAttribute(const std::string& attName, boost::uint64_t value);

        virtual void writeValue(const std::string& value);

        virtual void writeValue(const double& value);

        virtual void writeValue(boost::int32_t value);

        virtual void writeValue(boost::uint32_t value);

        virtual void writeValue(boost::int64_t value);

        virtual void writeValue(boost::uint64_t value);

        virtual void writeEndElement(const std::string& qName);

        /*virtual void writeEndDocument(std::ostream& ostr);*/

      private:

        std::ostream& m_ostr;
        bool m_isOpened;
    };

  } // end namespace xml
}   // end namespace te

#endif  // __TERRALIB_XML_INTERNAL_WRITER_H

