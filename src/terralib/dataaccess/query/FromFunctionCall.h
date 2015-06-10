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
  \file terralib/dataaccess/query/FromFunctionCall.h

  \brief A function can be used as a source of information in another query.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FROMFUNCTIONCALL_H
#define __TERRALIB_DATAACCESS_INTERNAL_FROMFUNCTIONCALL_H

// TerraLib
#include "FromItem.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
// Forward declarations
    class Function;

    /*!
      \class FromFunctionCall
      
      \brief A Select can be used as a source of information in another query.

      \sa FromItem, DataSetName, DataSetType, DataSet, FromFunctionCall, Join 
    */
    class TEDATAACCESSEXPORT FromFunctionCall : public FromItem
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param f The function to be associated to this object.

          \note The FromFunctionCall will take the ownership of the function.
        */
        FromFunctionCall(Function* f, const std::string& alias);

        /*!
          \brief Constructor.

          \param f The function to be associated to this object.
        */
        FromFunctionCall(const Function& f, const std::string& alias);

        /*! \brief Copy constructor. */
        FromFunctionCall(const FromFunctionCall& rhs);

        /*! \brief Destructor. */
        ~FromFunctionCall();

        /*! Assignment operator.  */
        FromFunctionCall& operator=(const FromFunctionCall& rhs);

        /*! \brief It creates a new copy of this FromItem. */
        FromItem* clone() const;

        /*!
          \brief It returns the associated function.

          \return The associated function.
        */
        Function* getFunction() const;

        /*!
          \brief It sets the function associated to this object.

          \param f The function to be associated to the FromItem object.
        */
        void setFunction(Function* f);

      private:

        std::auto_ptr<Function> m_f;  //!< The associated function.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FROMFUNCTIONCALL_H

