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
\file src/terralib/qt/plugins/mobile/core/form/IntFormItem.h

\brief This file defines the class for a int form item.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_INTFORMITEM_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_INTFORMITEM_H

// TerraLib
#include "../../Config.h"
#include "AbstractFormItem.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {

        /*!
        \class BoolFormItem

        \brief This file defines the class for a bool form item.
        */

        class IntFormItem : public AbstractFormItem
        {
        public:

          /* \brief Default Constructor */
          IntFormItem();

          /* \brief Default Destructor*/
          ~IntFormItem();

        public:

          void setValue(int value);

          int getValue();

        protected:

          virtual void toString();

        protected:

          int m_value;
        };

      }   // end namespace thirdParty
    }     // end namespace plugins
  }       // end namespace qt
}         // end namespace te


#endif  // __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_INTFORMITEM_H
