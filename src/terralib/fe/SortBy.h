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
  \file SortBy.h
  
  \brief It represents a sort by expression.
 */

#ifndef __TERRALIB_FE_INTERNAL_SORTBY_H
#define __TERRALIB_FE_INTERNAL_SORTBY_H

// TerraLib
#include "Config.h"

// STL
#include <vector>

namespace te
{
  namespace fe
  {
// Forward declaration
    class SortProperty;

    /*!
      \class SortBy
      
      \brief It represents a sort by expression.

      \sa SortProperty
     */
    class TEFEEXPORT SortBy
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new SortBy. */
        SortBy();

        /*! \brief Destructor. */
        ~SortBy();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the number of sort expressions.

          \return The number of sort expressions.
         */
        std::size_t size() const;

        /*!
          \brief It adds the property to the sort list.
         
          \param p The property to be added to the sort list.

          \note The SortBy object will take the ownership of the property. So, you must not free it.
         */
        void push_back(SortProperty* p);        

        /*!
          \brief It returns a specified property.

          \param index The index of desired property.

          \return A specified property.

          \note You must not clear the returned property.

          \note The method will not check the index range.
         */
        const SortProperty* operator[](std::size_t index) const;

        //@}

      private:

        std::vector<SortProperty*> m_sortPropertyVector;  //!< Mandatory at least one sort property.
    };

  } // end namespace fe
}     // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_SORTBY_H
