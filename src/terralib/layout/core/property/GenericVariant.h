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
#include "../enum/Enums.h"
#include "../Config.h"
#include "../../../maptools/AbstractLayer.h"

// STL
#include <vector>
#include <string>
#include <list>

// Boost
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
          \brief Destructor
        */ 
        virtual ~GenericVariant();

        /*!
          \brief Stores a copy of a vector.

          \param value copies the vector to be stored
          \param type data type
        */
        template<typename T>
        bool setVector(std::vector<T> value, EnumType* type);

        /*!
          \brief Stores a copy of a vector.

          \param value copies the vector to be stored
          \param type data type
        */
        template<typename T>
        bool setList(std::list<T> value, EnumType* type);
                
        /*!
          \brief Returns the value of string vector type. (The setValue method received a string vector)

          \return value of string type
        */
        std::vector<std::string> toStringVector();

        /*!
          \brief Returns the value of layer list type. (The setValue method received a list layer)

          \return value of string type
        */
        const std::list<te::map::AbstractLayerPtr>& toLayerList() const;
        
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

      /*!
          \brief Convert a list values into string representation.

          \param vector
          \return string representation 
       */
      virtual std::string listToString();

      EnumType*                               m_type;

      std::vector<std::string>                m_vString; //!< value of string vector type
      std::list<te::map::AbstractLayerPtr>    m_listLayer; //!< value of te::map::AbstractLayerPtr list type
    };

    template<typename T>
    inline bool te::layout::GenericVariant::setVector( std::vector<T> value, EnumType* type )
    {
      bool result = true;

      EnumDataType* dataType = Enums::getInstance().getEnumDataType();
      if(!dataType)
      {
        return false;
      }
      
      m_type = type;

      if(!type || !dataType)
      {
        return false;
      }

      if(m_type == dataType->getDataTypeStringVector())
      {
        m_vString = value;
      }
      else
      {
        result = false;
      }

      return result;
    }

    template<typename T>
    inline bool te::layout::GenericVariant::setList( std::list<T> value, EnumType* type )
    {
      bool result = true;

      EnumDataType* dataType = Enums::getInstance().getEnumDataType();
      if(!dataType)
      {
        return false;
      }

      m_type = type;

      if(!type || !dataType)
      {
        return false;
      }

      if(m_type == dataType->getDataTypeLayerList())
      {
        m_listLayer = value;
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



