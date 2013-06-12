/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/QueryLayerRenderer.h

  \brief It renders the objects associated to a query layer.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_QUERYLAYERRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_QUERYLAYERRENDERER_H

// TerraLib
#include "AbstractRenderer.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
    class DataSourceTransactor;
  }

  namespace map
  {
    class QueryLayer;

    /*!
      \class QueryLayerRenderer

      \brief It renders the objects associated to a query layer.

      \sa AbstractLayer, AbstractRenderer, CoverageStyleRenderer, FeatureTypeStyleRenderer, DataSetLayerRenderer, Canvas, te::se::Style
    */
    class TEMAPEXPORT QueryLayerRenderer : public AbstractRenderer
    {
      public:

        /*! Constructor */
        QueryLayerRenderer();

        /*! \brief Destructor. */
        ~QueryLayerRenderer();

        void draw(AbstractLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, int srid);

      private:

        void drawGeometries(std::auto_ptr<QueryLayer> layer, 
                            std::auto_ptr<te::da::DataSourceTransactor> transactor, 
                            Canvas* canvas, 
                            const te::gm::Envelope& bbox, 
                            int srid);

        void drawRaster(std::auto_ptr<QueryLayer> layer, 
                        std::auto_ptr<te::da::DataSourceTransactor> transactor, 
                        Canvas* canvas, 
                        const te::gm::Envelope& bbox, 
                        const te::gm::Envelope& visibleArea, 
                        int srid);
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_QUERYLAYERRENDERER_H

