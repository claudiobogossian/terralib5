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
  \file StrToXMLCh.h
   
  \brief A class for converting a standard string to a Xerces string (XMLCh).
*/

#ifndef __TERRALIB_XERCES_INTERNAL_STRTOXMLCH_H
#define __TERRALIB_XERCES_INTERNAL_STRTOXMLCH_H

// STL
#include <cassert>
#include <string>

// Xerces-C++
#include <xercesc/util/XMLString.hpp>

namespace te
{
  namespace xerces
  {
    /*!
      \class StrToXMLCh

      \brief A class for converting a standard string to a Xerces string (XMLCh).

      \sa XMLChToStr
    */
    class StrToXMLCh
    {
      public:

        /*!
          \brief It converts the standard string to an internal XML string.

          \param str A standard string.
        */
        StrToXMLCh(const std::string& str);

        /*! \brief Destructor */
        ~StrToXMLCh();

        /*!
          \brief It return the standard string representation.

          \return The standard string representation.
        */
        const XMLCh* getXMLCh() const;

      private:

        XMLCh* m_xmlch; //!< The internal XML string.
    };

    inline StrToXMLCh::StrToXMLCh(const std::string& str)
      : m_xmlch(0)
    {
      m_xmlch = xercesc::XMLString::transcode(str.c_str());
    }

    inline StrToXMLCh::~StrToXMLCh()
    {
      xercesc::XMLString::release(&m_xmlch);
    }

    inline const XMLCh* StrToXMLCh::getXMLCh() const
    {
      return m_xmlch;
    }

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_STRTOXMLCH_H

