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
\file src/terralib/qt/plugins/mobile/core/form/AbstractFormItem.h

\brief This file defines the abstract class for a form item.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_ABSTRACTITEM_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_ABSTRACTITEM_H

#define BOOLFORMITEMTYPE "boolean"
#define DATEFORMITEMTYPE "date"
#define DOUBLEFORMITEMTYPE "double"
#define INTFORMITEMTYPE "integer"
#define LABELFORMITEMTYPE "label"
#define PICTUREFORMITEMTYPE "pictures"
#define STRINGCOMBOFORMITEMTYPE "stringcombo"
#define STRINGFORMITEMTYPE "string"
#define TIMEFORMITEMTYPE "time"

// TerraLib
#include "../../Config.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/property_tree/ptree.hpp>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {
        /*!
        \class AbstractFormItem

        \brief This file defines the abstract class for a form item.
        */

        class AbstractFormItem
        {
        public:

          /* \brief Default Constructor */
          AbstractFormItem();

          /* \brief Default Destructor*/
          virtual ~AbstractFormItem();

        public:

          std::string getKey() { return m_key; }

          void setKey(std::string key) { m_key = key; }

          std::string getLabel() { return m_label; }

          void setLabel(std::string label) { m_label = label; }

          std::string getType() { return m_type; }

          bool hasDefaultValue() { return m_hasDefaultValue; }

          std::string getStrDefaultValue();

          std::vector<std::string> getValues();

        protected:

          virtual void toString() = 0;

        protected:

          std::string m_key;
          std::string m_label;
          std::string m_type;

          std::string m_defaultValue;
          std::vector<std::string> m_values;

          bool m_hasDefaultValue;
          
        };

      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te


#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_ABSTRACTITEM_H
