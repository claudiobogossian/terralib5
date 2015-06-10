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
  \file terralib/maptools/QueryLayerRenderer.cpp

  \brief It renders the objects associated to a query layer.
*/

// TerraLib
#include "../color/RGBAColor.h"
#include "../common/progress/TaskProgress.h"
#include "../common/Globals.h"
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
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
#include "../geometry/MultiPolygon.h"
#include "../geometry/Polygon.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/CoverageStyle.h"
#include "../se/Rule.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "AbstractLayer.h"
#include "QueryLayerRenderer.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "Chart.h"
#include "ChartRendererManager.h"
#include "Exception.h"
#include "Grouping.h"
#include "GroupingItem.h"
#include "QueryEncoder.h"
#include "Utils.h"
#include "QueryLayer.h"
#include "../dataaccess/query/OrderBy.h"
#include "../dataaccess/query/OrderByItem.h"
#include "../dataaccess/query/Select.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/round.hpp>

// STL
#include <cassert>
#include <cstdlib>
#include <memory>
#include <utility>

te::map::QueryLayerRenderer::QueryLayerRenderer()
  : m_index(0)
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
      drawLayerGroupingMem(layer, geometryProperty->getName(), canvas, ibbox, srid);
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

    drawLayerGeometries(layer, geometryProperty->getName(), fts, canvas, ibbox, srid);
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
    DrawRaster(raster.get(), canvas, ibbox, layer->getSRID(), bbox, srid, cs);
  }
  else
  {
    throw Exception(TE_TR("The layer don't have a geometry or raster property!"));
  }
}

void te::map::QueryLayerRenderer::drawLayerGeometries(AbstractLayer* layer,
                                                         const std::string& geomPropertyName,
                                                         te::se::FeatureTypeStyle* style,
                                                         Canvas* canvas,
                                                         const te::gm::Envelope& bbox,
                                                         int srid)
{
  assert(!geomPropertyName.empty());
  m_oid.clear();
  te::map::QueryLayer* qlayer = 0;
  te::da::Select* select = 0;

  bool linked = te::da::HasLinkedTable(layer->getSchema().get());
  if(linked)
  {
    // make sorting by object id
    qlayer = dynamic_cast<te::map::QueryLayer*>(layer);
    select = dynamic_cast<te::da::Select*>(qlayer->getQuery()->clone());
    te::da::Select* selectaux = dynamic_cast<te::da::Select*>(select->clone());
    te::da::OrderBy* orderBy = new te::da::OrderBy;

    std::auto_ptr<te::da::DataSetType> schema = layer->getSchema();
    std::vector<te::dt::Property*> props = schema->getPrimaryKey()->getProperties();
    size_t pksize = 0;
    while(++pksize < props.size())
    {
      m_oid.push_back(props[pksize-1]->getName());
      if(props[pksize-1]->getDatasetName() != props[pksize]->getDatasetName())
        break;
    }

    for(size_t i = 0; i < pksize; ++i)
      orderBy->push_back(new te::da::OrderByItem(m_oid[i]));

    selectaux->setOrderBy(orderBy);
    qlayer->setQuery(selectaux);
  }

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

    // Gets the rule filter
    const te::fe::Filter* filter = rule->getFilter();

    // Let's retrieve the correct dataset
    std::auto_ptr<te::da::DataSet> dataset(0);

    if(!filter)
    {
      try
      {
        // There isn't a Filter expression. Gets the data using only extent spatial restriction...
        dataset = layer->getData(geomPropertyName, &bbox, te::gm::INTERSECTS);
        if(linked)
          qlayer->setQuery(select);
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
        if(linked)
          qlayer->setQuery(select);
      }
      catch(std::exception& /*e*/)
      {
        continue; // TODO: deal the exceptions!
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

      // Let's draw! for each data set geometry...
      if(j != nSymbolizers - 1)
        drawDatSetGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, 0, &task);
      else
        drawDatSetGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, layer->getChart(), &task); // Here, produces the chart if exists

      // Prepares to draw the other symbolizer
      dataset->moveFirst();

    } // end for each <Symbolizer>

  }   // end for each <Rule>
}

void te::map::QueryLayerRenderer::drawLayerGrouping(AbstractLayer* layer,
                                                       const std::string& geomPropertyName,
                                                       Canvas* canvas,
                                                       const te::gm::Envelope& bbox,
                                                       int srid)
{
  //assert(!geomPropertyName.empty());

  //// Creates a canvas configurer
  //te::map::CanvasConfigurer cc(canvas);

  //// The layer grouping
  //Grouping* grouping = layer->getGrouping();

  //// The referenced property name
  //std::string propertyName = grouping->getPropertyName();
  //assert(!propertyName.empty());

  //// The referenced property type
  //int propertyType = grouping->getPropertyType();

  //// The grouping type
  //GroupingType type = grouping->getType();

  //// The grouping items
  //const std::vector<GroupingItem*>& items = grouping->getGroupingItems();
  //
  //std::size_t nGroupItems = items.size();

  //// Builds the task message; e.g. ("Drawing the grouping of layer Countries.")
  //std::string message = TE_TR("Drawing the grouping of layer");
  //message += " " + layer->getTitle() + ".";

  //// Creates the draw task
  //te::common::TaskProgress task(message, te::common::TaskProgress::DRAW, nGroupItems);

  //for(std::size_t i = 0; i < nGroupItems; ++i) // for each GroupingItem
  //{
  //  // The current group item
  //  GroupingItem* item = items[i];
  //  assert(item);

  //  /* 1) Creating te::da::Where object with the group item restriction expression + extent spatial restriction */

  //  te::da::PropertyName* groupingPropertyName = new te::da::PropertyName(propertyName);

  //  // Grouping item restriction
  //  te::da::Expression* exp = 0;

  //  if(type == UNIQUE_VALUE)
  //  {
  //    te::da::LiteralString* value = new te::da::LiteralString(item->getValue());
  //    exp = new te::da::EqualTo(groupingPropertyName, value);
  //  }
  //  else
  //  {
  //    te::da::Expression* lowerValue = 0;
  //    te::da::Expression* upperrValue = 0; 

  //    switch(propertyType)
  //    {
  //      case te::dt::STRING_TYPE:
  //        lowerValue = new te::da::LiteralString(item->getLowerLimit());
  //        upperrValue = new te::da::LiteralString(item->getUpperLimit());
  //      break;

  //      default:
  //        lowerValue = new te::da::LiteralDouble(boost::lexical_cast<double>(item->getLowerLimit()));
  //        upperrValue = new te::da::LiteralDouble(boost::lexical_cast<double>(item->getUpperLimit()));
  //    }

  //    te::da::GreaterThanOrEqualTo* gte = new te::da::GreaterThanOrEqualTo(groupingPropertyName, lowerValue);
  //    te::da::LessThanOrEqualTo* lte = new te::da::LessThanOrEqualTo(groupingPropertyName->clone(), upperrValue);

  //    exp = new te::da::And(gte, lte);
  //  }

  //  // The extent spatial restriction
  //  te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(bbox, layer->getSRID());
  //  te::da::PropertyName* geometryPropertyName = new te::da::PropertyName(geomPropertyName);
  //  te::da::ST_Intersects* intersects = new te::da::ST_Intersects(geometryPropertyName, lenv);

  //  // Combining the expressions (group item restriction expression + extent spatial restriction)
  //  te::da::And* restriction = new te::da::And(exp, intersects);

  //  /* 2) Calling the layer query method to get the correct restricted data. */

  //  std::auto_ptr<te::da::DataSet> dataset(0);
  //  try
  //  {
  //    dataset = layer->getData(restriction);
  //  }
  //  catch(std::exception& /*e*/)
  //  {
  //    continue; // TODO: deal the exceptions!
  //  }

  //  if(dataset.get() == 0)
  //    throw Exception((boost::format(TE_TR("Could not retrieve the data set from the layer %1%.")) % layer->getTitle()).str());

  //  if(dataset->moveNext() == false)
  //    continue;

  //  // Gets the set of symbolizers defined on group item
  //  const std::vector<te::se::Symbolizer*>& symbolizers = item->getSymbolizers();
  //  std::size_t nSymbolizers = symbolizers.size();

  //  // For while, first geometry property. TODO: get which geometry property the symbolizer references
  //  std::size_t gpos = te::da::GetPropertyPos(dataset.get(), geomPropertyName);

  //  for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
  //  {
  //    // The current symbolizer
  //    te::se::Symbolizer* symb = symbolizers[j];

  //    // Let's config the canvas based on the current symbolizer
  //    cc.config(symb);

  //    // Let's draw! for each data set geometry...
  //     if(j != nSymbolizers - 1)
  //      drawDatSetGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, 0, &task);
  //    else
  //      drawDatSetGeometries(dataset.get(), gpos, canvas, layer->getSRID(), srid, layer->getChart(), &task); // Here, produces the chart if exists

  //    // Prepares to draw the other symbolizer
  //    dataset->moveFirst();

  //  } // end for each <Symbolizer>

  //  if(!task.isActive())
  //    return;

  //  task.pulse();

  //} // end for each GroupItem
}

void te::map::QueryLayerRenderer::drawLayerGroupingMem(AbstractLayer* layer,
                                                          const std::string& geomPropertyName,
                                                          Canvas* canvas,
                                                          const te::gm::Envelope& bbox,
                                                          int srid)
{
  assert(!geomPropertyName.empty());

  if(te::da::HasLinkedTable(layer->getSchema().get()))
  {
    drawLayerLinkedGroupingMem(layer, geomPropertyName, canvas, bbox, srid);
    return;
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

    if(dataset->isNull(propertyPos) == false)
    {
      // Finds the current data set item on group map

      if(type == UNIQUE_VALUE)
      {
        std::string svalue = dataset->getAsString(propertyPos, precision);
        std::map<std::string, std::vector<te::se::Symbolizer*> >::const_iterator it = uniqueGroupsMap.find(svalue);
        if(it == uniqueGroupsMap.end())
          continue;
        symbolizers = it->second;
      }
      else
      {
        double dvalue = te::da::GetValueAsDouble(dataset.get(), propertyPos);
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
    }
    else
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

void te::map::QueryLayerRenderer::drawLayerLinkedGroupingMem(AbstractLayer* layer,
                                                          const std::string& geomPropertyName,
                                                          Canvas* canvas,
                                                          const te::gm::Envelope& bbox,
                                                          int srid)
{
  assert(!geomPropertyName.empty());

  m_oid.clear();

  te::map::QueryLayer* qlayer = dynamic_cast<te::map::QueryLayer*>(layer);
  te::da::Select* select = dynamic_cast<te::da::Select*>(qlayer->getQuery()->clone());

  std::auto_ptr<te::da::DataSetType> schema = layer->getSchema();
  std::vector<te::dt::Property*> props = schema->getPrimaryKey()->getProperties();
  size_t pksize = 0;
  while(++pksize < props.size())
  {
    m_oid.push_back(props[pksize-1]->getName());
    if(props[pksize-1]->getDatasetName() != props[pksize]->getDatasetName())
      break;
  }

  te::da::OrderBy* orderBy = new te::da::OrderBy;
  for(size_t i = 0; i < pksize; ++i)
    orderBy->push_back(new te::da::OrderByItem(m_oid[i]));

  te::da::Select* selectaux = dynamic_cast<te::da::Select*>(select->clone());
  selectaux->setOrderBy(orderBy);
  qlayer->setQuery(selectaux);

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

  // The grouping sumarization
  const std::string gfunction = grouping->getSummary();
  
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
    qlayer->setQuery(select);
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
  size_t ptype = dataset->getPropertyDataType(te::da::GetPropertyPos(dataset.get(), propertyName));
 
    // Verifies if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (layer->getSRID() != srid))
    needRemap = true;

  std::vector<std::string> pkdata(pksize), pkdataaux(pksize);
  std::vector<double> values;
  double value;
  std::vector<std::string> svalues;
  std::string svalue;

  // The layer chart
  Chart* chart = layer->getChart();

  // The chart sumarization
  std::string cfunction;
  std::map<std::string, std::vector<double> > chartValues;
  std::map<std::string, double> chartValue;
  bool hasGroupNullValue = false;
  bool hasGroupNullValueAux = false;
  bool hasChartNullValue = false;
  bool hasChartNullValueAux = false;
  size_t csize = 0;
  if(chart)
  {
    cfunction = chart->getSummary();
    csize = chart->getProperties().size();
    std::vector<size_t> propPos;
    for(std::size_t i = 0; i < csize; ++i)
    {
      std::vector<double> v;
      chartValues[chart->getProperties()[i]] = v;
      propPos.push_back(te::da::GetPropertyPos(dataset.get(), chart->getProperties()[i]));
    }
    chart->setPropertiesPos(propPos);
  }

  te::gm::Geometry  *geom = 0, *geomaux = 0;
  do
  {
    try
    {
      if(geomaux == 0)
        geomaux = dataset->getGeometry(gpos).release();
      if(geomaux == 0)
        continue;
    }
    catch(std::exception& /*e*/)
    {
      continue;
    }

    std::vector<te::se::Symbolizer*> symbolizers;

    // Finds the current data set item on group map   
    size_t i;
    for(i = 0; i < pksize; ++i)
      pkdata[i] = dataset->getAsString(m_oid[i]);
      
    for(i = 0; i < pksize; ++i)
    {
      if(dataset->isAtBegin())
        pkdataaux[i] = dataset->getAsString(m_oid[i]);
      else
      {
        if(pkdata[i] != pkdataaux[i])
        {
          pkdataaux = pkdata;
          break;
        }
      }      
    }
    if(i == pksize) // it is the same object
    {
      // read value
      if(hasGroupNullValue == false && dataset->isNull(propertyPos) == false)
      {
        if(type == UNIQUE_VALUE)
        {
          if(ptype == te::dt::STRING_TYPE)
            svalues.push_back(dataset->getAsString(propertyPos, precision));
          else
            values.push_back(te::da::GetValueAsDouble(dataset.get(), propertyPos));
        }
        else
          values.push_back(te::da::GetValueAsDouble(dataset.get(), propertyPos));
      }
      else
        hasGroupNullValue = true;

      if(hasChartNullValue == false)
      {
        for(std::size_t i = 0; i < csize; ++i)
        {
          if(dataset->isNull(chart->getProperties()[i]))
          {
            hasChartNullValue = true;
            break;
          }
        }
        if(hasChartNullValue == false)
        {
          for(std::size_t i = 0; i < csize; ++i)
            chartValues[chart->getProperties()[i]].push_back(te::da::GetValueAsDouble(dataset.get(), chart->getPropertiesPos()[i]));
        }
      }

      // read other values
      continue;
    }
    else // it is other object
    {
      delete geom;
      geom = geomaux;

      // sumarize value
      // computing a value according to the required summarization 
      // get symbolizers
      if(hasGroupNullValue == false)
      {
        if(type == UNIQUE_VALUE)
        {
          if(ptype == te::dt::STRING_TYPE)
          {
            value = te::da::GetSummarizedValue(values, gfunction);
            value = te::da::Round(value, precision);
            svalue = boost::lexical_cast<std::string>(value);
          }
          else
            svalue = te::da::GetSummarizedValue(svalues, gfunction);

          std::map<std::string, std::vector<te::se::Symbolizer*> >::const_iterator it = uniqueGroupsMap.find(svalue);
          if(it == uniqueGroupsMap.end())
            continue;
          symbolizers = it->second;
        }
        else
        {
          value = te::da::GetSummarizedValue(values, gfunction);
          value = te::da::Round(value, precision);

          std::map<std::pair<double, double>, std::vector<te::se::Symbolizer*> >::const_iterator it;
          for(it = othersGroupsMap.begin(); it != othersGroupsMap.end(); ++it)
          {
            if(value >= it->first.first && value <= it->first.second)
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
            symbolizers = it->second;
        }
      }
      else
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

      if(hasChartNullValue == false)
      {
        for(std::size_t i = 0; i < csize; ++i)
          chartValue[chart->getProperties()[i]] = te::da::GetSummarizedValue(chartValues[chart->getProperties()[i]], cfunction);
      }

      // store the values of the other object (for next loop).
      try
      {
        geomaux = dataset->getGeometry(gpos).release();
      }
      catch(std::exception& /*e*/)
      {
        geomaux = 0;
        continue;
      }

      for(std::size_t i = 0; i < csize; ++i)
        chartValues[chart->getProperties()[i]].clear();
      values.clear();
      svalues.clear();

      if(dataset->isNull(propertyPos) == false)
      {
        if(type == UNIQUE_VALUE)
        {
          if(ptype == te::dt::STRING_TYPE)
            svalues.push_back(dataset->getAsString(propertyPos, precision));
          else
            values.push_back(te::da::GetValueAsDouble(dataset.get(), propertyPos));
        }
        else
          values.push_back(te::da::GetValueAsDouble(dataset.get(), propertyPos));
      }
      else
        hasGroupNullValueAux = true;

      for(std::size_t i = 0; i < csize; ++i)
      {
        if(dataset->isNull(chart->getProperties()[i]))
        {
          hasChartNullValueAux = true;
          break;
        }
      }
      if(hasChartNullValueAux == false)
      {
        for(std::size_t i = 0; i < csize; ++i)
          chartValues[chart->getProperties()[i]].push_back(te::da::GetValueAsDouble(dataset.get(), chart->getPropertiesPos()[i]));
      }  
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

      canvas->draw(geom);

      if(chart && hasChartNullValue == false && j == nSymbolizers - 1)
        buildChart(chart, chartValue, geom);
    }

    hasChartNullValue = hasChartNullValueAux;
    hasGroupNullValue = hasGroupNullValueAux;
    hasChartNullValueAux = false;
    hasGroupNullValueAux = false;
  } while(dataset->moveNext());

  delete geom;
  geom = geomaux;

  std::vector<te::se::Symbolizer*> symbolizers;
  if(hasGroupNullValue == false)
  {
    if(type == UNIQUE_VALUE)
    {
      if(ptype == te::dt::STRING_TYPE)
      {
        value = te::da::GetSummarizedValue(values, gfunction);
        value = te::da::Round(value, precision);
        svalue = boost::lexical_cast<std::string>(value);
      }
      else
        svalue = te::da::GetSummarizedValue(svalues, gfunction);

      std::map<std::string, std::vector<te::se::Symbolizer*> >::const_iterator it = uniqueGroupsMap.find(svalue);
      if(it != uniqueGroupsMap.end())
        symbolizers = it->second;
    }
    else
    {
      value = te::da::GetSummarizedValue(values, gfunction);
      value = te::da::Round(value, precision);

      std::map<std::pair<double, double>, std::vector<te::se::Symbolizer*> >::const_iterator it;
      for(it = othersGroupsMap.begin(); it != othersGroupsMap.end(); ++it)
      {
        if(value >= it->first.first && value <= it->first.second)
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
        symbolizers = it->second;
    }
  }

  if(hasChartNullValue == false)
  {
    for(std::size_t i = 0; i < csize; ++i)
    {
      chartValue[chart->getProperties()[i]] = te::da::GetSummarizedValue(chartValues[chart->getProperties()[i]], cfunction);
    }
  }

  // Gets the set of symbolizers defined on group item
  std::size_t nSymbolizers = symbolizers.size();

  if(hasGroupNullValue == false)
  {

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

      canvas->draw(geom);

      if(chart && hasChartNullValue == false && j == nSymbolizers - 1)
        buildChart(chart, chartValue, geom);
    }
  }
  if(nSymbolizers == 0 && chart && hasChartNullValue == false)
    buildChart(chart, chartValue, geom);

  delete geom;

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

void te::map::QueryLayerRenderer::drawDatSetGeometries(te::da::DataSet* dataset, const std::size_t& gpos, Canvas* canvas,
                                                          int fromSRID, int toSRID,
                                                          Chart* chart, te::common::TaskProgress* task)
{
  assert(dataset);
  assert(canvas);

  std::string s;
  size_t pksize = m_oid.size();
  std::vector<std::string> pkdata(pksize), pkdataaux(pksize);

  // Verify if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if((fromSRID != TE_UNKNOWN_SRS) && (toSRID != TE_UNKNOWN_SRS) && (fromSRID != toSRID))
    needRemap = true;

  // The chart sumarization
  std::string cfunction;
  std::map<std::string, std::vector<double> > chartValues;
  std::map<std::string, double> chartValue;
  bool hasChartNullValue = false;
  bool hasChartNullValueAux = false;
  size_t csize = 0;
  if(chart)
  {
    cfunction = chart->getSummary();
    csize = chart->getProperties().size();
    std::vector<size_t> propPos;
    for(std::size_t i = 0; i < csize; ++i)
    {
      std::vector<double> v;
      chartValues[chart->getProperties()[i]] = v;
      propPos.push_back(te::da::GetPropertyPos(dataset, chart->getProperties()[i]));
    }
    chart->setPropertiesPos(propPos);
  }

  te::gm::Geometry  *geom = 0, *geomaux = 0;
  do
  {
    try
    {
      if(geomaux == 0)
        geomaux = dataset->getGeometry(gpos).release();
      if(geomaux == 0)
        continue;
    }
    catch(std::exception& /*e*/)
    {
      continue;
    }

    if(pksize) // if linked
    {
      // it is linked. Remove redundancies.
      size_t i;
      for(i = 0; i < pksize; ++i)
        pkdata[i] = dataset->getAsString(m_oid[i]);
      
      for(i = 0; i < pksize; ++i)
      {
        if(dataset->isAtBegin())
          pkdataaux[i] = dataset->getAsString(m_oid[i]);
        else
        {
          if(pkdata[i] != pkdataaux[i])
          {
            pkdataaux = pkdata;
            break;
          }
        }      
      }
      if(i == pksize) // it is the same object
      {
        if(hasChartNullValue == false)
        {
          // read value chart value
          for(std::size_t i = 0; i < csize; ++i)
          {
            if(dataset->isNull(chart->getProperties()[i]) == false)
              chartValues[chart->getProperties()[i]].push_back(te::da::GetValueAsDouble(dataset, chart->getPropertiesPos()[i]));
            else
            {
              hasChartNullValue = true;
              break;
            }
          }
        }
        // read other values
        continue;
      }
      else // it is other object
      {
        // sumarize chart value according to the required summarization 
        if(hasChartNullValue == false)
        {
          for(std::size_t i = 0; i < csize; ++i)
            chartValue[chart->getProperties()[i]] = te::da::GetSummarizedValue(chartValues[chart->getProperties()[i]], cfunction);
        }

        // prepare the next loop
        for(std::size_t i = 0; i < csize; ++i)
          chartValues[chart->getProperties()[i]].clear();

        hasChartNullValueAux = false;
        for(std::size_t i = 0; i < csize; ++i)
        {
          if(dataset->isNull(chart->getProperties()[i]))
          {
            hasChartNullValueAux = true;
            break;
          }
        }
        if(hasChartNullValueAux == false)
        {
          for(std::size_t i = 0; i < csize; ++i)
            chartValues[chart->getProperties()[i]].push_back(te::da::GetValueAsDouble(dataset, chart->getPropertiesPos()[i]));
        }
      }
    }
    else // if not linked
    {
      // read chart value
      for(std::size_t i = 0; i < csize; ++i)
      {
        if(dataset->isNull(chart->getProperties()[i]) == false)
          chartValue[chart->getProperties()[i]] = te::da::GetValueAsDouble(dataset, chart->getPropertiesPos()[i]);
        else
        {
          chartValue.clear();
          hasChartNullValue = true;
          break;
        }
      }
    }

    delete geom;
    geom = geomaux;

    // store the values of the other object (for next loop).
    try
    {
      geomaux = dataset->getGeometry(gpos).release();
    }
    catch(std::exception& /*e*/)
    {
      geomaux = 0;
      continue;
    }

    if(task)
    {
      if(!task->isActive())
        return;

      // update the draw task
      task->pulse();
    }

    // If necessary, geometry remap
    if(needRemap)
    {
      geom->setSRID(fromSRID);
      geom->transform(toSRID);
    }

    canvas->draw(geom);

    if(chart && hasChartNullValue == false)
      buildChart(chart, chartValue, geom);

    hasChartNullValue = hasChartNullValueAux;
    hasChartNullValueAux = false;

  } while(dataset->moveNext()); // next geometry!

  delete geom;
  geom = geomaux;

  if(needRemap)
  {
    geom->setSRID(fromSRID);
    geom->transform(toSRID);
  }

  canvas->draw(geom);
  if(chart && hasChartNullValue == false)
  {
    if(chartValues.empty() == false)
    {
      for(std::size_t i = 0; i < csize; ++i)
      {
        chartValue[chart->getProperties()[i]] = te::da::GetSummarizedValue(chartValues[chart->getProperties()[i]], cfunction);
      }
    }
    buildChart(chart, chartValue, geom);
  }
  delete geom;

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

void te::map::QueryLayerRenderer::buildChart(const Chart* chart, const std::map<std::string, double>& chartValue, te::gm::Geometry* geom)
{
  if(!chart->isVisible())
    return;

  // World coordinates
  const te::gm::Envelope* e = geom->getMBR();
  std::auto_ptr<te::gm::Coord2D> worldCoord(new te::gm::Coord2D(e->getCenter().x, e->getCenter().y));

  // Try finds the geometry centroid
  if(geom->getGeomTypeId() == te::gm::PolygonType)
  {
      te::gm::Polygon* p = dynamic_cast<te::gm::Polygon*>(geom);
      worldCoord.reset(p->getCentroidCoord());
  }
  else if (geom->getGeomTypeId() ==  te::gm::MultiPolygonType)
  {
      te::gm::MultiPolygon* mp = dynamic_cast<te::gm::MultiPolygon*>(geom);
      worldCoord.reset(mp->getCentroidCoord());
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
  te::color::RGBAColor** rgba = ChartRendererManager::getInstance().render(chart, chartValue, width);
  m_chartImages.push_back(rgba);
}

void te::map::QueryLayerRenderer::buildChart(const Chart* chart, te::da::DataSet* dataset, te::gm::Geometry* geom)
{
  if(!chart->isVisible())
    return;

  // World coordinates
  const te::gm::Envelope* e = geom->getMBR();
  std::auto_ptr<te::gm::Coord2D> worldCoord(new te::gm::Coord2D(e->getCenter().x, e->getCenter().y));

  // Try finds the geometry centroid
  if (geom->getGeomTypeId() == te::gm::PolygonType)
  {
    te::gm::Polygon* p = dynamic_cast<te::gm::Polygon*>(geom);
    worldCoord.reset(p->getCentroidCoord());
  }
  else if (geom->getGeomTypeId() == te::gm::MultiPolygonType)
  {
    te::gm::MultiPolygon* mp = dynamic_cast<te::gm::MultiPolygon*>(geom);
    worldCoord.reset(mp->getCentroidCoord());
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

void te::map::QueryLayerRenderer::reset()
{
  m_index = 0;
  m_chartCoordinates.clear();
  m_chartImages.clear();
}
