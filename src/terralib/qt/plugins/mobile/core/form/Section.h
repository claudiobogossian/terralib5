/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
\file src/terralib/qt/plugins/mobile/core/form/Section.h

\brief This file defines the section class.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_SECTION_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_SECTION_H

// TerraLib
#include "../../Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {
        //forward declaration
        class Form;

        /*!
        \class Section

        \brief This class defines the Section object.
        */

        class Section
        {
          public:

            /* \brief Default Constructor */
            Section();

            /* \brief Default Destructor*/
            ~Section();

          public:

            void serialize(std::string fileName);

            std::string getName() { return m_name;  }

            void setName(std::string name) { m_name = name; }

            std::string getDescription() { return m_description; }

            void setDescription(std::string desc) { m_description = desc; }

            std::vector<Form*>& getForms() { return m_forms; }

          protected:

            std::string m_name;
            std::string m_description;

            std::vector<Form*> m_forms;
        };

      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te


#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_SECTION_H