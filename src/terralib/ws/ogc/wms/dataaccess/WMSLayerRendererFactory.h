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
  \file terralib/ws/ogc/wms/WMSLayerRendererFactory.h

  \brief This is the concrete factory for renderers of a WMSLayer.
*/

#ifndef __TERRALIB_WS_OGC_WMSLAYERRENDERERFACTORY_H
#define __TERRALIB_WS_OGC_WMSLAYERRENDERERFACTORY_H

#include "Config.h"

#include "../../../../maptools/RendererFactory.h"

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {
        /*!
          \class WMSLayerRendererFactory

          \brief This is the concrete factory for renderers of a WMSLayer.
        */
        class WMSLayerRendererFactory : public te::map::RendererFactory
        {
        public:

          ~WMSLayerRendererFactory();

        protected:

          te::map::AbstractRenderer* build();

          WMSLayerRendererFactory();

        private:

          static WMSLayerRendererFactory sm_factory; //!< A pointer to the global renderer factory.

        };

      }
    }
  }
}

#endif //__TERRALIB_WS_OGC_WMSLAYERRENDERERFACTORY_H
