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
  \file terralib/datatype/SimpleData.h

  \brief This file contains several implementations for atomic data types (integers, floats, strings and others).
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_SIMPLEDATA_H
#define __TERRALIB_DATATYPE_INTERNAL_SIMPLEDATA_H

// TerraLib
#include "AbstractData.h"
#include "Enums.h"

// STL
#include <sstream>

// Boost
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>

namespace te
{
  namespace dt
  {
    /*!
      \class SimpleData

      \brief A template for atomic data types (integers, floats, strings and others).

      Requirements on type T:
      <ul>
      <li> T must be a copy constructible type.</li>
      <li> T must be used with output streams via operator <<.</li>
      </ul>

      \ingroup datatype

      \sa AbstractData, CompositeData, DataType, DateTime, ByteArray
    */
    template<class T, int typeCode = te::dt::UNKNOWN_TYPE> class SimpleData : public AbstractData
    {
      public:

        /*!
          \brief Constructor.

          \param value The value associated.
        */
        SimpleData(T value)
          : m_val(value)
        {
        }

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side data.
        */
        SimpleData(const SimpleData& rhs)
          : m_val(rhs.m_val)
        {
        }

        /*! \brief Virtual destructor. */
        virtual ~SimpleData() { }

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side data.
        */
        SimpleData& operator=(const SimpleData& rhs)
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
        AbstractData* clone() const
        {
          return new SimpleData(*this);
        }

        /*!
          \brief It returns the data type code associated to the data value.

          \return The data type code associated to the data value.
        */
        virtual int getTypeCode() const
        {
          return typeCode;
        }

        /*!
          \brief It returns the data value in a string representation.

          \return The data value in a string representation.
        */
        virtual std::string toString() const
        {
          //std::stringstream stream(std::ios_base::in | std::ios_base::out);
          //stream << m_val;
          //return stream.str();
          return boost::lexical_cast<std::string>(m_val);
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

    /*!
      \class SimpleData<T*, DT>

      \brief Template specialization for simple data using pointers.

      Requirements on type T:
      <ul>
      <li> T must be a copy constructible type.</li>
      <li> T must be used with output streams via operator <<.</li>
      </ul>
    */
    template<class T, int typeCode> class SimpleData<T*, typeCode> : public AbstractData
    {
      public:

        SimpleData(T* value)
          : m_val(value)
        {
        }

        SimpleData(const SimpleData& rhs)
        {
          m_val = rhs.m_val ? new T(rhs.m_val) : 0;
        }

        virtual ~SimpleData()
        {
          delete m_val;
        }

        SimpleData& operator=(const SimpleData& rhs)
        {
          if(this != &rhs)
          {
            delete m_val;
            m_val = rhs.m_val ? new T(rhs.m_val) : 0;
          }

          return *this;
        }

        AbstractData* clone() const
        {
          return new SimpleData(*this);
        }

        virtual int getTypeCode() const
        {
          return typeCode;
        }

        virtual std::string toString() const
        {
          std::stringstream stream(std::ios_base::in | std::ios_base::out);
          stream << *m_val;
          return stream.str();
        }

        T* getValue() const
        {
          return m_val;
        }

      private:

        T* m_val;  //!< The data value.
    };

    typedef SimpleData<char, CHAR_TYPE> Char;
    typedef SimpleData<unsigned char, UCHAR_TYPE> UChar;
    typedef SimpleData<boost::int16_t, INT16_TYPE> Int16;
    typedef SimpleData<boost::uint16_t, UINT16_TYPE> UInt16;
    typedef SimpleData<boost::int32_t, INT32_TYPE> Int32;
    typedef SimpleData<boost::uint32_t, UINT32_TYPE> UInt32;
    typedef SimpleData<boost::int64_t, INT64_TYPE> Int64;
    typedef SimpleData<boost::uint64_t, UINT64_TYPE> UInt64;
    typedef SimpleData<bool, BOOLEAN_TYPE> Boolean;
    typedef SimpleData<float, FLOAT_TYPE> Float;
    typedef SimpleData<double, DOUBLE_TYPE> Double;
    typedef SimpleData<std::string, NUMERIC_TYPE> Numeric;
    typedef SimpleData<std::string, STRING_TYPE> String;

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_SIMPLEDATA_H

