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
  \file terralib/raster/BlockUtils.cpp

  \brief Utility functions for dealing with raster data blocks.
*/

// TerraLib
#include "../datatype/Enums.h"
#include "BlockUtils.h"
#include "Exception.h"

namespace te
{
  namespace rst
  {
    /*!
      \struct CInt16

      \brief A simple type for use by driver to cast raster data blocks.
    */
    struct CInt16
    {
      short r;
      short i;
    };

    /*!
      \struct CInt32

      \brief A simple type for use by driver to cast raster data blocks.
    */
    struct CInt32
    {
      int r;
      int i;
    };

    /*!
      \struct CFloat

      \brief A simple type for use by driver to cast raster data blocks.
    */
    struct CFloat
    {
      float r;
      float i;
    };

    /*!
      \struct CDouble

      \brief A simple type for use by driver to cast raster data blocks.
    */
    struct CDouble
    {
      double r;
      double i;
    };

  } // end namespace rst
}   // end namespace te

void te::rst::DummyGetValue(int /*index*/, void* /*buffer*/, double* /*value*/)
{
}

void te::rst::DummySetValue(int /*index*/, void* /*buffer*/, const double* /*value*/)
{
}

void te::rst::Get4bits(int index, void* buffer, double* value)
{
  unsigned int bitshift = (index % 2) * 4;
  unsigned char ucvalue = ((unsigned char*) buffer) [index >> 1]; // index >> 1 is used to divide the index by 2
  *value = (double) ( (ucvalue >> bitshift) & 15 ); // '& 15' is used to get only first 4 bits (mask 00001111)
}

void te::rst::Get2bits(int index, void* buffer, double* value)
{
  unsigned int bitshift = (index % 4) * 2;
  unsigned char ucvalue = ((unsigned char*) buffer) [index >> 2]; // index >> 2 is used to divide the index by 4
  *value = (double) ( ( ucvalue >> bitshift ) & 3 ); // '& 3' is used to get only first 2 bits (mask 00000011)
}

void te::rst::Get1bit(int index, void* buffer, double* value)
{
  unsigned int bitshift = (index % 8);
  unsigned char ucvalue = ((unsigned char*) buffer) [index >> 3]; // index >> 3 is used to divide the index by 8
  *value = (double) ( ( ucvalue >> bitshift ) & 1 ); // '& 1' is used to get only first bit (mask 00000001)
}

void te::rst::GetUChar(int index, void* buffer, double* value)
{
  *value = (double)((unsigned char*)buffer)[index];
}

void te::rst::GetChar(int index, void* buffer, double* value)
{
  *value = (double)((char*)buffer)[index];
}

void te::rst::GetUInt16(int index, void* buffer, double* value)
{
  *value = (double)((unsigned short*)buffer)[index];
}

void te::rst::GetInt16(int index, void* buffer, double* value)
{
  *value = (double)((short*)buffer)[index];
}

void te::rst::GetUInt32(int index, void* buffer, double* value)
{
  *value = (double)((unsigned int*)buffer)[index];
}

void te::rst::GetInt32(int index, void* buffer, double* value)
{
  *value = (double)((int*)buffer)[index];
}

void te::rst::GetFloat(int index, void* buffer, double* value)
{
  *value = (double)((float*)buffer)[index];
}

void te::rst::GetDouble(int index, void* buffer, double* value)
{
  *value = ((double*)buffer)[index];
}

void te::rst::GetCRInt16(int index, void* buffer, double* value)
{
  *value = (double)(((CInt16*)buffer)[index]).r;
}

void te::rst::GetCIInt16(int index, void* buffer, double* value)
{
  *value = (double)(((CInt16*)buffer)[index]).i;
}

void te::rst::GetCRInt32(int index, void* buffer, double* value)
{
  *value = (double)(((CInt32*)buffer)[index]).r;
}

void te::rst::GetCIInt32(int index, void* buffer, double* value)
{
  *value = (double)(((CInt32*)buffer)[index]).i;
}

void te::rst::GetCRFloat(int index, void* buffer, double* value)
{
  *value = (double)(((CFloat*)buffer)[index]).r;
}

void te::rst::GetCIFloat(int index, void* buffer, double* value)
{
  *value = (double)(((CFloat*)buffer)[index]).i;
}

void te::rst::GetCRDouble(int index, void* buffer, double* value)
{
  *value = (((CDouble*)buffer)[index]).r;
}

void te::rst::GetCIDouble(int index, void* buffer, double* value)
{
  *value = (((CDouble*)buffer)[index]).i;
}

void te::rst::Set1bit(int index, void* buffer, const double* value)
{
  unsigned int bitshift = (index % 8);
  unsigned char firstbit = ((unsigned char) *value) & 1; // '& 1' is used to get only first bit (mask 00000001)
  ((unsigned char*)buffer)[index >> 3] &= ~(1u << bitshift); // clear bit value, index >> 3 is used to divide the index by 8
  ((unsigned char*)buffer)[index >> 3] |= (firstbit << bitshift); // set new value, index >> 3 is used to divide the index by 8
}

void te::rst::Set2bits(int index, void* buffer, const double* value)
{
  unsigned int bitshift = (index % 4) * 2;
  unsigned char first2bits = ((unsigned char) *value) & 3; // '& 3' is used to get only first 2 bits (mask 00000011)
  ((unsigned char*)buffer)[index >> 2] &= ~(3u << bitshift); // clear bit values, index >> 2 is used to divide the index by 4
  ((unsigned char*)buffer)[index >> 2] |= (first2bits << bitshift); // set new value, index >> 2 is used to divide the index by 4
}

void te::rst::Set4bits(int index, void* buffer, const double* value)
{
  unsigned int bitshift = (index % 2) * 4;
  unsigned char first4bits = ((unsigned char) *value) & 15; // '& 15' is used to get only first 4 bits (mask 00001111)
  ((unsigned char*)buffer)[index >> 1] &= ~(15u << bitshift); // clear bit values, index >> 1 is used to divide the index by 2
  ((unsigned char*)buffer)[index >> 1] |= (first4bits << bitshift); // set new value, index >> 1 is used to divide the index by 2
}

void te::rst::SetUChar(int index, void* buffer, const double* value)
{
  ((unsigned char*)buffer)[index] = (unsigned char) *value;
}

void te::rst::SetChar(int index, void* buffer, const double* value)
{
  ((char*)buffer)[index] = (char) *value;
}

void te::rst::SetUInt16(int index, void* buffer, const double* value)
{
  ((unsigned short*)buffer)[index] = (unsigned short) *value;
}

void te::rst::SetInt16(int index, void* buffer, const double* value)
{
  ((short*)buffer)[index] = (short) *value;
}

void te::rst::SetUInt32(int index, void* buffer, const double* value)
{
  ((unsigned int*)buffer)[index] = (unsigned int) *value;
}

void te::rst::SetInt32(int index, void* buffer, const double* value)
{
  ((int*)buffer)[index] = (int) *value;
}

void te::rst::SetFloat(int index, void* buffer, const double* value)
{
  ((float*)buffer)[index] = (float) *value;
}

void te::rst::SetDouble(int index, void* buffer, const double* value)
{
  ((double*)buffer)[index] = (double) *value;
}

void te::rst::SetCRInt16(int index, void* buffer, const double* value)
{
  (((CInt16*)buffer)[index]).r = (short) *value;
}

void te::rst::SetCIInt16(int index, void* buffer, const double* value)
{
  (((CInt16*)buffer)[index]).i = (short)(*(value));
}

void te::rst::SetCRInt32(int index, void* buffer, const double* value)
{
  (((CInt32*)buffer)[index]).r = (int) *value;
}

void te::rst::SetCIInt32(int index, void* buffer, const double* value)
{
  (((CInt32*)buffer)[index]).i = (int)(*(value));
}

void te::rst::SetCRFloat(int index, void* buffer, const double* value)
{
  (((CFloat*)buffer)[index]).r = (float) *value;
}

void te::rst::SetCIFloat(int index, void* buffer, const double* value)
{
  (((CFloat*)buffer)[index]).i = (float) *(value);
}

void te::rst::SetCRDouble(int index, void* buffer, const double* value)
{
  (((CDouble*)buffer)[index]).r = *value;
}

void te::rst::SetCIDouble(int index, void* buffer, const double* value)
{
  (((CDouble*)buffer)[index]).i = *value;
}

void te::rst::SetBlockFunctions(GetBufferValueFPtr *gb, GetBufferValueFPtr *gbi,
                                SetBufferValueFPtr *sb, SetBufferValueFPtr *sbi, int type)
{
  switch(type)
  {
    case te::dt::R4BITS_TYPE:
      *gb = te::rst::Get4bits;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::Set4bits;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::R2BITS_TYPE:
      *gb = te::rst::Get2bits;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::Set2bits;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::R1BIT_TYPE:
      *gb = te::rst::Get1bit;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::Set1bit;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::UCHAR_TYPE:
      *gb = te::rst::GetUChar;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::SetUChar;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::CHAR_TYPE:
      *gb = te::rst::GetChar;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::SetChar;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::UINT16_TYPE:
      *gb = te::rst::GetUInt16;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::SetUInt16;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::INT16_TYPE:
      *gb = te::rst::GetInt16;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::SetInt16;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::UINT32_TYPE:
      *gb = te::rst::GetUInt32;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::SetUInt32;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::INT32_TYPE:
      *gb = te::rst::GetInt32;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::SetInt32;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::FLOAT_TYPE:
      *gb = te::rst::GetFloat;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::SetFloat;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::DOUBLE_TYPE:
      *gb = te::rst::GetDouble;
      *gbi = te::rst::DummyGetValue;
      *sb = te::rst::SetDouble;
      *sbi = te::rst::DummySetValue;
    break;

    case te::dt::CINT16_TYPE:
      *gb = te::rst::GetCRInt16;
      *gbi = te::rst::GetCIInt16;
      *sb = te::rst::SetCRInt16;
      *sbi = te::rst::SetCIInt16;
    break;

    case te::dt::CINT32_TYPE:
      *gb = te::rst::GetCRInt32;
      *gbi = te::rst::GetCIInt32;
      *sb = te::rst::SetCRInt32;
      *sbi = te::rst::SetCIInt32;
    break;

    case te::dt::CFLOAT_TYPE:
      *gb = te::rst::GetCRFloat;
      *gbi = te::rst::GetCIFloat;
      *sb = te::rst::SetCRFloat;
      *sbi = te::rst::SetCIFloat;
    break;

    case te::dt::CDOUBLE_TYPE:
      *gb = te::rst::GetCRDouble;
      *gbi = te::rst::GetCIDouble;
      *sb = te::rst::SetCRDouble;
      *sbi = te::rst::SetCIDouble;
    break;

    default:
      throw te::rst::Exception("Invalid data type");
  }
}
