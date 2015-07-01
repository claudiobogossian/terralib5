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
  \file terralib/raster/BlockUtils.h

  \brief Utility functions for dealing with raster data blocks.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_BLOCKUTILS_H
#define __TERRALIB_RASTER_INTERNAL_BLOCKUTILS_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace rst
  {
    /*! \brief The type of function used to extract data from a buffer. */
    typedef void (*GetBufferValueFPtr)(int index, void* buffer, double* value);

    /*! \brief The type of function used to extract data from a buffer. */
    typedef void (*SetBufferValueFPtr)(int index, void* buffer, const double* value);

    TERASTEREXPORT void DummyGetValue(int index, void* buffer, double* value);

    TERASTEREXPORT void DummySetValue(int index, void* buffer, const double* value);

    TERASTEREXPORT void Get4bits(int index, void* buffer, double* value);

    TERASTEREXPORT void Get2bits(int index, void* buffer, double* value);

    TERASTEREXPORT void Get1bit(int index, void* buffer, double* value);

    TERASTEREXPORT void GetUChar(int index, void* buffer, double* value);

    TERASTEREXPORT void GetChar(int index, void* buffer, double* value);

    TERASTEREXPORT void GetUInt16(int index, void* buffer, double* value);

    TERASTEREXPORT void GetInt16(int index, void* buffer, double* value);

    TERASTEREXPORT void GetUInt32(int index, void* buffer, double* value);

    TERASTEREXPORT void GetInt32(int index, void* buffer, double* value);

    TERASTEREXPORT void GetFloat(int index, void* buffer, double* value);

    TERASTEREXPORT void GetDouble(int index, void* buffer, double* value);

    TERASTEREXPORT void GetCRInt16(int index, void* buffer, double* value);

    TERASTEREXPORT void GetCIInt16(int index, void* buffer, double* value);

    TERASTEREXPORT void GetCRInt32(int index, void* buffer, double* value);

    TERASTEREXPORT void GetCIInt32(int index, void* buffer, double* value);

    TERASTEREXPORT void GetCRFloat(int index, void* buffer, double* value);

    TERASTEREXPORT void GetCIFloat(int index, void* buffer, double* value);

    TERASTEREXPORT void GetCRDouble(int index, void* buffer, double* value);

    TERASTEREXPORT void GetCIDouble(int index, void* buffer, double* value);

    TERASTEREXPORT void Set4bits(int index, void* buffer, const double* value);

    TERASTEREXPORT void Set2bits(int index, void* buffer, const double* value);

    TERASTEREXPORT void Set1bit(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetUChar(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetChar(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetUInt16(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetInt16(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetUInt32(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetInt32(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetFloat(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetDouble(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetCRInt16(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetCIInt16(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetCRInt32(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetCIInt32(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetCRFloat(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetCIFloat(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetCRDouble(int index, void* buffer, const double* value);

    TERASTEREXPORT void SetCIDouble(int index, void* buffer, const double* value);

    /*!
      \brief Sets the pointers to functions that helps to extract a double or complex value from a specific buffer data type.

      \param gb    The get buffer of real values function.
      \param gbi   The get buffer of imaginary values function.
      \param sb    The set buffer of real values function.
      \param sbi   The set buffer of imaginary values function.
      \param type  The block data type.
    */
    TERASTEREXPORT void SetBlockFunctions(GetBufferValueFPtr* gb, GetBufferValueFPtr* gbi,
                                          SetBufferValueFPtr* sb, SetBufferValueFPtr* sbi, int type);

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_BLOCKUTILS_H

