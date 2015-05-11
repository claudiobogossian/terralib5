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
  \file terralib/maptools/DataSetLayerRenderer.cpp

  \brief It renders the objects associated to a dataset layer.
*/

// TerraLib
#include "../common/progress/TaskProgress.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/query/And.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/EqualTo.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/GreaterThanOrEqualTo.h"
#include "../dataaccess/query/LessThanOrEqualTo.h"
#include "../dataaccess/query/LiteralDouble.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/LiteralString.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/ST_Intersects.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/RasterProperty.h"
#include "../se/CoverageStyle.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Utils.h"
#include "../srs/Config.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "DataSetLayer.h"
#include "DataSetLayerRenderer.h"
#include "Exception.h"
#include "Grouping.h"
#include "GroupingItem.h"
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
    throw Exception(TE_TR("Wrong type render type for this layer!"));

// check if layer extent intersects the drawing area and so compute bounding box intersection
  te::gm::Envelope reprojectedBBOX(bbox);

  if((dlayer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS))
  {
    reprojectedBBOX.transform(srid, dlayer->getSRID());
  }
  else if(dlayer->getSRID() != srid)
  {
    throw Exception(TE_TR("The layer or map don't have a valid SRID!"));
  }

  if(!reprojectedBBOX.intersects(dlayer->getExtent()))
    return;

  te::gm::Envelope ibbox = reprojectedBBOX.intersection(dlayer->getExtent());

// retrieve the associated data source
  te::da::DataSourcePtr ds = te::da::GetDataSource(dlayer->getDataSourceId(), true);

// get dataset information
  std::string dsname = dlayer->getDataSetName();
  assert(!dsname.empty());

  std::auto_ptr<te::da::DataSetType> dstype(ds->getDataSetType(dsname));

  if(dstype->hasGeom())
  {
    /* For while if the DataSetLayer has a grouping, do not consider the style.
       TODO: - Create the GroupingRenderer?
             - Create the StyledLayer and try build a generic renderer that look for Styles and Grouping? */
    Grouping* grouping = dlayer->getGrouping();
    if(grouping && grouping->isVisible())
    {
      drawGrouping(dlayer, ds, canvas, ibbox, srid);
      return;
    }

// for while, default geometry. TODO: need a visitor to get which properties the style references
    te::gm::GeometryProperty* geometryProperty = te::da::GetFirstGeomProperty(dstype.get());

// get the associated layer style
    te::se::Style* style = dlayer->getStyle();
    if(style == 0)
    {
// try create an appropriate style
      style = te::se::CreateFeatureTypeStyle(geometryProperty->getGeometryType());

      if(style == 0)
        throw Exception((boost::format(TE_TR("Could not create a default feature type style to the layer %1%.")) % layer->getTitle()).str());

      dlayer->setStyle(style);
    }

// should I render this style?
    te::se::FeatureTypeStyle* fts = dynamic_cast<te::se::FeatureTypeStyle*>(style);
    if(fts == 0)
      throw Exception(TE_TR("The layer style is not a Feature Type Style!"));

    DrawGeometries(dstype.get(), ds, canvas, ibbox, dlayer->getSRID(), srid, fts);
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
        throw Exception((boost::format(TE_TR("Could not create a default coverage style to the layer %1%.")) % layer->getTitle()).str());

      dlayer->setStyle(style);
    }

// should I render this style?
    te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
    if(cs == 0)
      throw Exception(TE_TR("The layer style is not a Coverage Style!"));

    DrawRaster(dstype.get(), ds, canvas, ibbox, dlayer->getSRID(), bbox, srid, cs);
  }
  else
  {
    throw Exception(TE_TR("The layer don't have a geometry or raster property!"));
  }
}

void te::map::DataSetLayerRenderer::drawGrouping(DataSetLayer* layer, te::da::DataSourcePtr ds, Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  std::string dsname = layer->getDataSetName();

// verify if is necessary convert the data set geometries to the given srid
  bool needRemap = false;
  if((layer->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (layer->getSRID() != srid))
    needRemap = true;

// get the dataset type. TODO: Can we pass the datat set type as method parameter?
  std::auto_ptr<te::da::DataSetType> dataSetType(ds->getDataSetType(dsname));
  assert(dataSetType.get());

// for while, default geometry. TODO: need a visitor to get which properties the style references
  assert(dataSetType->hasGeom());
  te::gm::GeometryProperty* geometryProperty = te::da::GetFirstGeomProperty(dataSetType.get());

// create a canvas configurer
  te::map::CanvasConfigurer cc(canvas);

// the layer grouping
  Grouping* grouping = layer->getGrouping();

// the referenced property name
  std::string propertyName = grouping->getPropertyName();
  assert(!propertyName.empty());

// the referenced property type
  int propertyType = grouping->getPropertyType();

// the grouping type
  GroupingType type = grouping->getType();

// the grouping items
  const std::vector<GroupingItem*>& items = grouping->getGroupingItems();
  
  std::size_t nGroupItems = items.size();

// build task message; e.g. ("Drawing the layer Countries. Rule 1 of 3.")
  std::string message = TE_TR("Drawing the grouping of layer");
  message += " " + layer->getTitle() + ".";

// create the draw task
  te::common::TaskProgress task(message, te::common::TaskProgress::DRAW, nGroupItems);

  for(std::size_t i = 0; i < nGroupItems; ++i) // for each GroupingItem
  {
// the current group item
    GroupingItem* item = items[i];
    assert(item);

/* 1) Creating te::da::Where object with the group item restriction expression + box restriction */

    te::da::PropertyName* groupingPropertyName = new te::da::PropertyName(propertyName);

// grouping item restriction
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

// the box restriction
    te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(bbox, srid);
    te::da::PropertyName* geometryPropertyName = new te::da::PropertyName(geometryProperty->getName());
    te::da::ST_Intersects* intersects = new te::da::ST_Intersects(geometryPropertyName, lenv);

// combining the expressions (Filter expression + box restriction)
    te::da::And* finalRestriction = new te::da::And(exp, intersects);
      
// the final where
    te::da::Where* wh = new te::da::Where(finalRestriction);

// fields
    te::da::Fields* all = new te::da::Fields;
    all->push_back(new te::da::Field("*"));

// from
    te::da::FromItem* fi = new te::da::DataSetName(dsname);
    te::da::From* from = new te::da::From;
    from->push_back(fi);

// build the Select
    te::da::Select select(all, from, wh);

/* 2) Calling the datasource query method to get the correct restricted dataset. */

    std::auto_ptr<te::da::DataSet> dataset(0);
    try
    {
      dataset = ds->query(select);
    }
    catch(std::exception& /*e*/)
    {
      continue; // TODO: deal the exceptions!
    }

    if(dataset.get() == 0)
      throw Exception((boost::format(TE_TR("Could not retrieve the data set %1% referenced by the layer %2%.")) % dsname % layer->getTitle()).str());

    if(dataset->moveNext() == false)
      continue;

// get the set of symbolizers defined on group item
    const std::vector<te::se::Symbolizer*>& symbolizers = item->getSymbolizers();

    std::size_t nSymbolizers = symbolizers.size();

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

    if(!task.isActive())
      return;

    task.pulse();

  }   // end for each GroupItem
}
