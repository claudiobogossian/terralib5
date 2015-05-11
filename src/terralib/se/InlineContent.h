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
  \file terralib/se/InlineContent.h

  \brief InlineContent is XML or base64 encoded content in some externally-defined format that is included in an SE in-line.
*/

#ifndef __TERRALIB_SE_INTERNAL_INLINECONTENT_H
#define __TERRALIB_SE_INTERNAL_INLINECONTENT_H

// TerraLib
#include "Config.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  {
    /*!
      \class InlineContent
      
      \brief InlineContent is XML- or base64-encoded encoded content in some externally-defined format that is included in an SE in-line.

      \sa Mark, ExternalGraphic

      \todo Esta classe precisa lidar com dados mixed!
    */
    class TESEEXPORT InlineContent : public boost::noncopyable
    {
      public:

        /*!
          \enum EncodingType

          \brief The format type of encoded data.
        */
        enum EncodingType
        {
          XML,    /*!< XML encoded.     */
          BASE64  /*!< Base-64 encoded. */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new InlineContent.

          \param e Encoding type.
        */
        explicit InlineContent(const EncodingType& e);

        /*! \brief Destructor. */
        ~InlineContent();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setData(const std::string& d);

        const std::string& getData() const;

        void setEncoding(EncodingType e);

        EncodingType getEncoding() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        InlineContent* clone() const;

      private:

        std::string m_data;      //!< The inlined data. (Optional)
        EncodingType m_encoding; //!< The possible values are: xml or base64. (Mandatory)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_INLINECONTENT_H

