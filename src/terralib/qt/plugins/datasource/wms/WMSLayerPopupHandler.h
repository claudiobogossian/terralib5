/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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

#ifndef __TERRALIB_QT_PLUGINS_DATASOURCE_WMS_INTERNAL_ITEMLAYERPOPUPHANDLER_H
#define __TERRALIB_QT_PLUGINS_DATASOURCE_WMS_INTERNAL_ITEMLAYERPOPUPHANDLER_H

// TerraLib
#include "../../../../common/Decorator.h"

// Qt
#include <QObject>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace wms
      {
        class WMSLayerPopupHandler : public te::common::Decorator<QObject>
        {

        public:

          WMSLayerPopupHandler(QObject* decorated, QObject* parent = 0);

          ~WMSLayerPopupHandler();

          bool eventFilter(QObject* watched, QEvent* event);
        };
      }
    }
  }
}

#endif //__TERRALIB_QT_PLUGINS_DATASOURCE_WMS_INTERNAL_ITEMLAYERPOPUPHANDLER_H
