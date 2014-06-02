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
  \file terralib/maptools/QueryLayerRenderer.cpp

  \brief It renders the objects associated to a query layer.
*/

// TerraLib
#include "../common/Translator.h"
#include "../common/progress/TaskProgress.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/query/And.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/ST_Intersects.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "../se/CoverageStyle.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/ImageOutline.h"
#include "../se/RasterSymbolizer.h"
#include "../se/Rule.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "../srs/Converter.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "Exception.h"
#include "QueryEncoder.h"
#include "QueryLayer.h"
#include "QueryLayerRenderer.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>

te::map::QueryLayerRenderer::QueryLayerRenderer()
{
}

te::map::QueryLayerRenderer::~QueryLayerRenderer()
{
}

void te::map::QueryLayerRenderer::draw(AbstractLayer* layer,
                                       Canvas* canvas,
                                       const te::gm::Envelope& bbox,
                                       int srid)
{
// Is this a query layer?
  te::map::QueryLayer* qlayer = dynamic_cast<te::map::QueryLayer*>(layer);

  if(qlayer == 0)
    throw Exception(TE_TR("Wrong type render type for this layer!"));

// check if layer extent intersects the drawing area and so compute bounding box intersection
  te::gm::Envelope reprojectedBBOX(bbox);

  if((qlayer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, qlayer->getSRID());
  }
  else if(qlayer->getSRID() != srid)
  {
    throw Exception(TE_TR("The layer or map don't have a valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(qlayer->getExtent()))
    return;

  te::gm::Envelope ibbox = reprojectedBBOX.intersection(qlayer->getExtent());

// get dataset information
  std::auto_ptr<LayerSchema> schema(qlayer->getSchema());
  assert(schema.get());

  if(schema->hasGeom())
  {
    drawGeometries(qlayer, canvas, ibbox, srid);
  }
  else if(schema->hasRaster())
  {
    drawRaster(qlayer, canvas, ibbox, bbox, srid);
  }
  else
  {
    throw Exception(TE_TR("The layer don't have a geometry or raster property!"));
  }
}

void te::map::QueryLayerRenderer::drawGeometries(QueryLayer* layer,
                                                 Canvas* canvas,
                                                 const te::gm::Envelope& bbox, 
                                                 int srid)
{
  std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());
  assert(schema.get());

// verify if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (layer->getSRID() != srid))
    needRemap = true;

// for while, default geometry. TODO: need a visitor to get which properties the style references
  assert(schema->hasGeom());
  te::gm::GeometryProperty* geometryProperty = te::da::GetFirstGeomProperty(schema.get());

// get the associated layer style
  te::se::Style* style = layer->getStyle();
  if(style == 0)
  {
// try create an appropriate style
    style = te::se::CreateFeatureTypeStyle(geometryProperty->getGeometryType());

    if(style == 0)
      throw Exception((boost::format(TE_TR("Could not create a default feature type style to the layer %1%.")) % layer->getTitle()).str());

    layer->setStyle(style);
  }

// should I render this style?
  te::se::FeatureTypeStyle* fts = dynamic_cast<te::se::FeatureTypeStyle*>(style);
  if(fts == 0)
    throw Exception(TE_TR("The layer style is not a Feature Type Style!"));

// create a canvas configurer
  te::map::CanvasConfigurer cc(canvas);

// number of rules defined on feature type style
  std::size_t nRules = fts->getRules().size();

  for(std::size_t i = 0; i < nRules; ++i) // for each <Rule>
  {
// the current rule
    const te::se::Rule* rule = fts->getRule(i);
    assert(rule);

// TODO: should be verified the MinScaleDenominator and MaxScaleDenominator. Where will we put the current scale information? AbstractLayer? Method parameter?

// gets the rule filter
    const te::fe::Filter* filter = rule->getFilter();

// let's retrieve the correct dataset
    std::auto_ptr<te::da::DataSet> dataset(0);
    if(!filter)
    {
// there isn't a Filter expression. Gets the dataset using only box restriction...
      dataset = layer->getData(geometryProperty->getName(), &bbox, te::gm::INTERSECTS);
    }
    else
    {
      // TODO!
      throw Exception(TE_TR("No implemented yet!"));
    }

    if(dataset.get() == 0)
      throw Exception((boost::format(TE_TR("Could not retrieve the data set %1% referenced by the layer %2%.")) % schema->getName() % layer->getTitle()).str());

    if(dataset->moveNext() == false)
      continue;

// get the set of symbolizers defined on current rule
    const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();
    std::size_t nSymbolizers = symbolizers.size();

// build task message; e.g. ("Drawing the layer Countries. Rule 1 of 3.")
    std::string message = TE_TR("Drawing the layer");
    message += " " + layer->getTitle() + ". ";
    message += TE_TR("Rule");
    message += " " + boost::lexical_cast<std::string>(i + 1) + " " + TE_TR("of") + " ";
    message += boost::lexical_cast<std::string>(nRules) + ".";

// create a draw task
    te::common::TaskProgress task(message, te::common::TaskProgress::DRAW);
    //task.setTotalSteps(nSymbolizers * dataset->size()); // Removed! The te::da::DataSet size() method would be too costly to compute.

    for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
    {
// the current symbolizer
      te::se::Symbolizer* symb = symbolizers[j];

// let's config de canvas based on the current symbolizer
      cc.config(symb);

// for while, first geometry. TODO: get which property the symbolizer references
      std::size_t gpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);

// let's draw! for each data set object...
      do
      {
        if(!task.isActive())
          return;

// update the draw task
        task.pulse();

        std::auto_ptr<te::gm::Geometry> geom = dataset->getGeometry(gpos);
        if(geom.get() == 0)
          continue;

// if necessary, geometry remap
        if(needRemap)
        {
          geom->setSRID(layer->getSRID());
          geom->transform(srid);
        }

        canvas->draw(geom.get());

      }while(dataset->moveNext()); // next geometry!

// prepare to draw the other symbolizer
      dataset->moveFirst();

    } // end for each <Symbolizer>

  }   // end for each <Rule>
}

void te::map::QueryLayerRenderer::drawRaster(QueryLayer* layer,
                                             Canvas* canvas,
                                             const te::gm::Envelope& bbox, 
                                             const te::gm::Envelope& visibleArea, 
                                             int srid)
{
}
