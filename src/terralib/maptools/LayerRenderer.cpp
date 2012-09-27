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
  \file LayerRenderer.cpp

  \brief It renders the objects associated to a Layer.
 */

// TerraLib
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
#include "../fe/Filter.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../se/CoverageStyle.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Style.h"
#include "../se/Rule.h"
#include "../srs/Config.h"
#include "AbstractLayer.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "Layer.h"
#include "LayerRenderer.h"
#include "QueryEncoder.h"

te::map::LayerRenderer::LayerRenderer()
{}

te::map::LayerRenderer::~LayerRenderer()
{}

void te::map::LayerRenderer::draw(AbstractLayer* layer, Canvas* canvas,
                                  const te::gm::Envelope& bbox, int srid)
{
  // Is our business?
  te::map::Layer* l = dynamic_cast<Layer*>(layer);
  assert(l);

  // Gets the associate data source
  te::da::DataSource* ds = l->getDataSource();
  assert(ds);

  // Gets a transactor
  te::da::DataSourceTransactor* t = ds->getTransactor();
  assert(t);

  // What is the dataset?
  std::string dsname = l->getDataSetName();
  assert(!dsname.empty());

  // Gets a catalog loader
  te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
  assert(loader);
  
  // Gets the dataset type
  te::da::DataSetType* dataSetType = loader->getDataSetType(dsname);
  assert(dataSetType);

  delete loader;

  // For while, default geometry...
  te::gm::GeometryProperty* gcol = dataSetType->getDefaultGeomProperty();
  assert(gcol);

  // Adjusting box...
  te::gm::Envelope box(bbox);
  if(gcol->getSRID() != -1 && gcol->getSRID() != srid)
    box.transform(srid, gcol->getSRID());

  // Gets the associated layer style
  te::se::Style* style = l->getStyle();
  assert(style);

  // The canvas configurer
  te::map::CanvasConfigurer cc(canvas);

  std::size_t nRules = style->getNRules();
  for(std::size_t i = 0; i < nRules; ++i) // for each <Rule>
  {
    // Current rule
    const te::se::Rule* rule = style->getRule(i);
    assert(rule);

    // TODO: Should be verified the MinScaleDenominator and MaxScaleDenominator!

    // Gets the rule filter...
    const te::fe::Filter* filter = rule->getFilter();

    // Let's retrieve the correct dataset...
    te::da::DataSet* dataset = 0;
    if(!filter)
    {
      // There isn't a Filter expression. Gets the dataset using only box restriction...
      dataset = t->getDataSet(dsname, gcol, &box, te::gm::INTERSECTS);
    }
    else
    {
      // Gets an enconder...
      te::map::QueryEncoder converter;
      // ... and converts the Filter expression to a TerraLib Expression!
      te::da::Expression* exp = converter.getExpression(filter);
      assert(exp);

      /* 1) Creating te::da::Where object with this expression + box restriction */

      // Box restriction
      te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(box, srid);
      te::da::PropertyName* geomProperty = new te::da::PropertyName(gcol->getName());
      te::da::ST_Intersects* intersects = new te::da::ST_Intersects(geomProperty, lenv);

      // Combining the two expressions (Filter expression + box restriction)
      te::da::And* finalRestriction = new te::da::And(exp, intersects);
      
      te::da::Where* wh = new te::da::Where(exp);
      wh->setExp(finalRestriction);

      // Fields
      te::da::Fields* all = new te::da::Fields;
      all->push_back(new te::da::Field("*"));

      // From
      te::da::FromItem* fi = new te::da::DataSetName(dsname);
      te::da::From* from = new te::da::From;
      from->push_back(fi);

      // Builds the Select
      te::da::Select select(all, from, wh);

      /* 2) Calling the transactor query method to get the correct restricted dataset. */
      dataset = t->query(select);
    }
    assert(dataset);
    
    // Loading the dataset type
    dataset->loadTypeInfo();

    // Gets the set of symbolizers
    const std::vector<te::se::Symbolizer*> symbolizers = rule->getSymbolizers();
    for(std::size_t j = 0; j < symbolizers.size(); ++j) // for each <Symbolizer>
    {
      te::se::Symbolizer* symb = symbolizers[j];

      // Let's config de canvas!
      cc.config(symb);

      // Let's draw!
      while(dataset->moveNext())
      {
        /* How can we get anothers or a specified, for example? And others data sets? <Geometry> element? 
           For while, Default Geometry. */
        te::gm::Geometry* g = dataset->getGeometry();
        if(g == 0)
          continue;

        // Verifies the SRID. Case differents, converts coordinates...
        int gsrid = g->getSRID();
        if(gsrid != TE_UNKNOWN_SRS && gsrid != srid)
          g->transform(srid);

        canvas->draw(g);

        delete g;
      }
      dataset->moveBeforeFirst();
    } // end for each <Symbolizer>

    delete dataset;

  } // end for each <Rule>

  delete t;
}

void te::map::LayerRenderer::draw(Layer* /*layer*/, te::se::FeatureTypeStyle* /*style*/, Canvas* /*canvas*/,
                                  const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}

void te::map::LayerRenderer::draw(Layer* /*layer*/, te::se::CoverageStyle* /*style*/, Canvas* /*canvas*/,
                                  const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}
