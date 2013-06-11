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
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/RasterProperty.h"
#include "../se/CoverageStyle.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "DataSetLayer.h"
#include "DataSetLayerRenderer.h"
#include "Exception.h"
#include "Utils.h"

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
// for while, default geometry. TODO: need a visitor to get which properties the style references
    te::gm::GeometryProperty* geometryProperty = te::da::GetFirstGeomProperty(dstype.get());

// get the associated layer style
    te::se::Style* style = dlayer->getStyle();
    if(style == 0)
    {
// try create an appropriate style
      style = te::se::CreateFeatureTypeStyle(geometryProperty->getGeometryType());

      if(style == 0)
        throw Exception((boost::format(TR_MAP("Could not create a default feature type style to the layer %1%.")) % layer->getTitle()).str());

      dlayer->setStyle(style);
    }

// should I render this style?
    te::se::FeatureTypeStyle* fts = dynamic_cast<te::se::FeatureTypeStyle*>(style);
    if(fts == 0)
      throw Exception(TR_MAP("The layer style is not a Feature Type Style!"));

    DrawGeometries(dstype.get(), transactor.get(), canvas, ibbox, dlayer->getSRID(), srid, fts);
  }
  else if(dstype->hasRaster())
  {
// for while, first raster property
    te::rst::RasterProperty* rasterProperty = te::da::GetFirstRasterProperty(dstype.get());

// get the associated layer style
    te::se::Style* style = dlayer->getStyle();
    if(style == 0)
    {
// try create an appropriate style
      style = te::se::CreateCoverageStyle(rasterProperty->getBandProperties());

      if(style == 0)
        throw Exception((boost::format(TR_MAP("Could not create a default coverage style to the layer %1%.")) % layer->getTitle()).str());

      dlayer->setStyle(style);
    }

// should I render this style?
    te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
    if(cs == 0)
      throw Exception(TR_MAP("The layer style is not a Coverage Style!"));

    DrawRaster(dstype.get(), transactor.get(), canvas, ibbox, dlayer->getSRID(), bbox, srid, cs);
  }
  else
  {
    throw Exception(TR_MAP("The layer don't have a geometry or raster property!"));
  }
}
