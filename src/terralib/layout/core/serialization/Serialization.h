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
  \file Serialization.h
   
  \brief Abstract class for serialization properties in any file extension.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SERIALIZATION_H 
#define __TERRALIB_LAYOUT_INTERNAL_SERIALIZATION_H

// TerraLib
#include "../property/Properties.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief Abstract class for serialization properties in any file extension.
	  
	    \ingroup layout
	  */
    class Serialization
    {
      public:

        Serialization();

        virtual ~Serialization();

        virtual void loadFromPath(std::string loadPath) = 0;

        virtual void loadFromProperties(std::vector<te::layout::Properties*> properties) = 0;

        virtual void setSerializationPath(std::string path);

        virtual std::string getSerializationPath();

        virtual std::string getLoadPath();

        virtual bool serialize() = 0;

        virtual std::vector<te::layout::Properties*> retrieve() = 0;

        virtual bool isEmpty() = 0;

        virtual void setRootKey(std::string rootKey);

        virtual std::string getRootKey();
        
       protected:

         std::string m_loadPath;
         std::string m_serializationPath;
         std::vector<te::layout::Properties*> m_properties;
         std::string m_rootKey;
    };
  }
}

#endif