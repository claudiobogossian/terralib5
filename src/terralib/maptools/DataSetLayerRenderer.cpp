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
  \file terralib/maptools/DataSetLayerRenderer.cpp

  \brief It renders the objects associated to a dataset layer.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../common/progress/TaskProgress.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/query/And.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/ST_Intersects.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"
#include "../fe/Filter.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Geometry.h"
#include "../geometry/Utils.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "../raster/RasterSummary.h"
#include "../raster/RasterSummaryManager.h"
#include "../raster/Utils.h"
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
#include "DataSetLayerRenderer.h"
#include "Exception.h"
#include "QueryEncoder.h"
#include "RasterTransform.h"
#include "RasterTransformConfigurer.h"

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>
#include <memory>

te::map::DataSetLayerRenderer::DataSetLayerRenderer()
{
}

te::map::DataSetLayerRenderer::~DataSetLayerRenderer()
{
}

void te::map::DataSetLayerRenderer::draw(AbstractLayer* layer,
                                         Canvas* canvas,
                                         const te::gm::Envelope& bbox,
                                         int srid)
{
// should I render this layer?
  DataSetLayer* dlayer = dynamic_cast<DataSetLayer*>(layer);

  if(dlayer == 0)
    throw Exception(TR_MAP("Wrong type render type for this layer!"));

// check if layer extent intersects the drawing area and so compute bounding box intersection
  te::gm::Envelope reprojectedBBOX(bbox);

  if((dlayer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, dlayer->getSRID());
  }
  else if(dlayer->getSRID() != srid)
  {
    throw Exception(TR_MAP("The layer or map don't have a valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(dlayer->getExtent()))
    return;

  te::gm::Envelope ibbox = reprojectedBBOX.intersection(dlayer->getExtent());

// retrieve the associated data source
  te::da::DataSourcePtr ds = te::da::GetDataSource(dlayer->getDataSourceId(), true);

// get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> transactor(ds->getTransactor());
  assert(transactor.get());

// get a catalog loader
  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());
  assert(cloader.get());

// get dataset information
  std::string dsname = dlayer->getDataSetName();
  assert(!dsname.empty());

  std::auto_ptr<te::da::DataSetType> dstype(cloader->getDataSetType(dsname));

  if(dstype->hasGeom())
  {
    drawGeometries(dlayer, transactor.get(), canvas, reprojectedBBOX, srid);
  }
  else if(dstype->hasRaster())
  {
    drawRaster(dlayer, transactor.get(), canvas, reprojectedBBOX, bbox, srid);
  }
  else
  {
    throw Exception(TR_MAP("The layer don't have a geometry or raster property!"));
  }
}

void te::map::DataSetLayerRenderer::drawGeometries(DataSetLayer* layer,
                                                   te::da::DataSourceTransactor* transactor,
                                                   Canvas* canvas,
                                                   const te::gm::Envelope& bbox,
                                                   int srid)
{
  std::string dsname = layer->getDataSetName();

// verify if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (layer->getSRID() != srid))
    needRemap = true;

// get a catalog loader
  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());
  assert(cloader.get());

// get the dataset type. TODO: Can we pass the datat set type as method parameter?
  std::auto_ptr<te::da::DataSetType> dataSetType(cloader->getDataSetType(dsname));
  assert(dataSetType.get());

// for while, default geometry. TODO: need a visitor to get which properties the style references
  assert(dataSetType->hasDefaultGeom());
  te::gm::GeometryProperty* geometryProperty = dataSetType->getDefaultGeomProperty();

// get the associated layer style
  te::se::Style* style = layer->getStyle();
  if(style == 0)
  {
// try create an appropriate style
    style = te::se::CreateFeatureTypeStyle(geometryProperty->getGeometryType());

    if(style == 0)
      throw Exception((boost::format(TR_MAP("Could not create a default feature type style to the layer %1%.")) % layer->getTitle()).str());

    layer->setStyle(style);
  }

// should I render this style?
  te::se::FeatureTypeStyle* fts = dynamic_cast<te::se::FeatureTypeStyle*>(style);
  if(fts == 0)
    throw Exception(TR_MAP("The layer style is not a Feature Type Style!"));

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
      dataset = std::auto_ptr<te::da::DataSet>(transactor->getDataSet(dsname, geometryProperty, &bbox, te::gm::INTERSECTS));
    }
    else
    {
// get an enconder
      te::map::QueryEncoder queryConverter; 

// convert the Filter expression to a TerraLib Expression!
      te::da::Expression* exp = queryConverter.getExpression(filter);
      if(exp == 0)
        throw Exception(TR_MAP("Could not convert the OGC Filter expression to TerraLib expression!"));

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
      te::da::FromItem* fi = new te::da::DataSetName(dsname);
      te::da::From* from = new te::da::From;
      from->push_back(fi);

// build the Select
      te::da::Select select(all, from, wh);

/* 2) Calling the transactor query method to get the correct restricted dataset. */
      dataset = std::auto_ptr<te::da::DataSet>(transactor->query(select));
    }

    if(dataset.get() == 0)
      throw Exception((boost::format(TR_MAP("Could not retrieve the data set %1% referenced by the layer %2%.")) % dsname % layer->getTitle()).str());

// get the set of symbolizers defined on current rule
    const std::vector<te::se::Symbolizer*> symbolizers = rule->getSymbolizers();
    std::size_t nSymbolizers = symbolizers.size();

// build task message; e.g. ("Drawing the layer Countries. Rule 1 of 3.")
    std::string message = TR_MAP("Drawing the layer");
    message += " " + layer->getTitle() + ". ";
    message += TR_MAP("Rule");
    message += " " + boost::lexical_cast<std::string>(i + 1) + " " + TR_MAP("of") + " ";
    message += boost::lexical_cast<std::string>(nRules) + ".";

// create a draw task
    te::common::TaskProgress task(message, te::common::TaskProgress::DRAW);
    //task.setTotalSteps(nSymbolizers * dataset->size()); // Removed! The te::da::DataSet size() method would be too costly to compute.

    if(dataset->moveNext() == false)
      continue;

    for(std::size_t j = 0; j < nSymbolizers; ++j) // for each <Symbolizer>
    {
// the current symbolizer
      te::se::Symbolizer* symb = symbolizers[j];

// let's config de canvas based on the current symbolizer
      cc.config(symb);

      std::size_t geompos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);

// let's draw!
      do // for each data set object
      {
        if(!task.isActive())
          return;

// update the draw task
        task.pulse();

// for while, default geometry. TODO: need a visitor to get which properties the style references
        te::gm::Geometry* geom = dataset->getGeometry(geompos);
        if(geom == 0)
          continue;

// if necessary, geometry remap
        if(needRemap)
        {
          geom->setSRID(layer->getSRID());
          geom->transform(srid);
        }

        canvas->draw(geom);

        delete geom;

      }while(dataset->moveNext());

// prepare to draw the other symbolizer
      dataset->moveFirst();

    } // end for each <Symbolizer>

  }   // end for each <Rule>
}

void te::map::DataSetLayerRenderer::drawRaster(DataSetLayer* layer,
                                              te::da::DataSourceTransactor* transactor,
                                              Canvas* canvas,
                                              const te::gm::Envelope& bbox,
                                              const te::gm::Envelope& visibleArea,
                                              int srid)
{
  std::string dsname = layer->getDataSetName();

// get a catalog loader
  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());
  assert(cloader.get());

// get the dataset type. TODO: Can we pass the datat set type as method parameter?
  std::auto_ptr<te::da::DataSetType> dataSetType(cloader->getDataSetType(dsname, true));
  assert(dataSetType.get());

// for while, first raster property
  assert(dataSetType->hasRaster());
  te::rst::RasterProperty* rasterProperty = dataSetType->findFirstRasterProperty();

// retrieve the data set
  std::auto_ptr<te::da::DataSet> dataset(transactor->getDataSet(dsname, rasterProperty, &bbox, te::gm::INTERSECTS));
  if(dataset.get() == 0)
    throw Exception((boost::format(TR_MAP("Could not retrieve the data set %1% referenced by the layer %2%.")) % dsname % layer->getTitle()).str());

// retrieve the raster
  std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> raster(dataset->getRaster(rpos));
  if(dataset.get() == 0)
    throw Exception((boost::format(TR_MAP("Could not retrieve the raster %1% referenced by the layer %2%.")) % dsname % layer->getTitle()).str());

// get the associated layer style
  te::se::Style* style = layer->getStyle();
  if(style == 0)
  {
// try create an appropriate style
    style = te::se::CreateCoverageStyle(rasterProperty->getBandProperties());

    if(style == 0)
      throw Exception((boost::format(TR_MAP("Could not create a default coverage style to the layer %1%.")) % layer->getTitle()).str());

    layer->setStyle(style);
  }

// should I render this style?
  te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
  if(cs == 0)
    throw Exception(TR_MAP("The layer style is not a Coverage Style!"));

// get the raster symbolizer
  std::size_t nRules = cs->getRules().size();
  assert(nRules >= 1);

// for while, consider one rule
  const te::se::Rule* rule = cs->getRule(0);

  const std::vector<te::se::Symbolizer*> symbolizers = rule->getSymbolizers();
  assert(!symbolizers.empty());

// for while, consider one raster symbolizer
  te::se::RasterSymbolizer* rasterSymbolizer = dynamic_cast<te::se::RasterSymbolizer*>(symbolizers[0]);
  assert(rasterSymbolizer);

// build the grid canvas. i.e. a grid with canvas dimensions and requested mbr
  te::gm::Envelope* gmbr = new te::gm::Envelope(visibleArea);
  std::auto_ptr<te::rst::Grid> gridCanvas(new te::rst::Grid(static_cast<unsigned int>(canvas->getWidth()), static_cast<unsigned int>(canvas->getHeight()), gmbr, srid));

// create a raster transform
  RasterTransform rasterTransform(raster.get(), 0);

// configure the raster transformation based on the raster symbolizer
  RasterTransformConfigurer rtc(rasterSymbolizer, &rasterTransform);
  rtc.configure();

//check band data type
  if(raster->getBandDataType(0) != te::dt::UCHAR_TYPE)
  {
    // raster min / max values
    const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(raster.get(), te::rst::SUMMARY_MIN);
    const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(raster.get(), te::rst::SUMMARY_MAX);
    const std::complex<double>* cmin = rsMin->at(0).m_minVal;
    const std::complex<double>* cmax = rsMax->at(0).m_maxVal;
    double min = cmin->real();
    double max = cmax->real();

    // *** aqui temos a questão da variável global que diz se é para normalizar ou não os valores do raster ***
    rasterTransform.setLinearTransfParameters(min, max, 0, 255);
  }

// verify if is necessary convert the raster to the given srid
  bool needRemap = false;
  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (layer->getSRID() != srid))
    needRemap = true;

// build task message; e.g. ("Drawing the layer cbers_sao_jose_dos_campos.")
  std::string message = TR_MAP("Drawing the layer");
  message += " " + layer->getTitle() + ". ";

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
  converter->setSourceSRID(srid);
  converter->setTargetSRID(layer->getSRID());

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
  te::se::Symbolizer* symbolizer = rasterSymbolizer->getImageOutline()->getSymbolizer();
  if(symbolizer == 0)
    return;

// create a canvas configurer
  te::map::CanvasConfigurer cc(canvas);
  cc.config(symbolizer);

// creates the image outline
  std::auto_ptr<te::gm::Geometry> geom(te::gm::GetGeomFromEnvelope(raster->getExtent(), layer->getSRID()));
  if(needRemap)
  {
    geom->setSRID(layer->getSRID());
    geom->transform(srid);
  }

  canvas->draw(geom.get());
}
