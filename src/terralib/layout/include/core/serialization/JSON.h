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
  \file JSON.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_JSON_H 
#define __TERRALIB_LAYOUT_INTERNAL_JSON_H

//TerraLib
#include "Serialization.h"
#include "Property.h"

// Boost
#include <boost/property_tree/ptree.hpp>

// STL
#include <utility>

namespace te
{
  namespace layout
  {
    class JSON : public Serialization
    {
      public:
        JSON();
        virtual ~JSON();

        template <typename ValueType>
        void add(std::string key, typename ValueType value);

        void addChild(JSON* json);

        template <typename ValueType>
        te::layout::JSON* addChild(std::string key, typename ValueType value);

        virtual int numberchilds();

        virtual std::vector<te::layout::JSON*> getChilds() const;

        virtual bool serialize();

        virtual boost::property_tree::ptree retrievePTree();

        virtual std::vector<te::layout::Properties*> retrieve();

        virtual bool isEmpty();

        virtual void loadFromPath( std::string loadPath );

        virtual void loadFromProperties( std::vector<te::layout::Properties*> properties );

      protected:

        boost::property_tree::ptree m_array;
        std::vector<JSON*> m_jsons;
        std::vector<std::string> m_keys;

        virtual void searchProperty(Property property, JSON* json);

        virtual void searchPTree(boost::property_tree::ptree tree, JSON* json);

        virtual std::vector<std::string> getKeys();
    };

    template <typename ValueType>
    inline void te::layout::JSON::add( std::string key, typename ValueType value )
    {
      m_array.push_back(std::make_pair(key, value));  
      m_keys.push_back(key);         
    }

    template <typename ValueType>
    inline te::layout::JSON* addChild(std::string key, typename ValueType value) 
    {
      JSON* json = new JSON;
      json->add(key, value);    
      m_jsons.push_back(json);

      return json;
    }
  }
}

#endif