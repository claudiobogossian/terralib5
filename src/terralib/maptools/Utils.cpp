/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/Utils.cpp
   
  \brief Utility functions for MapTools module.
*/

// TerraLib
#include "../common/progress/TaskProgress.h"
#include "../common/Translator.h"
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
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
#include "../fe/Literal.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "../memory/DataSet.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterFactory.h"
#include "../raster/RasterProperty.h"
#include "../raster/RasterSummary.h"
#include "../raster/RasterSummaryManager.h"
#include "../raster/Utils.h"
#include "../se/ChannelSelection.h"
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
#include "DataSetLayer.h"
#include "Exception.h"
#include "QueryEncoder.h"
#include "RasterTransform.h"
#include "RasterTransformConfigurer.h"
#include "Utils.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>
#include <cstdlib>
#include <memory>

te::gm::Envelope te::map::GetSelectedExtent(const std::list<te::map::AbstractLayerPtr> layers, int srid, bool onlyVisibles)
{
  std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin();

  te::gm::Envelope finalEnv;

  while(it != layers.end())
  {
    if(it == layers.begin())
      finalEnv = GetSelectedExtent((*it), srid, onlyVisibles);
    else
      finalEnv.Union(GetSelectedExtent((*it), srid, onlyVisibles));

    ++it;
  }

  return finalEnv;
}

te::gm::Envelope te::map::GetSelectedExtent(const te::map::AbstractLayerPtr layer, int srid, bool onlyVisibles)
{
  if(onlyVisibles && layer->getVisibility() == te::map::NOT_VISIBLE)
    return te::gm::Envelope();

  te::gm::Envelope finalEnv;

  bool isFirst = true;

  if(!layer->hasChildren())
  {
    std::auto_ptr<te::da::DataSetType> dt(layer->getSchema());

    const te::da::ObjectIdSet* objSet = layer->getSelected();

    if(!objSet)
      return te::gm::Envelope();

    std::auto_ptr<te::da::DataSet> ds(layer->getData(objSet));

    te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

    while(ds->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(geomProp->getName());

      te::gm::Envelope auxEnv(*geom->getMBR());

      if(layer->getSRID() != srid)
        auxEnv.transform(layer->getSRID(), srid);

      if(isFirst)
      {
        finalEnv = auxEnv;
        isFirst = false;
        continue;
      }

      finalEnv.Union(auxEnv);
    }
  }
  else
  {
    for(std::size_t i = 0; i < layer->getChildrenCount(); ++i)
    {
      te::map::AbstractLayerPtr child(boost::dynamic_pointer_cast<AbstractLayer>(layer->getChild(i)));
      
      if(i == 0)
        finalEnv = GetSelectedExtent(child, srid, onlyVisibles);
      else
        finalEnv.Union(GetSelectedExtent(child, srid, onlyVisibles));
    }
  }

  return finalEnv;
}

void te::map::GetDashStyle(const std::string& dasharray, std::vector<double>& style)
{
  std::vector<std::string> values;
  te::common::Tokenize(dasharray, values);
  for(std::size_t i = 0; i < values.size(); ++i)
    style.push_back(atof(values[i].c_str()));
}

te::rst::RasterProperty* te::map::GetRasterProperty(DataSetLayer* layer)
{
  if(layer == 0)
    throw Exception(TE_TR("The layer is invalid!"));

// name of referenced data set
  std::string dsname = layer->getDataSetName();

  if(dsname.empty())
    throw Exception(TE_TR("The data set name referenced by the layer is empty!"));

// retrieve the associated data source
  te::da::DataSourcePtr ds = te::da::GetDataSource(layer->getDataSourceId(), true);

// gets the data set type 
  std::auto_ptr<te::da::DataSetType> dstype(ds->getDataSetType(dsname));

  if(dstype.get() == 0)
    throw Exception(TE_TR("Could not get the data set type!"));

// gets the raster property
  std::auto_ptr<te::rst::RasterProperty> rasterProperty(dynamic_cast<te::rst::RasterProperty*>(dstype->getProperties()[0]->clone()));

  if(rasterProperty.get() == 0)
     throw Exception(TE_TR("Could not get the raster property!"));

  return rasterProperty.release();
}

te::rst::Raster* te::map::GetRaster(DataSetLayer* layer)
{
  if(layer == 0)
    throw Exception(TE_TR("The layer is invalid!"));

// name of referenced data set
  std::string dsname = layer->getDataSetName();

  if(dsname.empty())
    throw Exception(TE_TR("The data set name referenced by the layer is empty!"));

// retrieve the associated data source
  te::da::DataSourcePtr ds = te::da::GetDataSource(layer->getDataSourceId(), true);

// gets the data set type 
  std::auto_ptr<te::da::DataSetType> dstype(ds->getDataSetType(dsname));

  if(dstype.get() == 0)
    throw Exception(TE_TR("Could not get the data set type!"));

  if(!dstype->hasRaster())
    throw Exception(TE_TR("The data set referenced by the layer not contains raster data!"));

// get the referenced data set
  std::auto_ptr<te::da::DataSet> dataset(ds->getDataSet(dsname));
  if(dataset.get() == 0)
    throw Exception(TE_TR("Could not get the data set reference by the layer!"));

// gets the raster
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> raster(dataset->getRaster(rpos));
  if(raster.get() == 0)
    throw Exception(TE_TR("Could not get the raster referenced by the layer!"));

  return raster.release();
}

void te::map::GetVisibleLayers(const te::map::AbstractLayerPtr& layer, std::list<te::map::AbstractLayerPtr>& visibleLayers)
{
  te::map::Visibility visibility = layer->getVisibility();

  if(visibility == te::map::NOT_VISIBLE)
    return;

  if(visibility == te::map::VISIBLE)
    visibleLayers.push_back(layer);

  for(std::size_t i = 0; i < layer->getChildrenCount(); ++i)
  {
    te::map::AbstractLayerPtr child(boost::dynamic_pointer_cast<AbstractLayer>(layer->getChild(i)));

    GetVisibleLayers(child, visibleLayers);
  }
}

void te::map::GetVisibleLayers(const std::list<te::map::AbstractLayerPtr>& layers, std::list<te::map::AbstractLayerPtr>& visibleLayers)
{
  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
    GetVisibleLayers(*it, visibleLayers);
}

te::gm::Envelope te::map::GetExtent(const std::list<te::map::AbstractLayerPtr>& layers, int srid, bool onlyVisibles)
{
  te::gm::Envelope e;

  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
    e.Union(GetExtent(*it, srid, onlyVisibles));

  return e;
}

te::gm::Envelope te::map::GetExtent(const  te::map::AbstractLayerPtr& layer, int srid, bool onlyVisibles)
{
  if(onlyVisibles && layer->getVisibility() == te::map::NOT_VISIBLE)
    return te::gm::Envelope();

  te::gm::Envelope e = layer->getExtent();

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
    e.transform(layer->getSRID(), srid);

  for(std::size_t i = 0; i < layer->getChildrenCount(); ++i)
  {
    te::map::AbstractLayerPtr child(boost::dynamic_pointer_cast<AbstractLayer>(layer->getChild(i)));
    e.Union(GetExtent(child, srid, onlyVisibles));
  }

  return e;
}

te::da::DataSet* te::map::DataSet2Memory(te::da::DataSet* dataset)
{
  assert(dataset);

  if(!dataset->moveNext())
    throw Exception(TE_TR("Could not copy the data set to memory!"));

  return new te::mem::DataSet(*dataset);
}

void te::map::DrawGeometries(te::da::DataSetType* type, te::da::DataSourcePtr ds,
                             Canvas* canvas, const te::gm::Envelope& bbox, int bboxSRID,
                             int srid, te::se::FeatureTypeStyle* style)
{
  assert(type);
  assert(type->hasGeom());
  assert(canvas);
  assert(bbox.isValid());
  assert(style);

  const std::string& datasetName = type->getName();
  assert(!datasetName.empty());

// for while, default geometry. TODO: need a visitor to get which properties the style references
  te::gm::GeometryProperty* geometryProperty = te::da::GetFirstGeomProperty(type);

// create a canvas configurer
  te::map::CanvasConfigurer cc(canvas);

// number of rules defined on feature type style
  std::size_t nRules = style->getRules().size();

  for(std::size_t i = 0; i < nRules; ++i) // for each <Rule>
  {
// the current rule
    const te::se::Rule* rule = style->getRule(i);
    assert(rule);

// TODO: should be verified the MinScaleDenominator and MaxScaleDenominator. Where will we put the current scale information? Method parameter?

// gets the rule filter
    const te::fe::Filter* filter = rule->getFilter();

// let's retrieve the correct dataset
    std::auto_ptr<te::da::DataSet> dataset(0);
    if(!filter)
    {
// there isn't a Filter expression. Gets the dataset using only box restriction...
      dataset = ds->getDataSet(datasetName, geometryProperty->getName(), &bbox, te::gm::INTERSECTS);
    }
    else
    {
// get an enconder
      te::map::QueryEncoder queryConverter; 

// convert the Filter expression to a TerraLib Expression!
      te::da::Expression* exp = queryConverter.getExpression(filter);
      if(exp == 0)
        throw Exception(TE_TR("Could not convert the OGC Filter expression to TerraLib expression!"));

/* 1) Creating te::da::Where object with this expression + box restriction */

// the box restriction
      te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(bbox, srid);
      te::da::PropertyName* geometryPropertyName = new te::da::PropertyName(geometryProperty->getName());
      te::da::ST_Intersects* intersects = new te::da::ST_Intersects(geometryPropertyName, lenv);

// combining the expressions (Filter expression + box restriction)
      te::da::And* finalRestriction = new te::da::And(exp, intersects);
      
      te::da::Where* wh = new te::da::Where(exp);
      wh->setExp(finalRestriction);

// fields
      te::da::Fields* all = new te::da::Fields;
      all->push_back(new te::da::Field("*"));

// from
      te::da::FromItem* fi = new te::da::DataSetName(datasetName);
      te::da::From* from = new te::da::From;
      from->push_back(fi);

// build the Select
      te::da::Select select(all, from, wh);

/* 2) Calling the datasource query method to get the correct restricted dataset. */
      dataset = ds->query(select);
    }

    if(dataset.get() == 0)
      throw Exception((boost::format(TE_TR("Could not retrieve the data set %1%.")) % datasetName).str());

    if(dataset->moveNext() == false)
      continue;

// get the set of symbolizers defined on current rule
    const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();
    std::size_t nSymbolizers = symbolizers.size();

// build task message; e.g. ("Drawing the dataset Countries. Rule 1 of 3.")
    std::string message = TE_TR("Drawing the dataset");
    message += " " + datasetName + ". ";
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

// let's draw! for each data set geometry...
      DrawGeometries(dataset.get(), gpos, canvas, bboxSRID, srid, &task);

// prepare to draw the other symbolizer
      dataset->moveFirst();

    } // end for each <Symbolizer>

  }   // end for each <Rule>
}

void te::map::DrawGeometries(te::da::DataSet* dataset, const std::size_t& gpos, Canvas* canvas, int fromSRID, int toSRID, te::common::TaskProgress* task)
{
  assert(dataset);
  assert(canvas);

// verify if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if((fromSRID != TE_UNKNOWN_SRS) && (toSRID != TE_UNKNOWN_SRS) && (fromSRID != toSRID))
    needRemap = true;

  do
  {
    if(task)
    {
      if(!task->isActive())
        return;

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

// if necessary, geometry remap
    if(needRemap)
    {
      geom->setSRID(fromSRID);
      geom->transform(toSRID);
    }

    canvas->draw(geom.get());

  }while(dataset->moveNext()); // next geometry!
}

void te::map::DrawRaster(te::da::DataSetType* type, te::da::DataSourcePtr ds, Canvas* canvas,
                         const te::gm::Envelope& bbox, int bboxSRID, const te::gm::Envelope& visibleArea, int srid, te::se::CoverageStyle* style)
{
  assert(type);
  assert(type->hasRaster());
  assert(canvas);
  assert(bbox.isValid());
  assert(visibleArea.isValid());
  assert(style);

  const std::string& datasetName = type->getName();
  assert(!datasetName.empty());

// for while, first raster property
  te::rst::RasterProperty* rasterProperty = te::da::GetFirstRasterProperty(type);

// retrieve the data set
  std::auto_ptr<te::da::DataSet> dataset(ds->getDataSet(datasetName, rasterProperty->getName(), &bbox, te::gm::INTERSECTS));
  if(dataset.get() == 0)
    throw Exception((boost::format(TE_TR("Could not retrieve the data set %1%.")) % datasetName).str());

// retrieve the raster
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> raster(dataset->getRaster(rpos));
  if(dataset.get() == 0)
    throw Exception((boost::format(TE_TR("Could not retrieve the raster from the data set %1%.")) % datasetName).str());

  DrawRaster(raster.get(), canvas, bbox, bboxSRID, visibleArea, srid, style);
}

void te::map::DrawRaster(te::rst::Raster* raster, Canvas* canvas, const te::gm::Envelope& bbox, int bboxSRID,
                         const te::gm::Envelope& visibleArea, int srid, te::se::CoverageStyle* style)
{
  assert(raster);
  assert(canvas);
  assert(bbox.isValid());
  assert(visibleArea.isValid());
  assert(style);

// build the grid canvas. i.e. a grid with canvas dimensions and requested mbr
  te::gm::Envelope* gmbr = new te::gm::Envelope(visibleArea);
  std::auto_ptr<te::rst::Grid> gridCanvas(new te::rst::Grid(static_cast<unsigned int>(canvas->getWidth()), static_cast<unsigned int>(canvas->getHeight()), gmbr, srid));

// create a raster transform
  RasterTransform rasterTransform(raster, 0);

//check band data type
  if(raster->getBandDataType(0) != te::dt::UCHAR_TYPE)
  {
    // raster min / max values
    const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(raster, te::rst::SUMMARY_MIN);
    const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(raster, te::rst::SUMMARY_MAX);
    const std::complex<double>* cmin = rsMin->at(0).m_minVal;
    const std::complex<double>* cmax = rsMax->at(0).m_maxVal;
    double min = cmin->real();
    double max = cmax->real();

    // *** aqui temos a questão da variável global que diz se é para normalizar ou não os valores do raster ***
    rasterTransform.setLinearTransfParameters(min, max, 0, 255);
  }
  else
  {
    rasterTransform.setLinearTransfParameters(0, 255, 0, 255);
  }

// get the raster symbolizer
  std::size_t nRules = style->getRules().size();
  assert(nRules >= 1);

// for while, consider one rule
  const te::se::Rule* rule = style->getRule(0);

  const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();
  assert(!symbolizers.empty());

// for while, consider one raster symbolizer
  te::se::RasterSymbolizer* rasterSymbolizer = dynamic_cast<te::se::RasterSymbolizer*>(symbolizers[0]);
  assert(rasterSymbolizer);

// configure the raster transformation based on the raster symbolizer
  RasterTransformConfigurer rtc(rasterSymbolizer, &rasterTransform);
  rtc.configure();

// verify if is necessary convert the raster to the given srid
  bool needRemap = false;
  if((bboxSRID != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (bboxSRID != srid))
    needRemap = true;

// build task message; e.g. ("Drawing the raster cbers_sao_jose_dos_campos.")
  std::string message = TE_TR("Drawing raster");
  const std::string& rasterName = raster->getName();
  !rasterName.empty() ? message += " " + raster->getName() + ". " : message += ".";

// create the draw task
  te::common::TaskProgress task(message, te::common::TaskProgress::DRAW, gridCanvas->getNumberOfRows());

// create a RGBA array that will be drawn in the canvas. i.e. This array is the result of the render process.
  //te::color::RGBAColor** rgba = new te::color::RGBAColor*[gridCanvas->getNumberOfRows()];

// create a RGBA array that will be drawn in the canvas. i.e. This array represents a row of the render process.
  te::color::RGBAColor** row = new te::color::RGBAColor*[1];
  te::color::RGBAColor* columns = new te::color::RGBAColor[gridCanvas->getNumberOfColumns()];
  row[0] = columns;

// create a SRS converter
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
    
  if(needRemap)
  {
    converter->setSourceSRID(srid);
    converter->setTargetSRID(bboxSRID);
  }

// fill the result RGBA array
  for(unsigned int r = 0; r < gridCanvas->getNumberOfRows(); ++r)
  {
    for(unsigned int c = 0; c < gridCanvas->getNumberOfColumns(); ++c)
    {
      te::gm::Coord2D inputGeo = gridCanvas->gridToGeo(c, r);

      if(needRemap)
        converter->convert(inputGeo.x, inputGeo.y, inputGeo.x, inputGeo.y);

      te::gm::Coord2D outputGrid = raster->getGrid()->geoToGrid(inputGeo.x, inputGeo.y);

// TODO: round or truncate?
      int x = te::rst::Round(outputGrid.x);
      int y = te::rst::Round(outputGrid.y);

      te::color::RGBAColor color(255, 255, 255, 0);

      if((x >= 0 && x < (int)(raster->getNumberOfColumns())) &&
         (y >= 0 && y < (int)(raster->getNumberOfRows())))
           color = rasterTransform.apply(x, y);

      columns[c] = color;
    }

    //rgba[r] = columns;

    if(!task.isActive())
    {
// draw the part of result
      //canvas->drawImage(0, 0, rgba, canvas->getWidth(), r + 1);
      canvas->drawImage(0, r, row, canvas->getWidth(), 1);

// free memory
      //te::common::Free(rgba, r + 1);
      te::common::Free(row, 1);

      return;
    }

    canvas->drawImage(0, r, row, canvas->getWidth(), 1);

    task.pulse();
  }

// put the result in the canvas
  //canvas->drawImage(0, 0, rgba, canvas->getWidth(), canvas->getHeight());

// free memory
  //te::common::Free(rgba, gridCanvas->getNumberOfRows());
  te::common::Free(row, 1);

// image outline
  if(rasterSymbolizer->getImageOutline() == 0)
    return;

// get the symbolizer that will be used to draw the image outline
  te::se::Symbolizer* outlineSymbolizer = rasterSymbolizer->getImageOutline()->getSymbolizer();
  if(outlineSymbolizer == 0)
    return;

// create a canvas configurer
  te::map::CanvasConfigurer cc(canvas);
  cc.config(outlineSymbolizer);

// creates the image outline
  std::auto_ptr<te::gm::Geometry> geom(te::gm::GetGeomFromEnvelope(raster->getExtent(), bboxSRID));
  if(needRemap)
  {
    geom->setSRID(bboxSRID);
    geom->transform(srid);
  }

  canvas->draw(geom.get());
}

te::rst::Raster* te::map::GetExtentRaster(te::rst::Raster* raster, int w, int h, const te::gm::Envelope& bbox, int bboxSRID,
                         const te::gm::Envelope& visibleArea, int srid)
{
  assert(raster);
  assert(bbox.isValid());
  assert(visibleArea.isValid());

  std::vector<te::rst::BandProperty*> bprops;

  for(size_t t = 0; t < raster->getNumberOfBands(); ++t)
  {
    te::rst::Band* band = raster->getBand(t);

    te::rst::BandProperty* bp = new te::rst::BandProperty(t, band->getProperty()->getType(), "");

    bprops.push_back(bp);
  }

// build the grid canvas. i.e. a grid with canvas dimensions and requested mbr
  te::gm::Envelope* gmbr = new te::gm::Envelope(visibleArea);
  te::rst::Grid* gridCanvas = new te::rst::Grid(static_cast<unsigned int>(w), static_cast<unsigned int>(h), gmbr, srid);

//build output raster
  te::rst::Raster* rasterOut = te::rst::RasterFactory::make("MEM", gridCanvas, bprops, std::map<std::string, std::string>());

// verify if is necessary convert the raster to the given srid
  bool needRemap = false;
  if((bboxSRID != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (bboxSRID != srid))
    needRemap = true;

// create a SRS converter
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
    
  if(needRemap)
  {
    converter->setSourceSRID(srid);
    converter->setTargetSRID(bboxSRID);
  }

// fill the result RGBA array
  for(unsigned int r = 0; r < gridCanvas->getNumberOfRows(); ++r)
  {
    for(unsigned int c = 0; c < gridCanvas->getNumberOfColumns(); ++c)
    {
      te::gm::Coord2D inputGeo = gridCanvas->gridToGeo(c, r);

      if(needRemap)
        converter->convert(inputGeo.x, inputGeo.y, inputGeo.x, inputGeo.y);

      te::gm::Coord2D outputGrid = raster->getGrid()->geoToGrid(inputGeo.x, inputGeo.y);

// TODO: round or truncate?
      int x = te::rst::Round(outputGrid.x);
      int y = te::rst::Round(outputGrid.y);

      if((x >= 0 && x < (int)(raster->getNumberOfColumns())) &&
         (y >= 0 && y < (int)(raster->getNumberOfRows())))
      {
        std::vector<double> values;
        raster->getValues(x, y, values);
        rasterOut->setValues(c, r, values);
      }
    }
  }

  return rasterOut;
}

te::gm::GeomType te::map::GetGeomType(const  te::map::AbstractLayerPtr& layer)
{
  assert(layer.get());

  std::auto_ptr<te::map::LayerSchema> schema(layer->getSchema());

  te::gm::GeometryProperty* geometryProperty = te::da::GetFirstGeomProperty(schema.get());
  assert(geometryProperty);

  te::gm::GeomType gtype = geometryProperty->getGeometryType();

  switch(gtype)
  {
    case te::gm::UnknownGeometryType:
    case te::gm::GeometryType:
    case te::gm::GeometryZType:
    case te::gm::GeometryMType:
    case te::gm::GeometryZMType:
      break;

    default:
      return gtype;
  }

  // Here, tries fetch a geometry and retrieve its type

  std::auto_ptr<te::da::DataSet> dataset(layer->getData());
  assert(dataset.get());

  dataset->moveNext();

  std::size_t gpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);

  std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gpos));
  assert(g.get());

  return g->getGeomTypeId();
}
