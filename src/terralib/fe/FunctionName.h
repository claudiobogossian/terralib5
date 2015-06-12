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
  \file FunctionName.h
  
  \brief It is used to list the function names that are supported and the number of arguments each function requires.  
 */

#ifndef __TERRALIB_FE_INTERNAL_FUNCTIONNAME_H
#define __TERRALIB_FE_INTERNAL_FUNCTIONNAME_H

// TerraLib
#include "Config.h"

// STL 
#include <string>

namespace te
{
  namespace fe
  {
    /*!
      \class FunctionName
      
      \brief It is used to list the function names that are supported and the number of arguments each function requires.

      \sa ArithmeticOperators
     */
    class TEFEEXPORT FunctionName
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new FunctionName.

          \param name  The function name.
          \param nArgs The number of arguments.
         */
        FunctionName(const char* name, const unsigned int& nArgs);

        /*! \brief Destructor. */
        ~FunctionName();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the function name.

          \return The function name.
         */
        const std::string& getName() const;

        /*!
          \brief It returns the function arguments number.

          \return The function arguments number.
         */
        unsigned int getNumArgs() const;

        //@}

      private:

        std::string m_name;   //!< Function name. (Mandatory)
        unsigned int m_nArgs; //!< Number of arguments. (Mandatory)
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_FUNCTIONNAME_H
