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
  \file terralib/datatype/ByteArray.h

  \brief A class for representing binary data.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_BYTEARRAY_H
#define __TERRALIB_DATATYPE_INTERNAL_BYTEARRAY_H

// TerraLib
#include "AbstractData.h"
#include "Enums.h"

// Boost
#include <boost/cstdint.hpp>

namespace te
{
  namespace dt
  {
    /*!
      \class ByteArray

      \brief A class for representing binary data.

      A byte array can be used as a container for binary data.

      \ingroup datatype

      \sa AbstractData, DataType, DateTime, SimpleData, CompositeData
    */
    class TEDATATYPEEXPORT ByteArray : public AbstractData
    {
      public:

        /*! \brief It creates a new empty byte array. */
        ByteArray();

        /*!
          \brief It creates a new byte array and allocates size bytes.

          \param capacity The number of bytes for the internal data buffer.
        */
        ByteArray(std::size_t capacity);

        /*!
          \brief It creates a new byte array taking the ownership of the external data buffer.

          \param data The data array.
          \param size The number of bytes in the data array. This value will be also the capacity of the byte array buffer.

           \note The byte array will take the ownership of the given data.
        */
        ByteArray(char* data, std::size_t size);

        /*!
          \brief It creates a new byte array taking the ownership of the external data buffer.

          \param data       The data array.
          \param capacity   The number of bytes available in the data buffer, it must be greater than or equal to usedBytes.
          \param usedBytes  The number of used bytes in the given data buffer.

          \note The byte array will take the ownership of the given data.
        */
        ByteArray(char* data, std::size_t capacity, std::size_t usedBytes);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side instance.
        */
        ByteArray(const ByteArray& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side instance.

          \return A reference to this byte array.
        */
        ByteArray& operator=(const ByteArray& rhs);

        /*! \brief Destructor. */
        ~ByteArray();        

        /*!
          \brief It returns the data array.

          \return The data array.

          \note Don't free the returned pointer (it belongs to the byte array object).
        */
        char* getData() const;

        /*!
          \brief It takes the ownership of the external data buffer.

          \param data The data array.
          \param size The number of bytes in the array of data. This value will be also the capacity of the byte array buffer.

          \note The byte array will take the ownership of the given data.

          \note It will drop the internal buffer before taking the data argument.
        */
        void take(char* data, std::size_t size);

        /*!
          \brief It takes the ownership of the external data buffer.

          \param data       The data array.
          \param capacity   The number of bytes available in the data buffer, it must be greater than or equal to usedBytes.
          \param usedBytes  The number of used bytes in the given data buffer.

          \note The byte array will take the ownership of the given data.

          \note It will drop the internal buffer before taking the data argument.
        */
        void take(char* data, std::size_t capacity, std::size_t usedBytes);

        /*!
          \brief It copies the data from the given pointer to the byte array.

          \param data The data array to be copied.
          \param size The number of bytes to be copied from the input array.

          \note It will drop the internal buffer if its capacity is less than size bytes.
        */
        void copy(char* data, std::size_t size);

        /*!
          \brief It copies the data from the given pointer to the byte array.

          \param data   The data array to be copied.          
          \param size   The number of bytes to be copied from the input array.
          \param offset An offset in the internal data buffer.

          \note It will expand the internal buffer if its capacity is less than offset + size bytes.

          \note After this operation the number of bytes ocupied will be set to offset + size.
        */
        void copy(char* data, std::size_t size, std::size_t offset);
        
        /*!
          \brief It returns the size of the internal buffer.

          \return The number of bytes in the internal buffer.
        */
        std::size_t capacity() const;

        /*!
          \brief It returns the number of used bytes in the internal buffer.

          \return The number of used bytes in the internal buffer.
        */
        std::size_t bytesUsed() const;

        /*!
          \brief It sets the number of used bytes in the internal buffer.

          \param size The number of used bytes in the internal buffer.
        */
        void setBytesUsed(std::size_t size);

        /*! \brief It clears the byte array. */
        void clear();

        /*!
          \brief It creates a new clone of the byte array

          \return A new clone of the byte array. The caller will take its ownership.
        */
        AbstractData* clone() const;

        /*!
          \brief The type code for byte array data: BYTE_ARRAY_TYPE.

          \return The type code for byte array data: BYTE_ARRAY_TYPE.
        */
        int getTypeCode() const { return BYTE_ARRAY_TYPE; }

        /*!
          \brief It returns the byte array in an string notation.

          \return An hex-string encoded version of the internal data buffer.

          \todo Remove the copy and allocation overhead in this method.
        */
        std::string toString() const;

      private:

        char* m_data;                 //!< The buffer data.
        std::size_t m_capacity;       //!< The number of bytes allocated by the internal buffer.
        std::size_t m_bytesOccupied;  //!< The number of bytes occupied in the buffer.
    };

  } // end namespace dt
}   // end namespace te

/*!
  \brief It writes the bytes from val to the byte array.

  \param barray A byte array container.
  \param val    A simple data type value like: an int, a double or a float.

  \return The byte array.
*/
template<class T> inline te::dt::ByteArray& operator<<(te::dt::ByteArray& barray, T val)
{
  barray.copy((char*)(&val), sizeof(T), barray.bytesUsed());

  return barray;
}


#endif  // __TERRALIB_DATATYPE_INTERNAL_BYTEARRAY_H



