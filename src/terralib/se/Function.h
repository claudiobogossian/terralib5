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
  \file terralib/se/Function.h

  \brief Symbology Encoding functions.
*/

#ifndef __TERRALIB_SE_INTERNAL_FUNCTION_H
#define __TERRALIB_SE_INTERNAL_FUNCTION_H

// TerraLib
#include "../fe/Expression.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace se
  {
    /*!
      \class Function
      
      \brief Symbology Encoding functions.

      Symbology Encoding extends the concept of ogc::fe::Expression
      inherited from Filter Encoding to adequately support the needs
      of symbolization in transforming and editing data. It does so
      by introducing a couple of functions (ogc::se::Function) which
      are substitutable for ogc::fe::expression.<br>
      There are two general groups of functions for usage in SE.
      The first group is used to transform raw values into
      "symbolizable" quantities. This especially comprises the
      processes of categorization, recoding, and interpolation.
      This group of functions is especially useful in all places
      using SvgParameters, making them dynamically related to data
      values. The second group defines means for formatting data
      items like numbers, strings, and dates. These functions are
      especially helpful to set up the Label element of TextSymbolizers.
        
      \sa FormatNumber, FormatDate, Substring,
          Concatenate, ChangeCase, Trim,
          StringPosition, StringLength, Categorize,
          Interpolate, Recode
    */
    class TESEEXPORT Function : public te::fe::Expression
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Function. */
        Function();

        /*! \brief Virtual destructor. */
        virtual ~Function();

        //@}

        void setFallbackValue(const std::string& v);

        const std::string& getFallbackValue() const;

      protected:

        std::string m_fallbackValue;  //!< The value of the fallbackValue attribute is used as a default value, if the SE implementation does not support the function. If the implementation supports the function, then the result value is determined by executing the function. (Mandatory)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_FUNCTION_H

