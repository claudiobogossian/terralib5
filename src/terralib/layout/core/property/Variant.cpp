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
  \file Variant.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Variant.h"
#include "../enum/Enums.h"

// STL
#include <sstream>
#include <string>  
#include <exception>
#include <stdexcept>
#include <cctype>
#include <iostream>
#include <stdlib.h>

te::layout::Variant::Variant() :
  m_sValue("unknown"),
  m_dValue(-1000.),
  m_iValue(-1000),
  m_lValue(-1000),
  m_fValue(-1000.),
  m_bValue(false),
  m_type(0),
  m_null(true),
  m_complex(false)
{
  m_type = Enums::getInstance().getEnumDataType()->getDataTypeNone();
}

te::layout::Variant::Variant(te::layout::EnumType* type, const void* valueCopy) :
  m_sValue("unknown"),
  m_dValue(-1000.),
  m_iValue(-1000),
  m_lValue(-1000),
  m_fValue(-1000.),
  m_bValue(false),
  m_type(type),
  m_null(true),
  m_complex(false)
{
  if(valueCopy)
  {
    convertValue(valueCopy);
  }
}

te::layout::Variant::~Variant()
{
  
}

te::layout::EnumType* te::layout::Variant::getType()
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
  Font* fontValue = 0;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!m_type || !dataType)
  {
    return;
  }

  try
  {
    if(m_type == dataType->getDataTypeString())
    {
      // Cast it back to a string pointer.
      sp = static_cast<std::string*>(value);
      if(sp)
      {
        null = false;
        m_sValue = *sp;
      }
    }
    else if(m_type == dataType->getDataTypeStringList())
    {
      // Cast it back to a string pointer.
      sp = static_cast<std::string*>(value);
      if(sp)
      {
        null = false;
        m_sValue = *sp;
      }
    }
   else if(m_type == dataType->getDataTypeDouble())
   {
      dValue = static_cast<double*>(value);
      if(dValue)
      {
        null = false;
        m_dValue = *dValue;
      }      
   }
   else if(m_type == dataType->getDataTypeFloat())
   {
      fValue = static_cast<float*>(value);
      if(fValue)
      {
        null = false;
        m_fValue = *fValue;
      }
   }
   else if(m_type == dataType->getDataTypeLong())
   {
      lValue = static_cast<long*>(value);
      if(lValue)
      {
        null = false;
        m_lValue = *lValue;
      }
      
   }
   else if(m_type == dataType->getDataTypeInt())
   {
      iValue = static_cast<int*>(value);
      if(iValue)
      {
        null = false;
        m_iValue = *iValue;
      }
   }
   else if(m_type == dataType->getDataTypeBool())
   {
      bValue = static_cast<bool*>(value);
      if(bValue)
      {
        null = false;
        m_bValue = *bValue;
      }
   }
   else if(m_type == dataType->getDataTypeColor())
   {
      // Cast it back to a string pointer.
      colorValue = static_cast<te::color::RGBAColor*>(value);
      if(colorValue)
      {
        null = false;
        m_colorValue = *colorValue;
        m_complex = true;
      }
   }
   else if(m_type == dataType->getDataTypeFont())
   {
      // Cast it back to a string pointer.
      fontValue = static_cast<Font*>(value);
      if(fontValue)
      {
        null = false;
        m_fontValue = *fontValue;
        m_complex = true;
      }
   }
   else // Any remaining data will be by default "std::string"  
   {
     // Cast it back to a string pointer.
     sp = static_cast<std::string*>(value);
     if(sp)
     {
       null = false;
       m_sValue = *sp;
     }
   }
  }
  catch (std::exception const& e)
  {
    std::string s_type = m_type->getName();
    std::cerr << e.what() << "Failed - te::layout::Variant: convert to " << s_type << std::endl;
  }

  m_null = null;
}

void te::layout::Variant::fromPtree( boost::property_tree::ptree tree, EnumType* type )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  bool null = true;

  if(!dataType)
  {
    return;
  }

  if(!type)
  {
    return;
  }

  /* the ptree boost returns data with string type */

  try
  {
    if(type == dataType->getDataTypeString())
    {
      m_sValue = tree.data();
      null = false;
    }
    else if(type == dataType->getDataTypeDouble())
    {
      m_dValue = std::atof(tree.data().c_str());
      null = false;
    }
    else if(type == dataType->getDataTypeInt())
    {
      m_iValue = std::atoi(tree.data().c_str());
      null = false;
    }
    else if(type == dataType->getDataTypeLong())
    {
      m_lValue = std::atol(tree.data().c_str());
      null = false;
    }
    else if(type == dataType->getDataTypeFloat())
    {
      m_fValue = (float)std::atof(tree.data().c_str());
      null = false;
    }
    else if(type == dataType->getDataTypeBool())
    {
      m_bValue = toBool(tree.data());
      null = false;
    }
    else if(type == dataType->getDataTypeColor())
    {
      std::string color = tree.data();

      std::vector<std::string> strings;
      std::istringstream f(color);
      std::string s;    
      while (std::getline(f, s, ',')) 
      {
        strings.push_back(s);
      }

      if(strings.empty() || strings.size() > 4)
        return;

      int r = std::atoi(strings[0].c_str());
      int g = std::atoi(strings[1].c_str());
      int b = std::atoi(strings[2].c_str());
      int a = std::atoi(strings[3].c_str());

      m_colorValue.setColor(r,g,b,a);

      m_complex = true;
      null = false;
    }
    else if(type == dataType->getDataTypeFont())
    {
      std::string font = tree.data();
      m_fontValue.fromString(font);
      m_complex = true;
      null = false;
    }
    else // Any remaining data will be by default "std::string"  
    {
      m_sValue = tree.data();
      null = false;
    }
  }
  catch (std::exception const& e)
  {
    std::string s_type = type->getName();
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

te::layout::Font te::layout::Variant::toFont()
{
  return m_fontValue;
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
  m_type = Enums::getInstance().getEnumDataType()->getDataTypeNone();
  m_null = true;
}

std::string te::layout::Variant::convertToString()
{
  std::stringstream ss;//create a stringstream
  std::string s_convert;
  
  if(m_null)
    return s_convert;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(m_type == dataType->getDataTypeNone())
    return s_convert;

  if(m_type == dataType->getDataTypeString())
  {
    s_convert = m_sValue;
  }
  else if(m_type == dataType->getDataTypeDouble())
  {
    ss << m_dValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_type == dataType->getDataTypeInt())
  {
    ss << m_iValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_type == dataType->getDataTypeLong())
  {
    ss << m_lValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_type == dataType->getDataTypeFloat())
  {
    ss << m_fValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_type == dataType->getDataTypeColor())
  {
    s_convert = toString(m_colorValue.getRed());
    s_convert += "," + toString(m_colorValue.getGreen());
    s_convert += "," + toString(m_colorValue.getBlue());
    s_convert += "," + toString(m_colorValue.getAlpha());
  }
  else if(m_type == dataType->getDataTypeFont())
  {
    s_convert = m_fontValue.toString();
  }
  else if(m_type == dataType->getDataTypeBool()) 
  {
    s_convert = m_bValue ? "true" : "false"; 
  }
  
  return s_convert;
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

bool te::layout::Variant::isComplex()
{
  return m_complex;
}

std::string te::layout::Variant::toString( int value )
{
  std::stringstream ss;//create a stringstream
  ss << value;//add number to the stream
  
  return ss.str();
}

bool te::layout::Variant::toBool( std::string str )
{
  if(str.compare("true") == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}



