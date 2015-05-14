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
  \file XMLChToStr.h
   
  \brief A class for converting a Xerces string (XMLCh) to a standard string.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_XMLCHTOSTR_H
#define __TERRALIB_XERCES_INTERNAL_XMLCHTOSTR_H

// TerraLib
#include "Config.h"

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
      \class XMLChToStr

      \brief A class for converting a Xerces string (XMLCh) to a standard string.

      \sa StrToXMLCh
    */
    class XMLChToStr
    {
      public:

        /*!
          \brief It converts the XML string to an internal standard string.

          \param xstr A XML string.
        */
        XMLChToStr(const XMLCh* const xstr);

        /*! \brief Destructor */
        ~XMLChToStr();

        /*!
          \brief It return the standard string representation.

          \return The standard string representation.
        */
        const std::string& getStr() const;

      private:

        std::string m_stdstr; //!< The internal standard string.
    };

    inline XMLChToStr::XMLChToStr(const XMLCh* const xstr)
    {
      if(xstr == 0)
        return;

      m_stdstr = xercesc::XMLString::transcode(xstr);
    }

    inline XMLChToStr::~XMLChToStr()
    {
    }

    inline const std::string& XMLChToStr::getStr() const
    {
      return m_stdstr;
    }

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_XMLCHTOSTR_H

