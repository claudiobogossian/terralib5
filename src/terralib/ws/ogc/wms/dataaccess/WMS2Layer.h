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
  \file terralib/ws/ogc/wms/dataaccess/WMS2Layer.h

  \brief A functor that converts a dataset from WMS server to a TerraLib OGC WMSLayer.
*/

#ifndef __TERRALIB_WS_OGC_WMS2LAYER_H
#define __TERRALIB_WS_OGC_WMS2LAYER_H



#include "Config.h"

//TerraLib
#include "WMSLayer.h"

// STL
#include <string>
namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {

        struct TEOGCWMSDATAACCESSEXPORT WMS2Layer
        {
          std::string m_dataSourceId;

          WMS2Layer(const std::string dataSourceId);

          WMSLayerPtr operator()(const te::ws::ogc::wms::WMSGetMapRequest& request) const;
        };

      }
    }
  }
}


#endif //__TERRALIB_WS_OGC_WMS2LAYER_H
