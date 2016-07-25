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
  \file terralib/maptools/AbstractLayerRenderer.cpp

  \brief It renders the objects associated to an abstract layer. i.e. a generic renderer.
*/

// TerraLib
#include "../color/RGBAColor.h"
#include "../common/progress/TaskProgress.h"
#include "../common/Globals.h"
#include "../common/STLUtils.h"
#include "../core/translator/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/query/And.h"
#include "../dataaccess/query/EqualTo.h"
#include "../dataaccess/query/GreaterThanOrEqualTo.h"
#include "../dataaccess/query/LessThanOrEqualTo.h"
#include "../dataaccess/query/LiteralDouble.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/LiteralString.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/ST_Intersects.h"
#include "../dataaccess/utils/Utils.h"
#include "../fe/Filter.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Line.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Polygon.h"
#include "../geometry/Utils.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "../se/AnchorPoint.h"
#include "../se/CoverageStyle.h"
#include "../se/Displacement.h"
#include "../se/Fill.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Halo.h"
#include "../se/LabelPlacement.h"
#include "../se/LinePlacement.h"
#include "../se/ParameterValue.h"
#include "../se/PointPlacement.h"
#include "../se/Rule.h"
#include "../se/TextSymbolizer.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "AbstractLayer.h"
#include "AbstractLayerRenderer.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "Chart.h"
#include "ChartRendererManager.h"
#include "Exception.h"
#include "Grouping.h"
#include "GroupingItem.h"
#include "QueryEncoder.h"
#include "Utils.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>
#include <cstdlib>
#include <memory>
#include <utility>



te::map::AbstractLayerRenderer::AbstractLayerRenderer()
  : m_index(0)
{
}

te::map::AbstractLayerRenderer::~AbstractLayerRenderer()
{
}

void te::map::AbstractLayerRenderer::draw(AbstractLayer* layer,
                                          Canvas* canvas,
                                          const te::gm::Envelope& bbox,
                                          int srid,
                                          const double& scale, bool* cancel)
{
  if(!bbox.isValid())
    throw Exception(TE_TR("The requested box is invalid!"));

  assert(layer);
  assert(canvas);

  // Check if layer extent intersects the drawing area and so compute bounding box intersection
  te::gm::Envelope reprojectedBBOX(bbox);

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, layer->getSRID());

    if(!reprojectedBBOX.isValid())
      throw Exception(TE_TR("The reprojected box is invalid!"));
  }
  else if(layer->getSRID() != srid)
  {
    throw Exception(TE_TR("The layer or map don't have a valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(layer->getExtent()))
    return;

  // Adjust internal renderer transformer
  m_transformer.setTransformationParameters(bbox.m_llx, bbox.m_lly, bbox.m_urx, bbox.m_ury, canvas->getWidth(), canvas->getHeight());

  canvas->setWindow(bbox.m_llx, bbox.m_lly, bbox.m_urx, bbox.m_ury);

  // Resets internal renderer state
  reset();

  te::gm::Envelope ibbox = reprojectedBBOX.intersection(layer->getExtent());

  assert(ibbox.isValid());

  // Gets the layer schema
  std::auto_ptr<LayerSchema> schema(layer->getSchema());
  assert(schema.get());

  // Gets the name of the referenced spatial property
  std::string spatialPropertyName = layer->getGeomPropertyName();

  if(schema->hasGeom())
  {
    te::gm::GeometryProperty* geometryProperty = 0;

    if(spatialPropertyName.empty())
      geometryProperty = te::da::GetFirstGeomProperty(schema.get());
    else
      geometryProperty = dynamic_cast<te::gm::GeometryProperty*>(schema->getProperty(spatialPropertyName));

    assert(geometryProperty);

    // If the AbstractLayer has a grouping, do not consider the style.
    Grouping* grouping = layer->getGrouping();
    if(grouping && grouping->isVisible())
    {
      drawLayerGroupingMem(layer, geometryProperty->getName(), canvas, ibbox, srid, scale, cancel);
      return;
    }

    // Gets the layer style
    te::se::Style* style = layer->getStyle();
    if(style == 0)
    {
      // Try create an appropriate style. i.e. a FeatureTypeStyle
      style = te::se::CreateFeatureTypeStyle(geometryProperty->getGeometryType());

      if(style == 0)
        throw Exception((boost::format(TE_TR("Could not create a default feature type style to the layer %1%.")) % layer->getTitle()).str());

      layer->setStyle(style);
    }

    // Should I render this style?
    te::se::FeatureTypeStyle* fts = dynamic_cast<te::se::FeatureTypeStyle*>(style);
    if(fts == 0)
      throw Exception(TE_TR("The layer style is not a Feature Type Style!"));

    drawLayerGeometries(layer, geometryProperty->getName(), fts, canvas, ibbox, srid, scale, cancel);
  }
  else if(schema->hasRaster())
  {
    te::rst::RasterProperty* rasterProperty = 0;

    if(spatialPropertyName.empty())
      rasterProperty = te::da::GetFirstRasterProperty(schema.get());
    else
      rasterProperty = dynamic_cast<te::rst::RasterProperty*>(schema->getProperty(spatialPropertyName));

    assert(rasterProperty);

    // Get the layer style
    te::se::Style* style = layer->getStyle();
    if(style == 0)
    {
      // Try create an appropriate style. i.e. a CoverageStyle
      style = te::se::CreateCoverageStyle(rasterProperty->getBandProperties());

      if(style == 0)
        throw Exception((boost::format(TE_TR("Could not create a default coverage style to the layer %1%.")) % layer->getTitle()).str());

      layer->setStyle(style);
    }

    // Should I render this style?
    te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
    if(cs == 0)
      throw Exception(TE_TR("The layer style is not a Coverage Style!"));

    // Retrieves the data
    std::auto_ptr<te::da::DataSet> dataset = layer->getData(rasterProperty->getName(), &ibbox, te::gm::INTERSECTS);

    if(dataset.get() == 0)
      throw Exception((boost::format(TE_TR("Could not retrieve the data set from the layer %1%.")) % layer->getTitle()).str());

    // Retrieves the raster
    std::auto_ptr<te::rst::Raster> raster(dataset->getRaster(rasterProperty->getName()));
    if(dataset.get() == 0)
      throw Exception((boost::format(TE_TR("Could not retrieve the raster from the layer %1%.")) % layer->getTitle()).str());

    // Let's draw!
    DrawRaster(raster.get(), canvas, ibbox, layer->getSRID(), bbox, srid, cs, scale);
  }
  else
  {
    throw Exception(TE_TR("The layer don't have a geometry or raster property!"));
  }
}

void te::map::AbstractLayerRenderer::drawLayerGeometries(AbstractLayer* layer,
                                                         const std::string& geomPropertyName,
                                                         te::se::FeatureTypeStyle* style,
                                                         Canvas* canvas,
                                                         const te::gm::Envelope& bbox,
                                                         int srid,
                                                         const double& scale, bool* cancel)
{
  assert(!geomPropertyName.empty());

  // Creates a canvas configurer
  CanvasConfigurer cc(canvas);

  // Number of rules defined on feature type style
  std::size_t nRules = style->getRules().size();

  for(std::size_t i = 0; i < nRules; ++i) // for each <Rule>
  {
    // The current rule
    te::se::Rule* rule = style->getRule(i);
    assert(rule);

    // TODO: Should be verified the MinScaleDenominator and MaxScaleDenominator. Where will we put the current scale information? Method parameter?

    if (!(scale >= rule->getMinScaleDenominator() && scale < rule->getMaxScaleDenominator()))
    {
      continue;
    }

    // Gets the rule filter
    const te::fe::Filter* filter = rule->getFilter();

    // Let's retrieve the correct dataset
    std::auto_ptr<te::da::DataSet> dataset;

    if(!filter)
    {
      try
      {
        // There isn't a Filter expression. Gets the data using only extent spatial restriction...
        dataset = layer->getData(geomPropertyName, &bbox, te::gm::INTERSECTS);
      }
      catch(Exception& e)
      {
        throw e;
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
          throw Exception(TE_TR("Could not convert the OGC Filter expression to TerraLib expression!"));

        /* 1) Creating the final restriction. i.e. Filter expression + extent spatial restriction */

        // The extent spatial restriction
        te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(bbox, layer->getSRID());
        te::da::PropertyName* geometryPropertyName = new te::da::PropertyName(geomPropertyName);
        te::da::ST_Intersects* intersects = new te::da::ST_Intersects(geometryPropertyName, lenv);

        // Combining the expressions (Filter expression + extent spatial restriction)
        te::da::And* restriction = new te::da::And(exp, intersects);

        /* 2) Calling the layer query method to get the correct restricted data. */
        dataset = layer->getData(restriction);
      }
      catch(std::exception& /*e*/)
      {
        throw Exception((boost::format(TE_TR("Could not retrieve the data set from the layer %1%.")) % layer->getTitle()).str());
      }
    }

    if(dataset.get() == 0)
      throw Exception((boost::format(TE_TR("Could not retrieve the data set from the layer %1%.")) % layer->getTitle()).str());

    if(dataset->moveNext() == false)
      continue;

    // Gets the set of symbolizers defined on current rule
    const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();

    // Builds task message; e.g. ("Drawing the layer Countries. Rule 1 of 3.")
    std::string message = TE_TR("Drawing the layer");
    message += " " + layer->getTitle() + ". ";
    message += TE_TR("Rule");
    message += " " + boost::lexical_cast<std::string>(i + 1) + " " + TE_TR("of") + " ";
    message += boost::lexical_cast<std::string>(nRules) + ".";

    // Creates a draw task
    te::common::TaskProgress task(message, te::common::TaskProgress::DRAW);
    //task.setTotalSteps(symbolizers.size() * dataset->size()); // Removed! The te::da::DataSet size() method would be too costly to compute.

    // For while, first geometry property. TODO: get which geometry property the symbolizer references
    std::size_t gpos = te::da::GetPropertyPos(dataset.get(), geomPropertyName);

    if(symbolizers.empty())
    {
      // The current rule do not have a symbolizer. Try creates a default based on first geometry of dataset.
      std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gpos));
      assert(g.get());

      te::se::Symbolizer* symbolizer = te::se::CreateSymbolizer(g->getGeomTypeId());
      assert(symbolizer);

      rule->push_back(symbolizer);

      dataset->moveFirst();
    }

    std::size_t nSymbolizers = symbolizers.size();

    for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
    {
      // The current symbolizer
      te::se::Symbolizer* symb = symbolizers[j];

      // Let's config the canvas based on the current symbolizer
      cc.config(symb);

      if (symb->getType() == "TextSymbolizer")
      {
        te::se::TextSymbolizer* ts = static_cast<te::se::TextSymbolizer*>(symb);

        drawDatSetTexts(dataset.get(), gpos, canvas, layer->getSRID(), srid, ts, cancel, &task);
      }
      else
      {
        // Let's draw! for each data set geometry...
        if (j != nSymbolizers - 1)
          drawDatSetGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, 0, cancel, &task);
        else
          drawDatSetGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, layer->getChart(), cancel, &task); // Here, produces the chart if exists
      }

      // Prepares to draw the other symbolizer
      dataset->moveFirst();

    } // end for each <Symbolizer>
  }   // end for each <Rule>
}

void te::map::AbstractLayerRenderer::drawLayerGrouping(AbstractLayer* layer,
                                                       const std::string& geomPropertyName,
                                                       Canvas* canvas,
                                                       const te::gm::Envelope& bbox,
                                                       int srid, bool* cancel)
{
  assert(!geomPropertyName.empty());

  // Creates a canvas configurer
  te::map::CanvasConfigurer cc(canvas);

  // The layer grouping
  Grouping* grouping = layer->getGrouping();

  // The referenced property name
  std::string propertyName = grouping->getPropertyName();
  assert(!propertyName.empty());

  // The referenced property type
  int propertyType = grouping->getPropertyType();

  // The grouping type
  GroupingType type = grouping->getType();

  // The grouping items
  const std::vector<GroupingItem*>& items = grouping->getGroupingItems();
  
  std::size_t nGroupItems = items.size();

  // Builds the task message; e.g. ("Drawing the grouping of layer Countries.")
  std::string message = TE_TR("Drawing the grouping of layer");
  message += " " + layer->getTitle() + ".";

  // Creates the draw task
  te::common::TaskProgress task(message, te::common::TaskProgress::DRAW, nGroupItems);

  for(std::size_t i = 0; i < nGroupItems; ++i) // for each GroupingItem
  {
    // The current group item
    GroupingItem* item = items[i];
    assert(item);

    /* 1) Creating te::da::Where object with the group item restriction expression + extent spatial restriction */

    te::da::PropertyName* groupingPropertyName = new te::da::PropertyName(propertyName);

    // Grouping item restriction
    te::da::Expression* exp = 0;

    if(type == UNIQUE_VALUE)
    {
      te::da::LiteralString* value = new te::da::LiteralString(item->getValue());
      exp = new te::da::EqualTo(groupingPropertyName, value);
    }
    else
    {
      te::da::Expression* lowerValue = 0;
      te::da::Expression* upperrValue = 0; 

      switch(propertyType)
      {
        case te::dt::STRING_TYPE:
          lowerValue = new te::da::LiteralString(item->getLowerLimit());
          upperrValue = new te::da::LiteralString(item->getUpperLimit());
        break;

        default:
          lowerValue = new te::da::LiteralDouble(boost::lexical_cast<double>(item->getLowerLimit()));
          upperrValue = new te::da::LiteralDouble(boost::lexical_cast<double>(item->getUpperLimit()));
      }

      te::da::GreaterThanOrEqualTo* gte = new te::da::GreaterThanOrEqualTo(groupingPropertyName, lowerValue);
      te::da::LessThanOrEqualTo* lte = new te::da::LessThanOrEqualTo(groupingPropertyName->clone(), upperrValue);

      exp = new te::da::And(gte, lte);
    }

    // The extent spatial restriction
    te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(bbox, layer->getSRID());
    te::da::PropertyName* geometryPropertyName = new te::da::PropertyName(geomPropertyName);
    te::da::ST_Intersects* intersects = new te::da::ST_Intersects(geometryPropertyName, lenv);

    // Combining the expressions (group item restriction expression + extent spatial restriction)
    te::da::And* restriction = new te::da::And(exp, intersects);

    /* 2) Calling the layer query method to get the correct restricted data. */

    std::auto_ptr<te::da::DataSet> dataset(0);
    try
    {
      dataset = layer->getData(restriction);
    }
    catch(std::exception& /*e*/)
    {
      continue; // TODO: deal the exceptions!
    }

    if(dataset.get() == 0)
      throw Exception((boost::format(TE_TR("Could not retrieve the data set from the layer %1%.")) % layer->getTitle()).str());

    if(dataset->moveNext() == false)
      continue;

    // Gets the set of symbolizers defined on group item
    const std::vector<te::se::Symbolizer*>& symbolizers = item->getSymbolizers();
    std::size_t nSymbolizers = symbolizers.size();

    // For while, first geometry property. TODO: get which geometry property the symbolizer references
    std::size_t gpos = te::da::GetPropertyPos(dataset.get(), geomPropertyName);

    for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
    {
      // The current symbolizer
      te::se::Symbolizer* symb = symbolizers[j];

      // Let's config the canvas based on the current symbolizer
      cc.config(symb);

      // Let's draw! for each data set geometry...
       if(j != nSymbolizers - 1)
        drawDatSetGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, 0, cancel, &task);
      else
        drawDatSetGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, layer->getChart(), cancel, &task); // Here, produces the chart if exists

      // Prepares to draw the other symbolizer
      dataset->moveFirst();

    } // end for each <Symbolizer>

    if(!task.isActive())
      return;

    task.pulse();

  } // end for each GroupItem
}

void te::map::AbstractLayerRenderer::drawLayerGroupingMem(AbstractLayer* layer,
                                                          const std::string& geomPropertyName,
                                                          Canvas* canvas,
                                                          const te::gm::Envelope& bbox,
                                                          int srid,
                                                          const double& scale, bool* cancel)
{
  assert(!geomPropertyName.empty());

  //check scale
  if (layer->getStyle())
  {
    if (!layer->getStyle()->getRules().empty())
    {
      // The current rule
      te::se::Rule* rule = layer->getStyle()->getRules()[0];

      if (!(scale >= rule->getMinScaleDenominator() && scale < rule->getMaxScaleDenominator()))
      {
        return;
      }
    }
  }

  // Creates a canvas configurer
  te::map::CanvasConfigurer cc(canvas);

  // The layer grouping
  Grouping* grouping = layer->getGrouping();

  // The referenced property name
  std::string propertyName = grouping->getPropertyName();
  assert(!propertyName.empty());

  // The grouping type
  GroupingType type = grouping->getType();

  // The grouping precision
  const std::size_t& precision = grouping->getPrecision();
  
  // The grouping items
  const std::vector<GroupingItem*>& items = grouping->getGroupingItems();

  std::size_t nGroupItems = items.size();

  // case UNIQUE_VALUE: for each GroupingItem, builds a map [item value] -> [symbolizers]
  std::map<std::string, std::vector<te::se::Symbolizer*> > uniqueGroupsMap;

  // case (NOT) UNIQUE_VALUE: for each GroupingItem, builds a map [item upper limit] -> [symbolizers]
  std::map<std::pair< double, double>, std::vector<te::se::Symbolizer*> > othersGroupsMap;

  for(std::size_t i = 0; i < nGroupItems; ++i)
  {
    // The current group item
    GroupingItem* item = items[i];
    assert(item);

    if(type == UNIQUE_VALUE)
    {
      uniqueGroupsMap[item->getValue()] = item->getSymbolizers();
    }
    else
    {
      double lowerLimit = atof(item->getLowerLimit().c_str());
      double upperLimit = atof(item->getUpperLimit().c_str());
      std::pair<double, double> range(lowerLimit, upperLimit);

      othersGroupsMap[range] = item->getSymbolizers();
    }
  }

  // Builds the task message; e.g. ("Drawing the grouping of layer Countries.")
  std::string message = TE_TR("Drawing the grouping of layer");
  message += " " + layer->getTitle() + ".";

  // Creates the draw task
  te::common::TaskProgress task(message, te::common::TaskProgress::DRAW);

  std::auto_ptr<te::da::DataSet> dataset(0);
  try
  {
    dataset = layer->getData(geomPropertyName, &bbox, te::gm::INTERSECTS);
  }
  catch(std::exception& /*e*/)
  {
    return; // TODO: deal the exceptions!
  }

  if(dataset.get() == 0)
    throw Exception((boost::format(TE_TR("Could not retrieve the data set from the layer %1%.")) % layer->getTitle()).str());

  if(dataset->moveNext() == false)
    return;

  // Gets the first geometry property
  std::size_t gpos = te::da::GetPropertyPos(dataset.get(), geomPropertyName);

  // Gets the property position
  std::auto_ptr<te::map::LayerSchema> dt(layer->getSchema());
  std::size_t propertyPos = te::da::GetPropertyPos(dt.get(), propertyName);

  // Verifies if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (layer->getSRID() != srid))
    needRemap = true;

  // The layer chart
  Chart* chart = layer->getChart();

  do
  {
    std::vector<te::se::Symbolizer*> symbolizers;

    // Finds the current data set item on group map
    std::string value;
    
    if(dataset->isNull(propertyPos))
      value = te::common::Globals::sm_nanStr;
    else
      value = dataset->getAsString(propertyPos, precision);

    if(type == UNIQUE_VALUE)
    {
      std::map<std::string, std::vector<te::se::Symbolizer*> >::const_iterator it = uniqueGroupsMap.find(value);
      if(it == uniqueGroupsMap.end())
        continue;
      symbolizers = it->second;
    }
    else
    {
      double dvalue = atof(value.c_str());
      std::map<std::pair< double, double>, std::vector<te::se::Symbolizer*> >::const_iterator it;
      for(it = othersGroupsMap.begin(); it != othersGroupsMap.end(); ++it)
      {
        if(dvalue >= it->first.first && dvalue <= it->first.second)
          break;
      }
      
      if(it == othersGroupsMap.end())
      {
        te::se::Style* style = layer->getStyle();
        if(style)
        {
          if(!style->getRules().empty())
          {
            te::se::Rule* rule = style->getRule(0);

            symbolizers = rule->getSymbolizers();
          }
        }
      }
      else
      {
        symbolizers = it->second;
      }

      if(symbolizers.empty())
        continue;
    }

    std::auto_ptr<te::gm::Geometry> geom;
    try
    {
      geom = dataset->getGeometry(gpos);
      if(geom.get() == 0)
        continue;
    }
    catch(std::exception& /*e*/)
    {
      continue;
    }

    // Gets the set of symbolizers defined on group item
    std::size_t nSymbolizers = symbolizers.size();

    for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
    {
      // The current symbolizer
      te::se::Symbolizer* symb = symbolizers[j];

      // Let's config the canvas based on the current symbolizer
      cc.config(symb);

      // If necessary, geometry remap
      if(needRemap)
      {
        geom->setSRID(layer->getSRID());
        geom->transform(srid);
      }

      canvas->draw(geom.get());

      if(chart && j == nSymbolizers - 1)
        buildChart(chart, dataset.get(), geom.get());
    }

    if(cancel != 0 && (*cancel))
      return;

  } while(dataset->moveNext());

  // Let's draw the generated charts
  for(std::size_t i = 0; i < m_chartCoordinates.size(); ++i)
  {
    canvas->drawImage(static_cast<int>(m_chartCoordinates[i].x),
                      static_cast<int>(m_chartCoordinates[i].y),
                      m_chartImages[i],
                      chart->getWidth(),
                      chart->getHeight());

    te::common::Free(m_chartImages[i], chart->getHeight());
  }
}

void te::map::AbstractLayerRenderer::drawDatSetGeometries(te::da::DataSet* dataset, const std::size_t& gpos, Canvas* canvas,
                                                          int fromSRID, int toSRID,
                                                          Chart* chart, bool* cancel, te::common::TaskProgress* task)
{
  assert(dataset);
  assert(canvas);

  // Verify if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if((fromSRID != TE_UNKNOWN_SRS) && (toSRID != TE_UNKNOWN_SRS) && (fromSRID != toSRID))
    needRemap = true;

  do
  {
    if(task)
    {
      if(!task->isActive())
      {
        *cancel = true;
        return;
      }

      // update the draw task
      task->pulse();
    }

    std::auto_ptr<te::gm::Geometry> geom(0);
    try
    {
      geom = dataset->getGeometry(gpos);
      if(geom.get() == 0)
        continue;
    }
    catch(std::exception& /*e*/)
    {
      continue;
    }

    // If necessary, geometry remap
    if(needRemap)
    {
      geom->setSRID(fromSRID);
      geom->transform(toSRID);
    }

    canvas->draw(geom.get());

    if(chart)
      buildChart(chart, dataset, geom.get());

    if(cancel != 0 && (*cancel))
      return;

  } while(dataset->moveNext()); // next geometry!

  // Let's draw the generated charts
  for(std::size_t i = 0; i < m_chartCoordinates.size(); ++i)
  {
    canvas->drawImage(static_cast<int>(m_chartCoordinates[i].x),
                      static_cast<int>(m_chartCoordinates[i].y),
                      m_chartImages[i],
                      chart->getWidth(),
                      chart->getHeight());

    te::common::Free(m_chartImages[i], chart->getHeight());
  }
}

void te::map::AbstractLayerRenderer::drawDatSetTexts(te::da::DataSet* dataset, const std::size_t& gpos, Canvas* canvas, int fromSRID, int toSRID, te::se::TextSymbolizer* symb, bool* cancel, te::common::TaskProgress* task)
{
  assert(dataset);
  assert(canvas);
  assert(symb);

  // Verify if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if ((fromSRID != TE_UNKNOWN_SRS) && (toSRID != TE_UNKNOWN_SRS) && (fromSRID != toSRID))
    needRemap = true;

  std::string propName = te::se::GetString(symb->getLabel());

  if (propName.empty())
    return;

  int propIdx = te::da::GetPropertyIndex(dataset, propName);

  //rtree for text boxes
  te::sam::rtree::Index<std::size_t, 4> rtree;

  do
  {
    if (task)
    {
      if (!task->isActive())
      {
        *cancel = true;
        return;
      }

      // update the draw task
      task->pulse();
    }

    std::auto_ptr<te::gm::Geometry> geom(0);
    try
    {
      geom = dataset->getGeometry(gpos);
      if (geom.get() == 0)
        continue;
    }
    catch (std::exception& /*e*/)
    {
      continue;
    }

    // If necessary, geometry remap
    if (needRemap)
    {
      geom->setSRID(fromSRID);
      geom->transform(toSRID);
    }

    std::string label = dataset->getString(propIdx);

    if (label == "")
      continue;

    //initial position
    te::gm::Coord2D cCenter = te::gm::GetCentroid(geom.get());
    double posX = cCenter.getX();
    double posY = cCenter.getY();

    //rotation
    float angle = 0.0;

    //displacement
    double anchorX = 0.5;
    double anchorY = 0.5;
    int displacementX = 0;
    int displacementY = 0;

    if (symb->getLabelPlacement())
    {
      if (symb->getLabelPlacement()->getPointPlacement())
      {
        const te::se::PointPlacement* pp = symb->getLabelPlacement()->getPointPlacement();

        if (pp->getRotation())
          angle = te::se::GetDouble(pp->getRotation());

        if (pp->getAnchorPoint())
        {
          anchorX = te::se::GetDouble(pp->getAnchorPoint()->getAnchorPointX());
          anchorY = te::se::GetDouble(pp->getAnchorPoint()->getAnchorPointY());
        }

        if (pp->getDisplacement())
        {
          displacementX = te::se::GetInt(pp->getDisplacement()->getDisplacementX());
          displacementY = te::se::GetInt(pp->getDisplacement()->getDisplacementY());
        }
      }

      if (symb->getLabelPlacement()->getLinePlacement())
      {
        //adjust centroid over the line
        if (geom->getGeomTypeId() == te::gm::GeomType::MultiLineStringType ||
            geom->getGeomTypeId() == te::gm::GeomType::LineStringType)
        {
          std::auto_ptr<te::gm::Point> point(new te::gm::Point(cCenter.x, cCenter.y, geom->getSRID()));
          te::gm::Coord2D coordGeom, coordPoint;
          te::gm::ClosestPoints(geom.get(), point.get(), coordGeom, coordPoint);

          posX = coordGeom.getX();
          posY = coordGeom.getY();
        }

        //adjust label over the line
        if (symb->getLabelPlacement()->getLinePlacement()->isAligned())
        {
          te::gm::Coord2D coord(posX, posY);
          std::auto_ptr<te::gm::Line> line(te::gm::GetIntersectionLine(geom.get(), coord));

          if (line.get())
          {
            te::gm::Coord2D cStart(line->getStartPoint()->getX(), line->getStartPoint()->getY());
            te::gm::Coord2D cEnd(line->getEndPoint()->getX(), line->getEndPoint()->getY());
            angle = te::gm::GetAngle(cStart, cEnd);
            angle = -angle;

            //set perpendicular distance
            if (symb->getLabelPlacement()->getLinePlacement()->getPerpendicularOffset())
            {
              double dist = te::se::GetDouble(symb->getLabelPlacement()->getLinePlacement()->getPerpendicularOffset());

              //rotate line
              std::auto_ptr<te::gm::LineString> lineRotate(new te::gm::LineString(2, te::gm::LineStringType, geom->getSRID()));
              te::gm::Coord2D centroidCoord(posX, posY);
              te::gm::Rotate(centroidCoord, line.get(), 90., lineRotate.get());
              
              //adjust segment
              std::auto_ptr<te::gm::Point> p0(dynamic_cast<te::gm::Point*>(lineRotate->intersection(line.get())));
              std::auto_ptr<te::gm::Point> p1(lineRotate->getPointN(1));
              te::gm::Coord2D c0, c1;
              te::gm::AdjustSegment(p0.get(), p1.get(), dist, c0, c1);

              //set new point to start drawing text
              posX = c1.getX();
              posY = c1.getY();
            }
          }
        }
      }
    }

    std::auto_ptr<te::gm::Polygon> poly(canvas->getTextBoundary(posX, posY, label, angle, anchorX, anchorY, displacementX, displacementY));

    te::gm::Envelope mbr(*poly->getMBR());

    //check rtree to avoid text overlay
    std::vector<std::size_t> report;
    rtree.search(mbr, report);

    if (report.empty())
    {
      canvas->drawText(posX, posY, label, angle, anchorX, anchorY, displacementX, displacementY);

      rtree.insert(mbr, 0);
    }

    if (cancel != 0 && (*cancel))
      return;

  } while (dataset->moveNext()); // next geometry!
}

void te::map::AbstractLayerRenderer::buildChart(Chart* chart, te::da::DataSet* dataset, te::gm::Geometry* geom)
{
  if(!chart->isVisible())
    return;

  // World coordinates
  std::auto_ptr<te::gm::Coord2D> worldCoord;

  // Try finds the geometry centroid
  switch(geom->getGeomTypeId())
  {
    case te::gm::PolygonType:
    {
      te::gm::Polygon* p = dynamic_cast<te::gm::Polygon*>(geom);
      worldCoord.reset(p->getCentroidCoord());
    }
    break;

    case te::gm::MultiPolygonType:
    {
      te::gm::MultiPolygon* mp = dynamic_cast<te::gm::MultiPolygon*>(geom);
      worldCoord.reset(mp->getCentroidCoord());
    }
    break;

    default:
      break;
  }

  // Case not find, use the center of the MBR
  if(worldCoord.get() == 0)
  {
    const te::gm::Envelope* e = geom->getMBR();
    worldCoord.reset(new te::gm::Coord2D(e->getCenter().x, e->getCenter().y));
  }

  // Device coordinates
  double dx = 0.0; double dy = 0.0;
  m_transformer.world2Device(worldCoord->x, worldCoord->y, dx, dy);

  double dw = dx + chart->getWidth();
  double dh = dy + chart->getHeight();

  // Builds the chart envelope
  te::gm::Envelope chartEnvelope(dx, dy, dw, dh);

  if(chart->getAvoidConflicts())
  {
    // Search on rtree
    std::vector<std::size_t> report;
    m_rtree.search(chartEnvelope, report);

    if(!report.empty())
      return;

    // Here, no intersections considering the current chart envelope
    m_rtree.insert(chartEnvelope, ++m_index);
  }

  // Stores the chart coordinate
  m_chartCoordinates.push_back(te::gm::Coord2D(dx, dy));

  // Builds the chart image
  std::size_t width = 0;
  te::color::RGBAColor** rgba = ChartRendererManager::getInstance().render(chart, dataset, width);
  m_chartImages.push_back(rgba);
}

void te::map::AbstractLayerRenderer::reset()
{
  m_index = 0;
  m_chartCoordinates.clear();
  m_chartImages.clear();
}
