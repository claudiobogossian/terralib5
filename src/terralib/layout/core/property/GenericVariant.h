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
  \file GenericVariant.h
   
  \brief Class acts like a vector for some C++/TerraLib5 data types. Responsible for storing the vector.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GENERIC_VARIANT_H 
#define __TERRALIB_LAYOUT_INTERNAL_GENERIC_VARIANT_H

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../Font.h"
#include "../enum/EnumType.h"
#include "../Config.h"
#include "../../../maptools/AbstractLayer.h"

// STL
#include <vector>
#include <string>

// Boost
#include <boost/any.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace std;

namespace te
{
  namespace layout
  {
    /*!
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT GenericVariant
    {
      public:

        /*!
          \brief Constructor
        */ 
        GenericVariant();

        /*!
          \brief Constructor
        */ 
        GenericVariant(EnumType* type);
        
        /*!
          \brief Destructor
        */ 
        virtual ~GenericVariant();

        /*!
          \brief Stores a copy of a vector.

          \param value copies the vector to be stored
          \param type data type
        */
        template<typename T>
        bool setVector(std::vector<T> value);

        /*!
          \brief Stores a copy of a vector.

          \param value copies the vector to be stored
          \param type data type
        */
        void setVector(std::vector<boost::any> value);
        
        /*!
          \brief Returns the value of string type. (The setValue method received a string)

          \return value of string type
        */
        std::vector<std::string> toString();

        /*!
          \brief Returns the value of double type. (The setValue method received a double)

          \return value of double type
        */
        std::vector<double> toDouble();

        /*!
          \brief Returns the value of int type. (The setValue method received a int)

          \return value of int type
        */
        std::vector<int> toInt();

        /*!
          \brief Returns the value of long type. (The setValue method received a long)

          \return value of long type
        */
        std::vector<long> toLong();

        /*!
          \brief Returns the value of float type. (The setValue method received a float)

          \return value of float type
        */
        std::vector<float> toFloat();

        /*!
          \brief Returns the value of boolean type. (The setValue method received a boolean)

          \return value of boolean type
        */
        std::vector<bool> toBool();

        /*!
          \brief Returns the value of te::color::RGBAColor type. (The setValue method received a te::color::RGBAColor). Complex type.

          \return value of te::color::RGBAColor type
        */
        std::vector<te::color::RGBAColor> toColor(); 

        /*!
          \brief Returns the value of te::layout::Font type. (The setValue method received a te::layout::Font). Complex type.

          \return value of te::layout::Font type
        */
        std::vector<Font> toFont();

        /*!
          \brief Returns the value of te::layout::Font type. (The setValue method received a te::layout::Font). Complex type.

          \return value of te::layout::Font type
        */
        std::vector<te::map::AbstractLayerPtr> toLayer();

        /*!
          \brief Returns the value of std::vector<boost::any> type. (The setValue method received a std::vector<boost::any>). Complex type.

          \return value of std::vector<boost::any> type
        */
        std::vector<boost::any> toAny();
        
        /*!
          \brief Converts the value to a string.

          \return Value as a string
        */
        virtual std::string convertToString();

        /*!
          \brief Returns true if no value has been set, false otherwise.

          \return true if no value has been set, false otherwise
        */
        bool isEmpty();

        /*!
          \brief Reset state of object. Null state.
        */
        virtual void clear();

        /*!
          \brief 
        */ 
        virtual void fromPtree(boost::property_tree::ptree tree);

    protected:
      
      /*!
          \brief Convert a vector values into string representation.

          \param vector
          \return string representation 
       */
      virtual std::string vectorToString();

      EnumType*                               m_type;

      std::vector<boost::any>                 m_data; //!< value of generic vector type
      std::vector<std::string>                m_vString; //!< value of string vector type
      std::vector<double>                     m_vDouble; //!< value of double vector type       
      std::vector<int>                        m_vInt; //!< value of int vector type
      std::vector<long>                       m_vLong; //!< value of long vector type
      std::vector<float>                      m_vFloat; //!< value of float vector type
      std::vector<bool>                       m_vBool; //!< value of bool vector type
      std::vector<te::color::RGBAColor>       m_vRGBColor; //!< value of te::color::RGBAColor vector type
      std::vector<te::layout::Font>           m_vFont; //!< value of te::layout::Font vector type
      std::vector<te::map::AbstractLayerPtr>  m_vLayer; //!< value of te::map::AbstractLayerPtr vector type
    };

    template<typename T>
    inline bool te::layout::GenericVariant::setVector( std::vector<T> value )
    {
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();

      if(!m_type || !dataType)
      {
        return;
      }

      bool result = true;

      if(m_type == dataType->getDataTypeString())
      {
        m_vString = value;
      }
      else if(m_type == dataType->getDataTypeDouble())
      {
        m_vDouble = value;
      }
      else if(m_type == dataType->getDataTypeInt())
      {
        m_vInt = value;
      }
      else if(m_type == dataType->getDataTypeLong())
      {
        m_vLong = value;
      }
      else if(m_type == dataType->getDataTypeFloat())
      {
        m_vFloat = value;
      }
      else if(m_type == dataType->getDataTypeBool())
      {
        m_vBool = value;
      }
      else if(m_type == dataType->getDataTypeColor())
      {
        m_vRGBColor = value;
      }
      else if(m_type == dataType->getDataTypeFont())
      {
        m_vFont = value;
      }
      else if(m_type == dataType->getDataTypeLayer())
      {
        m_vLayer = value;
      }
      else
      {
        result = false;
      }

      return result;
    }
  }
}

#endif



