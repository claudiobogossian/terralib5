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
  \file LayoutJSON.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_JSON_H 
#define __TERRALIB_LAYOUT_INTERNAL_JSON_H

//TerraLib
#include "LayoutSerialization.h"
#include "LayoutProperty.h"

// Boost
#include <boost/property_tree/ptree.hpp>

// STL
#include <utility>

namespace te
{
  namespace layout
  {
    class LayoutJSON : public LayoutSerialization
    {
      public:
        LayoutJSON();
        virtual ~LayoutJSON();

        template <typename ValueType>
        void add(std::string key, typename ValueType value);

        void addChild(LayoutJSON* json);

        template <typename ValueType>
        te::layout::LayoutJSON* addChild(std::string key, typename ValueType value);

        virtual int numberchilds();

        virtual std::vector<te::layout::LayoutJSON*> getChilds() const;

        virtual bool serialize();

        virtual boost::property_tree::ptree retrievePTree();

        virtual std::vector<te::layout::LayoutProperties*> retrieve();

        virtual bool isEmpty();

        virtual void loadFromPath( std::string loadPath );

        virtual void loadFromProperties( std::vector<te::layout::LayoutProperties*> properties );

      protected:

        boost::property_tree::ptree m_array;
        std::vector<LayoutJSON*> m_jsons;
        std::vector<std::string> m_keys;

        virtual void searchProperty(LayoutProperty property, LayoutJSON* json);

        virtual void searchPTree(boost::property_tree::ptree tree, LayoutJSON* json);

        virtual std::vector<std::string> getKeys();
    };

    template <typename ValueType>
    inline void te::layout::LayoutJSON::add( std::string key, typename ValueType value )
    {
      m_array.push_back(std::make_pair(key, value));  
      m_keys.push_back(key);         
    }

    template <typename ValueType>
    inline te::layout::LayoutJSON* addChild(std::string key, typename ValueType value) 
    {
      LayoutJSON* json = new LayoutJSON;
      json->add(key, value);    
      m_jsons.push_back(json);

      return json;
    }
  }
}

#endif