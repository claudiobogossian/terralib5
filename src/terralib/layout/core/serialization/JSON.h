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
  \file JSON.h
   
   \brief Implementation of .json for Serialization. It is a JSON file. Save or change a file .json and serializes the properties of the objects, MVC component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_JSON_H 
#define __TERRALIB_LAYOUT_INTERNAL_JSON_H

//TerraLib
#include "Serialization.h"
#include "../property/Property.h"
#include "../Config.h"

// Boost
#include <boost/property_tree/ptree.hpp>

// STL
#include <utility>

namespace te
{
  namespace layout
  {
    /*!
      \brief Implementation of .json for Serialization. It is a JSON file. Save or change a file .json and serializes the properties of the objects, MVC component.
	  
	    \ingroup layout

      \sa te::layout::Serialization
	  */
    class TELAYOUTEXPORT JSON : public Serialization
    {
      public:

        JSON();
        
        virtual ~JSON();
        
        virtual bool serialize();

        virtual boost::property_tree::ptree retrievePTree();

        virtual std::vector<te::layout::Properties*> retrieve();

        virtual bool isEmpty();

        virtual void loadFromPath( std::string loadPath );

        virtual void loadFromProperties( std::vector<te::layout::Properties*> properties );

      protected:

        virtual void searchProperty(Property& property, boost::property_tree::ptree& array, boost::property_tree::ptree& child);

        virtual void retrieveSubPTree(boost::property_tree::ptree subTree, Property& prop);

        boost::property_tree::ptree m_array;
    };
  }
}

#endif



