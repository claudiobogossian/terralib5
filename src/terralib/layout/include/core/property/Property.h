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
  \file Property.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTY_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTY_H

// TerraLib
#include "Variant.h"

namespace te
{
  namespace layout
  {
    class Property
    {
      public:

        Property();
        virtual ~Property();

        std::string getName();
        void setName(std::string name); 
        
        LayoutPropertyDataType getType();

	     std::vector<te::layout::Property> getSubProperty();
        
        std::string getId();
        void setId(std::string id);

        template <typename ValueType>
        void setValue(typename ValueType value, LayoutPropertyDataType type);

        Variant getValue();

        bool isEditable();
        void setEditable(bool editable);

        /*Property Options*/

        void addOption(Variant variant);
        void removeOption(Variant variant);
        void setOptionChoice(Variant variant);
        void setOptionChoice(int numberOption);
        Variant getOptionByCurrentChoice();        
        std::vector<Variant> getOptionChoices();  

        //Ex.: For Grid Map
        void addDependentProperty( Property property );
        void removeDependentProperty( Property property );

        //Ex.: For font information
        void addSubProperty( Property property );
        void removeSubProperty( Property property );

    protected:

      std::string m_name;
      LayoutPropertyDataType m_type;
      std::string m_id;
      Variant m_value;
      Variant m_currentChoice;
      bool m_editable;
      std::vector<Variant> m_variants;
      std::vector<te::layout::Property> m_dependentProperty;
      std::vector<te::layout::Property> m_subProperty;
    };

    template <typename ValueType>
    inline void te::layout::Property::setValue(typename ValueType value, LayoutPropertyDataType type)
    {
      //Use the information type?
      Variant v;
      v.setValue(value, type);
      m_value = v;
      m_type = type;
    }
  }
}

#endif