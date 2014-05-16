/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file Variant.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Variant.h"
#include "EnumUtils.h"

// STL
#include <sstream>
#include <string>  
#include <exception>

te::layout::Variant::Variant() :
  m_sValue("unknown"),
  m_dValue(-1000.),
  m_iValue(-1000),
  m_lValue(-1000),
  m_fValue(-1000.),
  m_bValue(false),
  m_type(DataTypeNone),
  m_null(true)
{

}

te::layout::Variant::Variant(te::layout::LayoutPropertyDataType type, const void* valueCopy) :
  m_sValue("unknown"),
  m_dValue(-1000.),
  m_iValue(-1000),
  m_lValue(-1000),
  m_fValue(-1000.),
  m_bValue(false),
  m_type(type),
  m_null(true)
{
  if(valueCopy)
  {
    convertValue(valueCopy);
  }
}

te::layout::Variant::~Variant()
{
  
}

te::layout::LayoutPropertyDataType te::layout::Variant::getType()
{
  return m_type;
}

void te::layout::Variant::convertValue( const void* valueCopy )
{
  void* value = const_cast<void*>(valueCopy);
  bool null = true;

  //init variables
  std::string* sp = 0;
  double* dValue = 0;
  float* fValue = 0;
  long* lValue = 0;
  int* iValue = 0;
  bool* bValue = 0;
  te::color::RGBAColor* colorValue = 0;

  try
  {
    switch(m_type)
    {
    case DataTypeString:
      // Cast it back to a string pointer.
      sp = static_cast<std::string*>(value);
      if(sp)
      {
        null = false;
        m_sValue = *sp;
        //return throw_exception("Cast failure! Wrong type.");
      }
      break;
    case DataTypeStringList:
      // Cast it back to a string pointer.
      sp = static_cast<std::string*>(value);
      if(sp)
      {
        null = false;
        m_sValue = *sp;
      }
      break;
    case DataTypeDouble:
      if(checkNumberAsString(value))
      {
        // Cast it back to a string pointer.
        sp = static_cast<std::string*>(value);
        if(sp)
        {
          m_dValue = string2Double(*sp);
          null = false;
        }
      }
      else
      {
        dValue = static_cast<double*>(value);
        if(dValue)
        {
          null = false;
          m_dValue = *dValue;
        }
      }
      break;
    case DataTypeFloat:
      if(checkNumberAsString(value))
      {
        // Cast it back to a string pointer.
        sp = static_cast<std::string*>(value);
        if(sp)
        {
          m_fValue = string2Float(*sp);
          null = false;
        }
      }
      else
      {
        fValue = static_cast<float*>(value);
        if(fValue)
        {
          null = false;
          m_fValue = *fValue;
        }
      }
      break;
    case DataTypeLong:
      if(checkNumberAsString(value))
      {
        // Cast it back to a string pointer.
        sp = static_cast<std::string*>(value);
        if(sp)
        {
          m_lValue = string2Long(*sp);
          null = false;
        }
      }
      else
      {
        lValue = static_cast<long*>(value);
        if(lValue)
        {
          null = false;
          m_lValue = *lValue;
        }
      }
      break;
    case DataTypeInt:
      if(checkNumberAsString(value))
      {
        // Cast it back to a string pointer.
        sp = static_cast<std::string*>(value);
        if(sp)
        {
          m_iValue = string2Int(*sp);
          null = false;
        }
      }
      else
      {
        iValue = static_cast<int*>(value);
        if(iValue)
        {
          null = false;
          m_iValue = *iValue;
        }
      }
      break;
    case DataTypeBool:
      bValue = static_cast<bool*>(value);
      if(bValue)
      {
        null = false;
        m_bValue = *bValue;
      }
      break;
    case DataTypeGridSettings:
      // Cast it back to a string pointer.
      sp = static_cast<std::string*>(value);
      if(sp)
      {
        null = false;
        m_sValue = *sp;
      }
      break;
    case DataTypeColor:
      // Cast it back to a string pointer.
      colorValue = static_cast<te::color::RGBAColor*>(value);
      if(colorValue)
      {
        null = false;
        m_colorValue = *colorValue;
      }
      break;
    default:
      null = true;
      break;
    }
  }
  catch (std::exception const& e)
  {
    std::string s_type = te::layout::getLayoutPropertyDataType(m_type);
    std::cerr << e.what() << "Failed - te::layout::Variant: convert to " << s_type << std::endl;
  }

  m_null = null;
}

std::string te::layout::Variant::toString()
{
  return m_sValue;
}

double te::layout::Variant::toDouble()
{
  return m_dValue;
}

int te::layout::Variant::toInt()
{
  return m_iValue;
}

long te::layout::Variant::toLong()
{
  return m_lValue;
}

float te::layout::Variant::toFloat()
{
  return m_fValue;
}

bool te::layout::Variant::toBool()
{
  return m_bValue;
}

te::color::RGBAColor te::layout::Variant::toColor()
{
  return m_colorValue;
}

bool te::layout::Variant::isNull()
{
  return m_null;
}

void te::layout::Variant::clear()
{
  m_sValue = "unknown";
  m_dValue = -1000.;
  m_iValue = -1000;
  m_lValue = -1000;
  m_fValue = -1000.;
  m_bValue = false;
  m_type = DataTypeNone;
  m_null = true;
}

std::string te::layout::Variant::convertToString()
{
  std::string s_convert;
  std::stringstream ss;//create a stringstream
  
  if(m_null)
    return s_convert;

  if(m_type == DataTypeNone)
    return s_convert;

  if(m_sValue.compare("unknown") != 0)
  {
    s_convert = m_sValue;
  }
  else if(m_dValue != -1000.)
  {
    ss << m_dValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_iValue != -1000)
  {
    ss << m_iValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_lValue != -1000)
  {
    ss << m_iValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_fValue != -1000.)
  {
    ss << m_fValue;//add number to the stream
    s_convert = ss.str();
  }
  else 
  {
    if(m_bValue)
    {
      s_convert = "true";
    }
    else
    {
      s_convert = "false";
    }
  }
  
  return s_convert;
}

bool te::layout::Variant::checkNumberAsString( const void* valueCopy )
{
  void* value = const_cast<void*>(valueCopy);

  std::string* sp = 0;
  bool result = false;

  try
  {
    // Cast it back to a string pointer.
    sp = static_cast<std::string*>(value);   

    if(sp)
    {
      std::string res = *sp;
      
      /* Verification because the result of static_cast<std::string*> 
        may be garbage, if the value is not a string. */
      std::istringstream buffer(res);
      int value = 0;
      buffer >> value;

      if (buffer.fail())
      {
        result = false;
      }
      else
      {
        result = true;
      }
    }
  }
  catch(std::exception const& e)
  {
    result = false;
  }

  return result;
}

double te::layout::Variant::string2Double( std::string str )
{
  // Convert a string representation of a number into a double value.
  
  double result;

  // Get rid of any trailing whitespace
  str.erase( str.find_last_not_of( " \f\n\r\t\v" ) + 1 );

  // Read it into the target type
  std::istringstream ss( str );
  ss >> result;

  // Check to see that there is nothing left over
  if (!ss.eof())
  {
    throw std::runtime_error("Failed: convert string to double.");
  }

  return result;
}

int te::layout::Variant::string2Int( std::string str )
{
  // Convert a string representation of a number into a int value.

  int result;

  // Get rid of any trailing whitespace
  str.erase( str.find_last_not_of( " \f\n\r\t\v" ) + 1 );

  // Read it into the target type
  std::istringstream ss( str );
  ss >> result;

  // Check to see that there is nothing left over
  if (!ss.eof())
  {
    throw std::runtime_error("Failed: convert string to int.");
  }

  return result;
}

float te::layout::Variant::string2Float( std::string str )
{
  // Convert a string representation of a number into a floating point value.

  float result;

  // Get rid of any trailing whitespace
  str.erase( str.find_last_not_of( " \f\n\r\t\v" ) + 1 );

  // Read it into the target type
  std::istringstream ss( str );
  ss >> result;

  // Check to see that there is nothing left over
  if (!ss.eof())
  {
    throw std::runtime_error("Failed: convert string to float.");
  }

  return result;
}

long te::layout::Variant::string2Long( std::string str )
{
  // Convert a string representation of a number into a long value.

  long result;

  // Get rid of any trailing whitespace
  str.erase( str.find_last_not_of( " \f\n\r\t\v" ) + 1 );

  // Read it into the target type
  std::istringstream ss( str );
  ss >> result;

  // Check to see that there is nothing left over
  if (!ss.eof())
  {
    throw std::runtime_error("Failed: convert string to long.");
  }

  return result;
}

