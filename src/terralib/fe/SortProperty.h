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
  \file SortProperty.h
  
  \brief A sort property name and order type.  
 */

#ifndef __TERRALIB_FE_INTERNAL_SORTPROPERTY_H
#define __TERRALIB_FE_INTERNAL_SORTPROPERTY_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace fe
  {
// Forward declarations
    class PropertyName;

    /*!
      \class SortProperty
      
      \brief A sort property name and order type.

      The order default will be ascendent if none is informed.

      \sa SortBy, PropertyName
     */
    class TEFEEXPORT SortProperty
    {
      public:

        /*!
          \enum SortOrder

          \brief Sort order type: asc or desc.
         */
        enum SortOrder
        {
          DESC, /*!< Descendent order type. */
          ASC   /*!< Ascendent order type.  */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new SortProperty. */
        SortProperty();

        /*! \brief Destructor. */
        ~SortProperty();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the property name.

          \param p The property name. The SortProperty will take the ownership of the property name.

          \note The previous property is discarted.
         */
        void setPropertyName(PropertyName* p);

        /*!
          \brief It returns the property name.

          \return The property name.
         */
        const PropertyName* getPropertyName() const;        

        /*!
          \brief It sets the sort order.

          \param direction Sort order direction (ASC or DESC).
         */
        void setSortOrder(SortOrder direction);

        /*!
          \brief It return the associated sort order.

          \return The associated sort order.
         */
        SortOrder getSortOrder() const;

        //@}

      private:

        PropertyName* m_property;  //!< Mandatory.
        SortOrder m_sortOrder;     //!< Optional.
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_SORTPROPERTY_H
