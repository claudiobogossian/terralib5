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
  \file terralib/datatype/Array.h

  \brief Class for dealing with arrays of abstract data.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_ARRAY_H
#define __TERRALIB_DATATYPE_INTERNAL_ARRAY_H

// TerraLib
#include "AbstractData.h"
#include "Enums.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace dt
  {
    /*!
      \class Array

      \brief The type for variable-length multidimensional arrays.

      \note This class was not designed to achieve performance, 
            since it maps a vector of positions to one abstract data. This
            class is only a container to be used by database drivers. If
            you need a high performance array class with matrix operations,
            see the Matrix module. To access the data, you must define a vector
            of positions, where each dimension in the vector corresponds to the 
            respective dimension in the array.

      \ingroup datatype

      \sa ArrayProperty, AbstractData
    */
    class TEDATATYPEEXPORT Array : public AbstractData
    {
      public:
        /*!
          \brief Multi-dimensional array constructor.

          \param d   The number of dimensions of the array.
          \param t   The data type.
        */
        Array(std::size_t d, int t);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side instance.
        */
        Array(const Array& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side instance.

          \return A reference to this array.
        */
        Array& operator=(const Array& rhs);

        /*! \brief Destructor. */
        ~Array();

        /*! \brief Returns the numbe of dimensions of the array. */
        std::size_t getDimension() const;

        /*!
          \brief Returns the data type of the elements of the array. 

          \note The attribute types are listed in terralib/datatype/DataTypes.h  
        */
        int getElementsTypeCode();

        /*!
          \brief Gets the number of elements in the i-th dimension.

          \param i  The dimension index.

          \return The number of elements in the i-th dimension.
        */
        std::size_t getDimensionSize(std::size_t i) const;

        /*!
          \brief Inserts data into specified position.

          \param data    The data to be inserted.
          \param pos     The position in the array to insert data (d1, d2, ... dn).
        */
        void insert(te::dt::AbstractData* data, const std::vector<std::size_t>& pos);

        /*!
          \brief Returns data from specified position.

          \param i      The position in the array to get data (d1, d2, ... dn).

          \return The data from specified position.
        */
        te::dt::AbstractData* getData(const std::vector<std::size_t>& i);

        /*!
          \brief Access data in i index.

          \param i The data index, a vector of std::size_t.

          \warning The caller is reponsible for providing a valid index.

          \return A reference to the i-th data.
        */
        te::dt::AbstractData& operator[](const std::vector<std::size_t>& i);

        /*!
          \brief It creates a new clone of the array.

          \return A new clone of the array. The caller will take its ownership.
        */
        AbstractData* clone() const;

        /*! \brief Returns the type code for array data: ARRAY_TYPE. */
        int getTypeCode() const { return ARRAY_TYPE; }

        /*! \brief Return a string with all the data inside array. */
        std::string toString() const;

      protected:

        std::map<std::vector<std::size_t>, te::dt::AbstractData*> m_data;     //!< A map from positions to data.
        std::size_t m_dimension;                                              //!< The number of dimensions.
        std::vector<std::size_t> m_dimensionSizes;                            //!< The vector of sizes for the dimensions.
        int m_type;                                                           //!< The data type of this array.
    };

  } // end namespace dt
}   // end namespace te

#endif //__TERRALIB_DATATYPE_INTERNAL_ARRAY_H
