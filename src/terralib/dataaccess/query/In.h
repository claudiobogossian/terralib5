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
  \file terralib/dataaccess/query/In.h

  \brief A class that represents the IN operator.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_IN_H
#define __TERRALIB_DATAACCESS_INTERNAL_IN_H

// TerraLib
#include "Function.h"

// STL
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace da
  {
// Forward declarations
    class PropertyName;
    class Literal;

    /*!
      \class In
      
      \brief A class that represents the IN operator.

      \sa Expression
    */
    class TEDATAACCESSEXPORT In : public Function
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param name The property name.

          \note The In will take the ownership of the given property.
        */
        In(PropertyName* p);

        /*!
          \brief Constructor.

          \param name The property name.
        */
        In(const PropertyName& p);

        /*!
          \brief Constructor.

          \param name The property name.
        */
        In(const std::string& name);

        /*! \brief Copy constructor. */
        In(const In& rhs);

        /*! \brief Destructor. */
        ~In();

        /*! Assignment operator.  */
        In& operator=(const In& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

        /*!
          \brief It returns the property name.

          \return The property name.
        */
        PropertyName* getPropertyName() const;

        /*!
          \brief It sets the property name.

          \param name The property name.
        */
        void setPropertyName(PropertyName* p);

      private:

        std::auto_ptr<PropertyName> m_name; //!< The property name.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_IN_H

