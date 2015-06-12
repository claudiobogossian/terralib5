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
  \file terralib/fe/Filter.h
  
  \brief A Filter is any valid predicate expression.
 */

#ifndef __TERRALIB_FE_INTERNAL_FILTER_H
#define __TERRALIB_FE_INTERNAL_FILTER_H

// TerraLib
#include "ObjectId.h"
#include "AbstractOp.h"

// STL 
#include <vector>

namespace te
{
  namespace fe
  {
    /*!
      \class Filter
      
      \brief A filter is any valid predicate expression.

      A Filter contains a expression which is created
      by combining other expressions.

      \ingroup fe

      \sa AbstractOp, ObjectId
     */
    class TEFEEXPORT Filter
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Filter. */
        Filter();

        /*! \brief Destructor. */
        ~Filter();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the filter main operation (expression).

          \param o The main operation (also called an expression).

          \note The Filter object will take the ownership of the given operation.
         */
        void setOp(AbstractOp* o);

        /*!
          \brief It returns main filter operation.

          \return The main filter operation.
         */
        AbstractOp* getOp() const;

        /*!
          \brief It adds the object identifier to the list of identifiers.
         
          \param id The object identifier to be added to the list of identifiers.

          \note The Filter object will take the ownership of the object identifier. So, you must not free it.
         */
        void add(ObjectId* id);        

        /*!
          \brief It returns a specified object identifier.

          \param i The index of desired object identifier.

          \return A specified object identifier.

          \note You must not clear the returned object identifier.

          \note The method will not check the index range.
         */
        ObjectId* operator[](size_t i) const;

        /*!
          \brief It returns a specified object identifier.

          \param i The index of desired object identifier.

          \return A specified object identifier.

          \note You must not clear the returned object identifier.

          \note The method will not check the index range.
         */
        ObjectId* getOid(size_t i) const;

        /*!
          \brief It returns the size of the list of object identifiers.

          \return The size of the list of object identifiers.
         */
        size_t getNumberOfOids() const;

        /*!
          \brief It clears the object identifier list and drop it off.

          \note After calling this method it will be safe to set an operator.
         */
        void clear();

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other instance.
         */
        Filter(const Filter& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other instance.

          \return A reference for this.
         */
        Filter& operator=(const Filter& rhs);

        //@}

      private:

        AbstractOp* m_op;               //!< May be: SpatialOp, ComparisonOp or LogicOp. (NULL if m_ids is informed)
        std::vector<ObjectId*>* m_ids;  //!< Object identifiers. (NULL if m_op is informed)
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_FILTER_H
