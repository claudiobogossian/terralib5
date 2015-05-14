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
  \file terralib/dataaccess/query/PropertyName.h

  \brief A class that models the name of any property of an object.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_PROPERTYNAME_H
#define __TERRALIB_DATAACCESS_INTERNAL_PROPERTYNAME_H

// TerraLib
#include "Expression.h"

// STL
#include <string>

namespace te
{
  namespace da
  {
    /*!
      \class PropertyName
      
      \brief A class that models the name of any property of an object.

      The property name can be used in scalar or spatial expressions to represent
      the value of that property
      for a particualr instance of an object.

      \sa Expression
    */
    class TEDATAACCESSEXPORT PropertyName : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param name The property name.
        */
        PropertyName(const std::string& name) : m_name(name) {}

        /*! \brief Copy constructor. */
        PropertyName(const PropertyName& rhs);

        /*! \brief Destructor. */
        ~PropertyName() {}

        /*! Assignment operator.  */
        PropertyName& operator=(const PropertyName& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

        /*!
          \brief It returns the property name.

          \return The property name.
         */
        const std::string& getName() const { return m_name; }

        /*!
          \brief It sets the property name.

          \param name The property name.
         */
        void setName(const std::string& name) { m_name = name; }

      private:

        std::string m_name;  //!< The property name.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_PROPERTYNAME_H

