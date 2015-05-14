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
  \file terralib/se/StringPosition.h

  \brief It returns the position of first occurence of a substring (String formatting functions).
*/

#ifndef __TERRALIB_SE_INTERNAL_STRINGPOSITION_H
#define __TERRALIB_SE_INTERNAL_STRINGPOSITION_H

// TerraLib
#include "Function.h"

namespace te
{
// Forward declaration
  class ParameterValue;

  namespace se
  {
    /*!
      \class StringPosition
      
      \brief It returns the position of first occurence of a substring (String formatting functions).

      This function returns the position of the first
      occurrence (counting from 1) of the LookupString
      in StringValue. Zero is returned in case of
      search failure. The direction of search is determined
      by the attribute searchdirection, which can take
      values "frontToBack" and "backToFront", where
      the former is the default.

      \sa Function, ParameterValue, StringLength, Trim
    */
    class TESEEXPORT StringPosition : public Function
    {
      public:

        /*!
          \enum SearchDirectionType

          \brief It controls the direction of search.
        */
        enum SearchDirectionType
        {
          FRONTTOBACK,  /*!< It means to search from front to back. */
          BACKTOFRONT   /*!< It means to search from back to front. */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new StringPosition. */
        StringPosition();

        /*! \brief Destructor. */
        ~StringPosition();

        //@}

        void setLookupString(ParameterValue* v);
        void setStringValue(ParameterValue* v);
        void setSearchDirection(SearchDirectionType t);

      private:

        ParameterValue* m_lookupString;        //!< Mandatory.
        ParameterValue* m_stringValue;         //!< Mandatory. 
        SearchDirectionType m_searchDirection; //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_STRINGPOSITION_H
