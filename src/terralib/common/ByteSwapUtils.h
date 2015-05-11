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
  \file terralib/common/ByteSwapUtils.h

  \brief Utility functions to swap bytes: double, int and unsigned int.

  \ingroup common
 */

#ifndef __TERRALIB_COMMON_INTERNAL_BYTESWAPUTILS_H
#define __TERRALIB_COMMON_INTERNAL_BYTESWAPUTILS_H

// TerraLib
#include "Config.h"

// STL
#include <cstring>

// Boost
#include <boost/cstdint.hpp>

namespace te
{
  namespace common
  {
    /*!
      \brief It swaps two bytes in local.

      \param v The value we want to swap.

      \note Requirements: The type T must have a sizeof(T) == 2.
    */
    template<class T> inline void Swap2Bytes(T& v)
    {
      char vIn[2], vOut[2];

      memcpy(vIn, &v, sizeof(T));

      vOut[0] = vIn[1];
      vOut[1] = vIn[0];

      memcpy(&v, vOut, sizeof(T));
    }

    /*!
      \brief It swaps an array of two bytes in local.

      \param v The value we want to swap.
    */
    inline void Swap2Bytes(char* v)
    {
      char v0 = v[0];
      v[0] = v[1];
      v[1] = v0;
    }

    /*!
      \brief It swaps four bytes in local.

      \param v The value we want to swap.

      \note Requirements: The type T must have a sizeof(T) == 4.
    */
    template<class T> inline void Swap4Bytes(T& v)
    {
      char vIn[4], vOut[4];

      memcpy(vIn, &v, sizeof(T));

      vOut[0] = vIn[3];
      vOut[1] = vIn[2];
      vOut[2] = vIn[1];
      vOut[3] = vIn[0];

      memcpy(&v, vOut, sizeof(T));
    }

    /*!
      \brief It swaps an array of four bytes in local.

      \param v The value we want to swap.
    */
    inline void Swap4Bytes(char* v)
    {
      char v0 = v[0];
      v[0] = v[3];
      v[3] = v0;

      char v1 = v[1];
      v[1] = v[2];
      v[2] = v1;
    }

    /*!
      \brief It swaps the bytes in local.

      \param v The value we want to swap.

      \note Generic version: swap n-bytes.
    */
    template<class T> inline void SwapBytes(T& v)
    {
      char* vIn = new char[sizeof(T)];
      char* vOut = new char[sizeof(T)];

      memcpy(vIn, &v, sizeof(T));

      for(unsigned int i = 0; i <  sizeof(T); ++i)
        vOut[i] = vIn[sizeof(T) - i - 1];

      memcpy(&v, vOut, sizeof(T));
      delete [] vIn;
      delete [] vOut;
    }

    /*!
      \brief It swaps an array of eight bytes in local.

      \param v The value we want to swap.      
    */
    inline void Swap8Bytes(char* v)
    {
      char v0 = v[0];
      v[0] = v[7];
      v[7] = v0;

      char v1 = v[1];
      v[1] = v[6];
      v[6] = v1;

      char v2 = v[2];
      v[2] = v[5];
      v[5] = v2;

      char v3 = v[3];
      v[3] = v[4];
      v[4] = v3;      
    }

    /*!
      \brief It swaps 2 bytes in local.

      \param v The value we want to swap.
    */
    template<> inline void SwapBytes(boost::int16_t& v)
    {
      Swap2Bytes(v);
    }

    /*!
      \brief It swaps four bytes in local.

      \param v The value we want to swap.
    */
    template<> inline void SwapBytes(boost::int32_t& v)
    {
      Swap4Bytes(v);
    }

    /*!
      \brief It swaps four bytes in local.

      \param v The value we want to swap.
    */
    template<> inline void SwapBytes(boost::uint32_t& v)
    {
      Swap4Bytes(v);
    }

    /*!
      \brief It swaps four bytes in local.

      \param v The value we want to swap.
    */
    template<> inline void SwapBytes(float& v)
    {
      Swap4Bytes(v);
    }

    /*!
      \brief It swaps eight bytes in local.

      \param v The value we want to swap.      
    */
    template<> inline void SwapBytes(double& v)
    {
      /*!
        \union SwapDoubleUnion

        \brief This union is meant to be used only by byte swap functions.

        \note This struct is for internal use only. Don't use it in your own code.
      */
      union SwapDoubleUnion
      {
        double m_dWord;              //!< Double word (64 bit).
        unsigned int m_uintWord[2];  //!< Unsigned int word (64 bit).
      };

      SwapDoubleUnion vAux1, vAux2;

      vAux1.m_dWord = v;

      SwapBytes(vAux1.m_uintWord[0]);
      SwapBytes(vAux1.m_uintWord[1]);

      vAux2.m_uintWord[1] = vAux1.m_uintWord[0];
      vAux2.m_uintWord[0] = vAux1.m_uintWord[1];

      v = vAux2.m_dWord;
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_BYTESWAPUTILS_H

