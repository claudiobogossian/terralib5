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
  \file terralib/dataaccess/query/FunctionDefn.h

  \brief The definition of a function that can be used in a query object.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONDEFN_H
#define __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONDEFN_H

// TerraLib
#include "../../datatype/Enums.h"
#include "../Config.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace da
  {
// Forward declarations
    class FunctionParameter;

    /*!
      \class FunctionDefn
      
      \brief The definition of a function that can be used in a query object.

      All functions must have a name in lower case letters.

      \sa FunctionCatalog, FunctionCatalogManager, FunctionParameter
    */
    class TEDATAACCESSEXPORT FunctionDefn : public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.
          
          \param name        The function name.
          \param retValType  The return value type (void by default).
          \param isAggregate Tells if function is an aggregate function (false by default).
          \param description A brief description about the function (NULL by default). The FunctionDefn will take the ownership of the given description.
        */
        FunctionDefn(const std::string& name,
                     int retValType = te::dt::VOID_TYPE,
                     bool isAggregate = false,
                     std::string* description = 0);

        /*! \brief Destructor. */
        ~FunctionDefn();

        /*!
          \brief It returns the function name.

          \return The function name.
        */
        const std::string& getName() const { return m_name; }

        /*!
          \brief It sets the function name;

          \param name The function name.

          \note If the function belongs to a FunctionCatalog, don't call this method!
        */
        void setName(const std::string& name) { m_name = name; }

        /*!
          \brief It returns the data type of the return value.

          \return The data type of the return value.
        */
        int getRetValType() const { return m_retValType; }

        /*!
          \brief It sets the data type of the return value.

          \param r The data type of the return value.
        */
        void setRetValType(int r) { m_retValType = r; }

        /*!
          \brief It returns true if it is an aggregate function, otherwise, returns false.

          \return True if it is an aggregate function, otherwise, returns false.
        */
        bool isAggregate() const { return m_isAggregate; }

        /*!
          \brief It tells if the function is an aggregate function or not.

          \param a If true the function is an aggeragte function otherwise it is not.
        */
        void SetIsAggregate(bool a) { m_isAggregate = a; }

        /*!
          \brief It returns a brief description about the function.

          \return A brief description about the function or NULL if none is provide.
        */
        std::string* getDescription() const { return m_description; };

        /*!
          \brief It sets a brief description about the function.

          \param description A brief description about the function. The FunctionDefn will take the ownership of the given description.
        */
        void setDescription(std::string* description);

        /*!
          \brief It returns the number of parameters for the function.

          \return The number of parameters.
        */
        std::size_t getNumParams() const { return m_params.size(); }

        /*!
          \brief It returns the i-th function parameter.

          \param i The parameter position.

          \return The i-th function parameter.

          \note It will not check the index range.
        */
        FunctionParameter* operator[](std::size_t i) const { return m_params[i]; }

        /*!
          \brief It adds the parameter to the function list of parameters.

          \param p The parameter to be added.

          \note The FunctionDefn will take the ownership of the given parameter.
        */
        void add(FunctionParameter* p) { m_params.push_back(p); }

      protected:

        std::string m_name;                           //!< The function name. It may be an operator symbol or just a regular name like st_intersects.        
        std::vector<FunctionParameter*> m_params;     //!< The list of arguments.
        int m_retValType;                             //!< Data type of the return value.
        bool m_isAggregate;                           //!< A flag that defines if the function is an aggregate one.        
        std::string* m_description;                   //!< A brief description of the function.
        //int m_cost;                                 //!< Estimated execution cost.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONDEFN_H

