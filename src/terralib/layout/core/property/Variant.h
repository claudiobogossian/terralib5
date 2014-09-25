/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file Variant.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VARIANT_H 
#define __TERRALIB_LAYOUT_INTERNAL_VARIANT_H

// TerraLib
#include "../enum/AbstractType.h"
#include "../../../color/RGBAColor.h"
#include "../Font.h"
#include "../enum/EnumType.h"

// STL
#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

namespace te
{
  namespace layout
  {
    class Variant
    {
      public:

        Variant();
        Variant(EnumType* type, const void* valueCopy);
        virtual ~Variant();
        
        template <typename ValueType>
        void setValue(ValueType value, EnumType* type);

        EnumType* getType();
        
        std::string toString();
        double toDouble();
        int toInt();
        long toLong();
        float toFloat();
        bool toBool();
        te::color::RGBAColor toColor();     
        Font toFont();

        virtual std::string convertToString();

        bool isNull();

        virtual void clear();
                
        bool operator ==(const Variant& other); 
        bool operator !=(const Variant& other); 

    protected:

      template <typename ValueType>
      void variantSetValue(Variant &v, const ValueType& value, EnumType* type);
      
      virtual void convertValue(const void* valueCopy);
      
      /* Check if a value passed, of type DataTypeInt and etc, is a std::string. 
         Ex.: value returned by a json file (boost). */
      virtual bool checkNumberAsString(const void* valueCopy);
      
      virtual double string2Double(std::string str);

      virtual int string2Int(std::string str);

      virtual float string2Float(std::string str);

      virtual long string2Long(std::string str);

      std::string m_sValue;
      double m_dValue;
      int m_iValue;
      long m_lValue;
      float m_fValue;
      bool m_bValue;
      te::color::RGBAColor m_colorValue;
      Font m_fontValue;
      EnumType* m_type;
      bool m_null;
    };

    template<typename ValueType>
    inline void te::layout::Variant::setValue( ValueType value, EnumType* type )
    { 
      ValueType v = value;
      variantSetValue(*this, v, type);
    }

    /* Is still necessary to revise this method.*/
    template <typename ValueType>
    inline void te::layout::Variant::variantSetValue(Variant &v, 
      const ValueType& value, EnumType* type )
    {
      v = Variant(type, &value);      
    }
    
    inline bool te::layout::Variant::operator ==(const Variant& other) 
    { 
      Variant& otherProp = const_cast<Variant&>(other);

      if(getType() == otherProp.getType())
      {
        if(m_sValue == otherProp.toString() &&
          m_dValue == otherProp.toDouble() &&
          m_iValue == otherProp.toInt() &&
          m_lValue == otherProp.toLong() &&
          m_fValue == otherProp.toFloat() &&
          m_bValue == otherProp.toBool() &&
          m_colorValue == otherProp.toColor() /*&&
          m_fontValue == otherProp.toFont()*/)
        {
          return true;
        }
      }
      return false;
    }

    inline bool te::layout::Variant::operator !=(const Variant& other) 
    { 
      Variant& otherProp = const_cast<Variant&>(other);

      if(getType() != otherProp.getType())
      {
        return true;
      }

      if(getType() == otherProp.getType())
      {
        if(m_sValue != otherProp.toString() ||
          m_dValue != otherProp.toDouble() ||
          m_iValue != otherProp.toInt() ||
          m_lValue != otherProp.toLong() ||
          m_fValue != otherProp.toFloat() ||
          m_colorValue != otherProp.toColor() /*||
          m_fontValue != otherProp.toFont()*/)
        {
          return true;
        }
        else
        {
          if(m_bValue != otherProp.toBool())
          {
            return true;
          }
          else
          {
            return false;
          }
        }
      }
      return true;
    }
  }
}

#endif
