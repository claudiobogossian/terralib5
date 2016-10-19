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
  \file terralib/ws/ogc/wms/serialization/xml/Layer.h

  \brief Auxiliary classes and functions to read/write WMS layers using XML document.
*/

#ifndef __TERRALIB_WS_OGC_WMSLAYER_SERIALIZATION_XML_H
#define __TERRALIB_WS_OGC_WMSLAYER_SERIALIZATION_XML_H

#include "../../Config.h"

// TerraLib
#include "../../../../../../maptools/serialization/xml/Layer.h"
#include "../../../client/DataTypes.h"

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {
        namespace serialize
        {

          TEOGCWMSDATAACCESSEXPORT te::map::AbstractLayer *LayerReader(te::xml::Reader& reader);

          TEOGCWMSDATAACCESSEXPORT void LayerWriter(const te::map::AbstractLayer* layer, te::xml::AbstractWriter& writer);

          TEOGCWMSDATAACCESSEXPORT te::ws::ogc::wms::WMSGetMapRequest GetMapRequestReader(te::xml::Reader& reader);

        }
      }
    }
  }
}

#endif // __TERRALIB_WS_OGC_WMSLAYER_SERIALIZATION_XML_H
