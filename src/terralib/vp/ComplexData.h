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
  \file terralib/datatype/ComplexData.h

  \brief This file contains several implementations for complex data types.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_COMPLEXDATA_H
#define __TERRALIB_DATATYPE_INTERNAL_COMPLEXDATA_H

// TerraLib
#include "../datatype/AbstractData.h"
#include "../datatype/Enums.h"
#include "Config.h"

namespace te
{
  namespace vp
  {
    /*!
      \class ComplexData

      \brief A template for complex data types.

      Requirements on type T:
      <ul>
      <li> T must be a copy constructible type.</li>
      </ul>

      \ingroup datatype

      \sa AbstractData, CompositeData, DataType, DateTime, ByteArray
    */
    template<class T> class ComplexData : public te::dt::AbstractData
    {
      public:

        /*!
          \brief Constructor.

          \param value The value associated.
        */
        ComplexData(T value)
          : m_val(value)
        {
        }

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side data.
        */
        ComplexData(const ComplexData& rhs)
          : m_val(rhs.m_val)
        {
        }

        /*! \brief Virtual destructor. */
        virtual ~ComplexData() { }

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side data.
        */
        ComplexData& operator=(const ComplexData& rhs)
        {
          if(this != &rhs)
          {
            m_val = rhs.m_val;
          }

          return *this;
        }

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        ComplexData* clone() const
        {
          return new ComplexData(*this);
        }

        /*!
          \brief It returns the data type code associated to the data value.

          \return The data type code associated to the data value.
        */
        virtual int getTypeCode() const
        {
          return te::dt::UNKNOWN_TYPE;
        }

        /*!
          \brief It returns the data value in a string representation.

          \return The data value in a string representation.
        */
        virtual std::string toString() const
        {
          return "";
        }

        /*!
          \brief It returns the associated value.

          \return The associated value.
        */
        T getValue() const
        {
          return m_val;
        }

      private:

        T m_val; //!< The data value.
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_COMPLEXDATA_H

