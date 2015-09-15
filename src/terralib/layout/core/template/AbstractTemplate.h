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
  \file AbstractTemplate.h
   
  \brief Abstract class that represents a Template.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TEMPLATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TEMPLATE_H

// TerraLib
#include "../Config.h"

// STL
#include <vector>
#include <string>

namespace te
{
  namespace layout
  {
    class Properties;
    class EnumType;

    /*!
      \brief Abstract class that represents a Template.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AbstractTemplate
    {
      public:

        AbstractTemplate(std::string path);

        virtual ~AbstractTemplate();

        virtual bool exportTemplate(std::vector<te::layout::Properties> properties) = 0;

        virtual std::vector<te::layout::Properties> importTemplate() = 0;

        virtual bool deleteTemplate() = 0;

        virtual std::string getFileName();

        virtual EnumType* getType();

      protected:
        std::string m_path;
        std::string m_fileName;
        std::string m_version;
        EnumType* m_type;

    };
  }
}

#endif
