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
  \file PropertiesUtils.h
   
  \brief Utility class with functions to facilitate handling of qt properties and properties layout module.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_UTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_UTILS_H

// Qt
#include <QObject>

// TerraLib
#include "../../../core/property/Property.h"
#include "../../../core/Config.h"

// STL
#include <string>
#include <vector>

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class Properties;

    /*!
	  \brief Utility class with functions to facilitate handling of qt properties and properties layout module.
	  
	  \ingroup layout
	  */
    class TELAYOUTEXPORT PropertiesUtils
    {
    public:

	    PropertiesUtils();

	    virtual ~PropertiesUtils();
      
      virtual Properties* intersection(QList<QGraphicsItem*> graphicsItems, bool& window);

      virtual Properties* sameProperties(QList<QGraphicsItem*> graphicsItems, bool& window);

      virtual void contains(std::vector<Properties*>::iterator itend, 
        std::vector<Properties*>::iterator it, std::string name, bool& result);

      virtual std::vector<Properties*> getAllProperties(QList<QGraphicsItem*> graphicsItems, bool& window);
      
      virtual void addDynamicOptions(Property& property, std::vector<std::string> list);

      virtual void checkDynamicProperty(Property& property, QList<QGraphicsItem*> graphicsItems);

      virtual void mapNameDynamicProperty(Property& property, QList<QGraphicsItem*> graphicsItems);
    };
  }
}

#endif






