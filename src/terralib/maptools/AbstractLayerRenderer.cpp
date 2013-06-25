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
  \file terralib/maptools/AbstractLayerRenderer.cpp

  \brief It renders the objects associated to an abstract layer. i.e. a generic renderer.
*/

// TerraLib
#include "../common/progress/TaskProgress.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/query/And.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/ST_Intersects.h"
#include "../dataaccess/utils/Utils.h"
#include "../fe/Filter.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/RasterProperty.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/CoverageStyle.h"
#include "../se/Rule.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "AbstractLayer.h"
#include "AbstractLayerRenderer.h"
#include "CanvasConfigurer.h"
#include "Exception.h"
#include "QueryEncoder.h"
#include "Utils.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>
#include <memory>

te::map::AbstractLayerRenderer::AbstractLayerRenderer()
{
}

te::map::AbstractLayerRenderer::~AbstractLayerRenderer()
{
}

void te::map::AbstractLayerRenderer::draw(AbstractLayer* layer,
                                          Canvas* canvas,
                                          const te::gm::Envelope& bbox,
                                          int srid)
{
  assert(layer);
  assert(canvas);
  assert(bbox.isValid());

  // Check if layer extent intersects the drawing area and so compute bounding box intersection
  te::gm::Envelope reprojectedBBOX(bbox);

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, layer->getSRID());
  }
  else if(layer->getSRID() != srid)
  {
    throw Exception(TR_MAP("The layer or map don't have a valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(layer->getExtent()))
    return;

  te::gm::Envelope ibbox = reprojectedBBOX.intersection(layer->getExtent());

  // Gets the layer schema
  std::auto_ptr<const LayerSchema> schema(layer->getSchema(true));
  assert(schema.get());

  if(schema->hasGeom())
  {
    // For while, first geometry property. TODO: need a visitor to get which spatial properties the layer style references
    te::gm::GeometryProperty* geometryProperty = te::da::GetFirstGeomProperty(schema.get());

    // Gets the layer style
    te::se::Style* style = layer->getStyle();
    if(style == 0)
    {
      // Try create an appropriate style. i.e. a FeatureTypeStyle
      style = te::se::CreateFeatureTypeStyle(geometryProperty->getGeometryType());

      if(style == 0)
        throw Exception((boost::format(TR_MAP("Could not create a default feature type style to the layer %1%.")) % layer->getTitle()).str());

      layer->setStyle(style);
    }

    // Should I render this style?
    te::se::FeatureTypeStyle* fts = dynamic_cast<te::se::FeatureTypeStyle*>(style);
    if(fts == 0)
      throw Exception(TR_MAP("The layer style is not a Feature Type Style!"));

    drawLayerGeometries(layer, geometryProperty->getName(), fts, canvas, ibbox, srid);
  }
  else if(schema->hasRaster())
  {
    // For while, first raster property. TODO: need a visitor to get which spatial properties the layer style references
    te::rst::RasterProperty* rasterProperty = te::da::GetFirstRasterProperty(schema.get());

    // Get the layer style
    te::se::Style* style = layer->getStyle();
    if(style == 0)
    {
      // Try create an appropriate style. i.e. a CoverageStyle
      style = te::se::CreateCoverageStyle(rasterProperty->getBandProperties());

      if(style == 0)
        throw Exception((boost::format(TR_MAP("Could not create a default coverage style to the layer %1%.")) % layer->getTitle()).str());

      layer->setStyle(style);
    }

    // Should I render this style?
    te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
    if(cs == 0)
      throw Exception(TR_MAP("The layer style is not a Coverage Style!"));

    // TODO!
  }
  else
  {
    throw Exception(TR_MAP("The layer don't have a geometry or raster property!"));
  }
}

void te::map::AbstractLayerRenderer::drawLayerGeometries(AbstractLayer* layer,
                                                         const std::string& geomPropertyName,
                                                         te::se::FeatureTypeStyle* style,
                                                         Canvas* canvas,
                                                         const te::gm::Envelope& bbox,
                                                         int srid)
{
  assert(!geomPropertyName.empty());

  // Creates a canvas configurer
  CanvasConfigurer cc(canvas);

  // Number of rules defined on feature type style
  std::size_t nRules = style->getRules().size();

  for(std::size_t i = 0; i < nRules; ++i) // for each <Rule>
  {
    // The current rule
    const te::se::Rule* rule = style->getRule(i);
    assert(rule);

    // TODO: Should be verified the MinScaleDenominator and MaxScaleDenominator. Where will we put the current scale information? Method parameter?

    // Gets the rule filter
    const te::fe::Filter* filter = rule->getFilter();

    // Let's retrieve the correct dataset
    std::auto_ptr<te::da::DataSet> dataset(0);

    if(!filter)
    {
      try
      {
        // There isn't a Filter expression. Gets the data using only extent spatial restriction...
        dataset.reset(layer->getData(bbox, te::gm::INTERSECTS));
      }
      catch(std::exception& /*e*/)
      {
        continue; // TODO: deal the exceptions!
      }
    }
    else
    {
      try
      {
        // Gets an enconder
        te::map::QueryEncoder filter2Query;

        // Converts the Filter expression to a TerraLib Expression!
        te::da::Expression* exp = filter2Query.getExpression(filter);
        if(exp == 0)
          throw Exception(TR_MAP("Could not convert the OGC Filter expression to TerraLib expression!"));

        /* 1) Creating the final restriction. i.e. Filter expression + extent spatial restriction */

        // The extent spatial restriction
        te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(bbox, srid);
        te::da::PropertyName* geometryPropertyName = new te::da::PropertyName(geomPropertyName);
        te::da::ST_Intersects* intersects = new te::da::ST_Intersects(geometryPropertyName, lenv);

        // Combining the expressions (Filter expression + extent restriction)
        te::da::And* restriction = new te::da::And(exp, intersects);

        /* 2) Calling the layer query method to get the correct restricted data. */
        dataset.reset(layer->getData(restriction));
      }
      catch(std::exception& /*e*/)
      {
        continue; // TODO: deal the exceptions!
      }
    }

    if(dataset.get() == 0)
      throw Exception((boost::format(TR_MAP("Could not retrieve the data set from the layer %1%.")) % layer->getTitle()).str());

    if(dataset->moveNext() == false)
      continue;

    // Gets the set of symbolizers defined on current rule
    const std::vector<te::se::Symbolizer*> symbolizers = rule->getSymbolizers();
    std::size_t nSymbolizers = symbolizers.size();

    // Build task message; e.g. ("Drawing the layer Countries. Rule 1 of 3.")
    std::string message = TR_MAP("Drawing the layer");
    message += " " + layer->getTitle() + ". ";
    message += TR_MAP("Rule");
    message += " " + boost::lexical_cast<std::string>(i + 1) + " " + TR_MAP("of") + " ";
    message += boost::lexical_cast<std::string>(nRules) + ".";

    // Creates a draw task
    te::common::TaskProgress task(message, te::common::TaskProgress::DRAW);
    //task.setTotalSteps(nSymbolizers * dataset->size()); // Removed! The te::da::DataSet size() method would be too costly to compute.

    for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
    {
      // The current symbolizer
      te::se::Symbolizer* symb = symbolizers[j];

      // Let's config the canvas based on the current symbolizer
      cc.config(symb);

      // For while, first geometry property. TODO: get which geometry property the symbolizer references
      std::size_t gpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);

      // Let's draw! for each data set geometry...
      DrawGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, &task);

      // Prepares to draw the other symbolizer
      dataset->moveBeforeFirst();

    } // end for each <Symbolizer>
  }   // end for each <Rule>
}

void te::map::AbstractLayerRenderer::drawRaster(AbstractLayer* layer,
                                                te::se::CoverageStyle* style,
                                                Canvas* canvas,
                                                const te::gm::Envelope& bbox,
                                                const te::gm::Envelope& visibleArea,
                                                int srid)
{
}
