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
 * \file terralib/qt/af/XMLFormatter.h
 *
 * \brief A class for xml serialization formatting strings.
 */

#ifndef TE_TERRAVIEW_XMLFORMATTER_H
#define TE_TERRAVIEW_XMLFORMATTER_H

#include "Config.h"

#include <terralib/maptools/AbstractLayer.h>

//STL
#include <list>
#include <string>

namespace te {

  //Forward declarations
  namespace da
  {
    class DataSourceInfo;
  }
}

struct ProjectMetadata;

/*!
         \class XMLFormatter

         \brief A class that formats strings.

         Use this class for changing strings, contained in some TerraLib objects, to XML percentage encode format and/or to human readable format. Use XML formatting before serializing the object to a XML file, for example, and human readable format after read the object from a XML.

         \ingroup af
       */
class XMLFormatter
{
public:

  /*!
            \brief Formats the project informations.

            \param p The project.

            \param encode \a Pass true to change for XML percentage format and \a false for human readable format.
           */
  static void format(ProjectMetadata* p, const std::list<te::map::AbstractLayerPtr>& layers, const bool& encode);

};

#endif // TE_TERRAVIEW_XMLFORMATTER_H
