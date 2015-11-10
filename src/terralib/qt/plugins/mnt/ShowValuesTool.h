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
\file terralib/mnt/core/ShowValueTool.h

\brief This file defines the Show MNT Values Tool
*/

#ifndef __TE_QT_PLUGINS_MNT_INTERNAL_SHOWVALUESTOOL_H
#define __TE_QT_PLUGINS_MNT_INTERNAL_SHOWVALUESTOOL_H

// TerraLib
#include "../../af/BaseApplication.h"
#include "../../../qt/widgets/tools/AbstractTool.h"

#include "Config.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace mnt
      {
        class ShowValuesTool : public te::qt::widgets::AbstractTool
        {

        public:

          ShowValuesTool(te::qt::widgets::MapDisplay* display, te::qt::af::BaseApplication* app, QObject* parent = 0);

          ~ShowValuesTool();

          /** @name AbstractTool Methods
          *  Methods related with tool behavior.
          */
          //@{

          bool mouseMoveEvent(QMouseEvent* e);

          //@}

        private:

          te::map::AbstractLayer* m_layer;
          te::qt::af::BaseApplication* m_app;
        };
      }
    }
  }
}

#endif //__TE_QT_PLUGINS_MNT_INTERNAL_SHOWVALUESTOOL_H
