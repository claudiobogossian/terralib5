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
  \file terralib/maptools/Utils.cpp
   
  \brief Utility functions for MapTools module.
*/

// TerraLib
#include "../common/Translator.h"
#include "../common/StringUtils.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/utils/Utils.h"
#include "../fe/Literal.h"
#include "../se/Fill.h"
#include "../se/ParameterValue.h"
#include "../se/Stroke.h"
#include "../se/SvgParameter.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "DataSetLayer.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cstdlib>

void te::map::GetColor(const te::se::Stroke* stroke, te::color::RGBAColor& color)
{
  if(stroke == 0)
    return;

  te::map::GetColor(stroke->getColor(), stroke->getOpacity(), color);
}

void te::map::GetColor(const te::se::Fill* fill, te::color::RGBAColor& color)
{
  if(fill == 0)
    return;
  te::map::GetColor(fill->getColor(), fill->getOpacity(), color);
}

void  te::map::GetColor(const te::se::ParameterValue* color, const te::se::ParameterValue* opacity, te::color::RGBAColor& rgba)
{
  if(color == 0 &&  opacity == 0)
    return;

  int alpha = TE_OPAQUE;
  if(opacity)
  {
    alpha = (int)(te::map::GetDouble(opacity) * TE_OPAQUE);
    rgba.setColor(rgba.getRed(), rgba.getGreen(), rgba.getBlue(), alpha);
  }

  if(color)
  {
    te::color::RGBAColor rgb = te::map::GetColor(color);
    rgba.setColor(rgb.getRed(), rgb.getGreen(), rgb.getBlue(), rgba.getAlpha());
  }
}

te::color::RGBAColor te::map::GetColor(const te::se::ParameterValue* param)
{
  return te::color::RGBAColor(te::map::GetString(param));
}

int te::map::GetInt(const te::se::ParameterValue* param)
{
  return atoi(te::map::GetString(param).c_str());
}

double te::map::GetDouble(const te::se::ParameterValue* param)
{
  return atof(te::map::GetString(param).c_str());
}

std::string te::map::GetString(const te::se::ParameterValue* param)
{
  assert(param->getNParameters() > 0);
  
  const te::se::ParameterValue::Parameter* p = param->getParameter(0);
  assert(p);

  if(p->m_mixedData)
  {
    return *p->m_mixedData;
  }
  else //if(p->m_expression)
  {
    te::fe::Literal* l = dynamic_cast<te::fe::Literal*>(p->m_expression);
    assert(l);
    return l->getValue();
  }
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
    throw Exception(TR_MAP("The layer is invalid!"));

// name of referenced data set
  std::string dsname = layer->getDataSetName();

  if(dsname.empty())
    throw Exception(TR_MAP("The data set name referenced by the layer is empty!"));

// retrieve the associated data source
  te::da::DataSourcePtr ds = te::da::GetDataSource(layer->getDataSourceId(), true);

// get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> transactor(ds->getTransactor());

  if(transactor.get() == 0)
    throw Exception(TR_MAP("Could not get a data source transactor!"));

// get a catalog loader
  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  if(cloader.get() == 0)
    throw Exception(TR_MAP("Could not get a data source catalog loader!"));

// gets the data set type 
  std::auto_ptr<te::da::DataSetType> dstype(cloader->getDataSetType(dsname));

  if(dstype.get() == 0)
    throw Exception(TR_MAP("Could not get the data set type!"));

// gets the raster property
  std::auto_ptr<te::rst::RasterProperty> rasterProperty(dynamic_cast<te::rst::RasterProperty*>(dstype->getProperties()[0]->clone()));

  if(rasterProperty.get() == 0)
     throw Exception(TR_MAP("Could not get the raster property!"));

  return rasterProperty.release();
}

te::rst::Raster* te::map::GetRaster(DataSetLayer* layer)
{
  if(layer == 0)
    throw Exception(TR_MAP("The layer is invalid!"));

// name of referenced data set
  std::string dsname = layer->getDataSetName();

  if(dsname.empty())
    throw Exception(TR_MAP("The data set name referenced by the layer is empty!"));

// retrieve the associated data source
  te::da::DataSourcePtr ds = te::da::GetDataSource(layer->getDataSourceId(), true);

// get a transactor
  std::auto_ptr<te::da::DataSourceTransactor> transactor(ds->getTransactor());

  if(transactor.get() == 0)
    throw Exception(TR_MAP("Could not get a data source transactor!"));

// get a catalog loader
  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  if(cloader.get() == 0)
    throw Exception(TR_MAP("Could not get a data source catalog loader!"));

// gets the data set type 
  std::auto_ptr<te::da::DataSetType> dstype(cloader->getDataSetType(dsname));

  if(dstype.get() == 0)
    throw Exception(TR_MAP("Could not get the data set type!"));

  if(!dstype->hasRaster())
    throw Exception(TR_MAP("The data set referenced by the layer not contains raster data!"));

// get the referenced data set
  std::auto_ptr<te::da::DataSet> dataset(transactor->getDataSet(dsname));
  if(dataset.get() == 0)
    throw Exception(TR_MAP("Could not get the data set reference by the layer!"));

// gets the raster
  std::auto_ptr<te::rst::Raster> raster(dataset->getRaster());
  if(raster.get() == 0)
    throw Exception(TR_MAP("Could not get the raster referenced by the layer!"));

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

