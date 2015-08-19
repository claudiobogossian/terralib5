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
  \file Variant.h
   
  \brief Class acts like a union for some C++/TerraLib5 data types. Responsible for storing the value.
  Any data type, not included in the convertValue method in this class, it will be by default "std::string".
  Storing value types:

    - std::string 
    - double
    - int 
    - long 
    - float 
    - bool 
    - te::color::RGBAColor 
    - te::layout::Font 
    - te::layout::GenericVariant

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VARIANT_H 
#define __TERRALIB_LAYOUT_INTERNAL_VARIANT_H

// TerraLib
#include "../enum/AbstractType.h"
#include "../../../color/RGBAColor.h"
#include "../Font.h"
#include "../enum/EnumType.h"
#include "../Config.h"
#include "GenericVariant.h"
#include "../../../geometry/Geometry.h"

// STL
#include <string>
#include <map>
#include <vector>
#include <iostream>

// Boost
#include <boost/property_tree/ptree.hpp>

using namespace std;

namespace te
{
  namespace layout
  {
    /*!
	    \brief Class acts like a union for some C++/TerraLib5 data types. Responsible for storing the value.
       Any data type, not included in the convertValue method in this class, it will be by default "std::string".
       Storing value types:

        - std::string 
        - double
        - int 
        - long 
        - float 
        - bool 
        - te::color::RGBAColor 
        - te::layout::Font 
        - te::layout::GenericVariant
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT Variant
    {
      public:

        /*!
          \brief Constructor
        */ 
        Variant();

        /*!
          \brief Constructor

          \type data type
          \valueCopy value pointer (real data)
        */
        Variant(EnumType* type, const void* valueCopy);

        /*!
          \brief Destructor
        */ 
        virtual ~Variant();
        
        /*!
          \brief Stores a copy of value.

          \param value copies the value to be stored
          \param type data type
        */
        template <typename ValueType>
        void setValue(ValueType value, EnumType* type);

        /*!
          \brief 
        */ 
        virtual void fromPtree(boost::property_tree::ptree tree, EnumType* type);

        /*!
          \brief Returns data type of this object.
        */ 
        EnumType* getType() const;

        /*!
          \brief Return true if value is not of common C++ data type, false otherwise.

          \param true if value is not of common C++ data type, false otherwise
        */ 
        virtual bool isComplex() const;
        
        /*!
          \brief Returns the value of string type. (The setValue method received a string)

          \return value of string type
        */
        const std::string& toString() const;

        /*!
          \brief Returns the value of double type. (The setValue method received a double)

          \return value of double type
        */
        double toDouble() const;

        /*!
          \brief Returns the value of int type. (The setValue method received a int)

          \return value of int type
        */
        int toInt() const;

        /*!
          \brief Returns the value of long type. (The setValue method received a long)

          \return value of long type
        */
        long toLong() const;

        /*!
          \brief Returns the value of float type. (The setValue method received a float)

          \return value of float type
        */
        float toFloat() const;

        /*!
          \brief Returns the value of boolean type. (The setValue method received a boolean)

          \return value of boolean type
        */
        bool toBool() const;

        /*!
          \brief Returns the value of te::color::RGBAColor type. (The setValue method received a te::color::RGBAColor). Complex type.

          \return value of te::color::RGBAColor type
        */
        const te::color::RGBAColor& toColor() const;

        /*!
          \brief Returns the value of te::layout::Font type. (The setValue method received a te::layout::Font). Complex type.

          \return value of te::layout::Font type
        */
        const Font& toFont() const;

        /*!
          \brief Returns the value of te::layout::Font type. (The setValue method received a te::layout::Font). Complex type.

          \return value of te::layout::Font type
        */
        const te::gm::Envelope& toEnvelope() const;

        /*!
          \brief Returns the value of te::layout::GenericVariant type. (The setValue method received a te::layout::GenericVariant). Complex type.

          \return value of te::layout::Font type
        */
        const GenericVariant& toGenericVariant() const;

        /*!
          \brief Returns the shared pointer of te::gm::Geometry type. (The setValue method received a shared pointer of te::gm::Geometry).

          \return shared poiniter of te::gm::Geometry type
        */
        const te::gm::GeometryShrPtr toGeometry() const;

        /*!
          \brief Converts the value to a string.

          \return Value as a string
        */
        virtual std::string convertToString() const;

        /*!
          \brief Returns true if no value has been set, false otherwise.

          \return true if no value has been set, false otherwise
        */
        bool isNull() const;

        /*!
          \brief Reset state of object. Null state.
        */
        virtual void clear();
                
        bool operator ==(const Variant& other) const;
        bool operator !=(const Variant& other) const;

    protected:

      /*!
          \brief Stores a copy of value.

          \param v this object
          \param value copies the value to be stored
          \param type data type
        */
      template <typename ValueType>
      void variantSetValue(Variant &v, const ValueType& value, EnumType* type);
      
      /*!
          \brief Discovers the type of the value and sets for the corresponding attribute (storage).
          Any type of data, not included in this class, it will be by default "std::string"

          \param valueCopy pointer of the value to be stored 
       */
      virtual void convertValue(const void* valueCopy);
      
      /*!
          \brief Convert a string representation of a number into a double value.

          \param str string representation of a number
          \return double value
       */
      virtual double string2Double(std::string str);

      /*!
          \brief Convert a string representation of a number into a int value.

          \param str string representation of a number
          \return int value
       */
      virtual int string2Int(std::string str);

      /*!
          \brief Convert a string representation of a number into a float value.

          \param str string representation of a number
          \return float value
       */
      virtual float string2Float(std::string str);

      /*!
          \brief Convert a string representation of a number into a long value.

          \param str string representation of a number
          \return long value
       */
      virtual long string2Long(std::string str);

      /*!
          \brief Convert a int value into a string representation of a number.

          \param value int value
          \return string representation of a number
       */
      virtual std::string toString(int value) const;

      /*!
          \brief Convert a string value into a boolean representation of a string. Ex.: true, false.

          \param str string value
          \return boolean representation of a string. Ex.: true, false
       */
      virtual bool toBool(std::string str);

      std::string m_sValue; //!< value of string type
      double m_dValue; //!< value of double type
      int m_iValue; //!< value of int type
      long m_lValue; //!< value of long type
      float m_fValue; //!< value of float type
      bool m_bValue; //!< value of boolean type 
      te::color::RGBAColor m_colorValue; //!< value of te::color::RGBAColor type
      Font m_fontValue; //!< value of te::layout::Font type
      te::gm::Envelope m_envelopeValue; //!< value of the envelope type
      EnumType* m_type; //!< data type of this object
      bool m_null; //!< true if no value has been set, false otherwise
      bool m_complex; //!< true if value is not of common C++ data type, false otherwise
      GenericVariant m_generic; //!< value of te::layout::GenericVariant type
      te::gm::GeometryShrPtr m_geometryPtr; //!< shared pointer of te::gm::Geometry type
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
    
    inline bool te::layout::Variant::operator ==(const Variant& other) const
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
          m_colorValue == otherProp.toColor()/*&&
          m_fontValue == otherProp.toFont()*/)
        {
          return true;
        }
      }
      return false;
    }

    inline bool te::layout::Variant::operator !=(const Variant& other) const
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
