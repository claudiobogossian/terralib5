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
  \file terralib/dataaccess/query/FunctionParameter.h

  \brief A FunctionParameter models the parameter of a function definition.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONPARAMETER_H
#define __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONPARAMETER_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace da
  {
    /*!
      \class FunctionParameter
      
      \brief A FunctionParameter models the parameter of a function definition.

      \sa FunctionDefn
    */
    class TEDATAACCESSEXPORT FunctionParameter
    {
      public:

        /*!
          \brief Constructor.

          \param name The parameter name.
          \param t    The parameter type.
        */
        FunctionParameter(const std::string& name, int t) : m_name(name), m_type(t) {}

        /*! \brief Destructor. */
        ~FunctionParameter() {}

        /*!
          \brief It returns the function name.

          \return The function name.
        */
        const std::string& getName() const { return m_name; }

        /*!
          \brief It sets the function name.

          \param name The function name.
        */
        void setName(const std::string& name) { m_name = name; }

        /*!
          \brief It returns the parameter type.

          \return The parameter type.
        */
        int getType() const { return m_type; }

        /*!
          \brief It sets the parameter type.

          \param t The parameter type.
        */
        void setType(int t) { m_type = t; }

      private:

        std::string m_name;   //!< Parameter name.
        int m_type;           //!< Parameter type.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONPARAMETER_H


