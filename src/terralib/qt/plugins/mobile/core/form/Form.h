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
\file src/terralib/qt/plugins/mobile/core/form/Form.h

\brief This file defines the form class.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_FORM_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_FORM_H

// TerraLib
#include "../../Config.h"
#include "AbstractFormItem.h"

// Boost
#include <boost/property_tree/ptree.hpp>

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

        /*!
        \class Form

        \brief This class defines the Form object.
        */

        class Form
        {
        public:

          /* \brief Default Constructor */
          Form();

          /* \brief Default Destructor*/
          ~Form();

        public:

          void serialize(boost::property_tree::ptree& pt);

          std::string getName() { return m_name; }

          void setName(std::string name) { m_name = name; }

          std::vector<AbstractFormItem*>& getItems() { return m_items; }

        protected:

          std::string m_name;

          std::vector<te::qt::plugins::terramobile::AbstractFormItem*> m_items;
        };

      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te


#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_FORM_H