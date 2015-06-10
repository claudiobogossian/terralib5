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
  \file terralib/maptools/RasterLayerRenderer.h

  \brief It renders the objects associated to a raster layer.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H

// TerraLib
#include "AbstractRenderer.h"

namespace te
{
  namespace map
  {
// Forward declarations
    class RasterLayer;

    /*!
      \class RasterLayerRenderer

      \brief It renders the objects associated to a raster layer.

      \sa AbstractLayer, AbstractRenderer, Canvas, te::se::Style
    */
    class TEMAPEXPORT RasterLayerRenderer : public AbstractRenderer
    {
      public:

        /*! Constructor */
        RasterLayerRenderer();

        /*! \brief Destructor. */
        ~RasterLayerRenderer();

        void draw(AbstractLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, int srid);
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_RASTERLAYERRENDERER_H

