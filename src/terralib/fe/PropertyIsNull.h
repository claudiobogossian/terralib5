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
  \file PropertyIsNull.h
  
  \brief The PropertyIsNull class encodes an operator that checks to see if the value of its content is NULL.
 */

#ifndef __TERRALIB_FE_INTERNAL_PROPERTYISNULL_H
#define __TERRALIB_FE_INTERNAL_PROPERTYISNULL_H

// TerraLib
#include "ComparisonOp.h"

namespace te
{
  namespace fe
  {
// Forward declarations
    class PropertyName;

    /*!
      \class PropertyIsNull
      
      \brief The PropertyIsNull class encodes an operator that checks to see if the value of its content is NULL.

      A NULL is equivalent to no value present. The value 0 is
      a valid value and is not considered NULL.

      \ingroup fe

      \sa ComparisonOp, PropertyName
     */
    class TEFEEXPORT PropertyIsNull : public ComparisonOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new PropertyIsNull. */
        PropertyIsNull();

        /*! \brief Destructor. */
        ~PropertyIsNull();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the property name.

          \param p The property name.

          \note It will take the ownership of the property name.
         */
        void setPropertyName(PropertyName* p);

        /*!
          \brief It returns the property name.

          \return The property name.
         */
        PropertyName* getPropertyName() const;

        //@}

      private:

        PropertyName* m_property;  //!< Mandatory.
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_PROPERTYISNULL_H
