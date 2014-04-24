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
#include "AbstractType.h"

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
        Variant(LayoutPropertyDataType type, const void* valueCopy);
        virtual ~Variant();
        
        template <typename ValueType>
        void setValue(typename ValueType value, LayoutPropertyDataType type);

        LayoutPropertyDataType getType();
        
        std::string toString();
        double toDouble();
        int toInt();
        long toLong();
        float toFloat();
        bool toBool();

        bool isNull();

        void clear();

        bool operator ==(const Variant& other); 

    protected:

      template <typename ValueType>
      void variantSetValue(Variant &v, const typename ValueType& value, LayoutPropertyDataType type);
      
      void convertValue(const void* valueCopy);

      std::string m_sValue;
      double m_dValue;
      int m_iValue;
      long m_lValue;
      float m_fValue;
      bool m_bValue;
      LayoutPropertyDataType m_type;
      bool m_null;
    };

    template<typename ValueType>
    inline void te::layout::Variant::setValue( typename ValueType value, LayoutPropertyDataType type )
    { 
      variantSetValue(*this, value, type);
    }

    /* Is still necessary to revise this method.*/
    template <typename ValueType>
    inline void te::layout::Variant::variantSetValue(Variant &v, 
      const typename ValueType& value, LayoutPropertyDataType type )
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
          m_iValue == toInt() &&
          m_lValue == toLong() &&
          m_fValue == toFloat() &&
          m_bValue == toBool())
        {
          return true;
        }
      }
      return false;
    }
  }
}

#endif