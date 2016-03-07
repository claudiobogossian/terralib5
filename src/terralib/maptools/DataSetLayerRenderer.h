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
  \file terralib/maptools/DataSetLayerRenderer.h

  \brief It renders the objects associated to a dataset layer.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_DATASETLAYERRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_DATASETLAYERRENDERER_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "AbstractRenderer.h"

namespace te
{
// Forward declarations
  namespace da { class DataSourceTransactor; }

  namespace map
  {
// Forward declarations
    class DataSetLayer;

    /*!
      \class DataSetLayerRenderer

      \brief It renders the objects associated to a dataset layer.

      \sa AbstractLayer, AbstractRenderer, QueryLayerRenderer, Canvas, te::se::Style

      \todo Tratar o caso de ter raster e geometry como atributos do mesmo dataset!

      \todo Tratar o caso de ter multiplos raster no mesmo dataset!
    */
    class TEMAPEXPORT DataSetLayerRenderer : public AbstractRenderer
    {
      public:

        /*! Constructor */
        DataSetLayerRenderer();

        /*! \brief Destructor. */
        ~DataSetLayerRenderer();

        void draw(AbstractLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, int srid, const double& scale, bool* cancel);

      private:

        void drawGrouping(DataSetLayer* layer, te::da::DataSourcePtr ds, Canvas* canvas, const te::gm::Envelope& bbox, int srid);
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_DATASETLAYERRENDERER_H

