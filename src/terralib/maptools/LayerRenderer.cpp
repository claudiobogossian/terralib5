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
#include "../geometry/GeometryProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../se/CoverageStyle.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Style.h"
#include "AbstractLayer.h"
#include "Canvas.h"
#include "Layer.h"
#include "LayerRenderer.h"

te::map::LayerRenderer::LayerRenderer()
{}

te::map::LayerRenderer::~LayerRenderer()
{}

void te::map::LayerRenderer::draw(AbstractLayer* layer, Canvas* canvas,
                                  const te::gm::Envelope& bbox, int srid)
//void te::map::LayerRenderer::draw(AbstractLayer* layer, Canvas* canvas,
//                                  const te::gm::Envelope& bbox, int /*srid*/)
{
  te::map::Layer* l = dynamic_cast<Layer*>(layer);
  assert(l);

  te::da::DataSource* ds = l->getDataSource();
  assert(ds);

  std::string dsname = l->getId();
  assert(!dsname.empty());
  //std::string dsname = l->getDataSetName();
  //assert(!dsname.empty());

  te::da::DataSourceTransactor* t = ds->getTransactor();
  assert(t);

  te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
  assert(loader);

  // std::vector<Rule*> rules = style->getRules();
  // for each <Rule>
  // {

  /* Here should be called the transactor query() method. 
  It is necessary interprets the <Rule> elements included on Style.
  For now, gets the data set using only box restriction. */
  te::da::DataSetType* type = loader->getDataSetType(dsname);
  assert(type);
  delete loader;
  te::gm::GeometryProperty* gcol = type->getDefaultGeomProperty();
  assert(gcol);

  te::gm::Envelope box(bbox);
  if(gcol->getSRID() != -1 && gcol->getSRID() != srid)
    box.transform(srid, gcol->getSRID());
  te::da::DataSet* dataset = t->getDataSet(dsname, gcol, &box, te::gm::INTERSECTS);
  assert(dataset);
//  delete t;

  // Here goes the canvas configuration. It is necessary interprets the <Symbolizers> elements included on Style.
  // For now...
  switch( gcol->getGeometryType())
  {
    case te::gm::MultiPolygonType:
    case te::gm::PolygonType:
      {
        canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
        if(l->getId().find("br") != std::string::npos)
          canvas->setPolygonFillColor(te::color::RGBAColor(220 ,0, 0, 255));
        else
        {
          canvas->setPolygonFillColor(te::color::RGBAColor(220 ,0, 0, 255));
          //char buf[64*1024];
          //FILE* fp = fopen("C:\\lixo\\ButtonInfo64.png", "rb");
          //int size = fread(buf, sizeof(char), 64*1024, fp);
          //fclose(fp);
          //canvas->setPolygonPatternWidth(64);
          //canvas->setPolygonFillPattern(buf, size, te::map::PNG);
          //canvas->setPolygonPatternOpacity(100);
          //canvas->setPolygonContourColor(te::color::RGBAColor(80, 200, 120, 255));
          //canvas->setPolygonContourWidth(1);
        }
      }
    break;
    
    case te::gm::MultiLineStringType:
    case te::gm::LineStringType:
      {
        canvas->setLineColor(te::color::RGBAColor(220 ,0, 0, 255));
        //char buf[256*1024];
        //FILE* fp = fopen("C:\\lixo\\2.bmp", "rb");
        //int size = fread(buf, sizeof(char), 256*1024, fp);
        //fclose(fp);

        //canvas->setLinePattern(buf, size, te::map::BMP);
        //canvas->setLineWidth(16);
      }
    break;
  
    default:
      canvas->setPointColor(te::color::RGBAColor(220 ,0, 0, 255));
      canvas->setPointWidth(15);
      canvas->setPointMarker(te::map::MarkerFourRays);
  }
  delete type;

  // let´s draw!
  //int aaa = 2;
  //while(dataset->moveNext() && aaa--)
  while(dataset->moveNext())
  {
    /* How can we get anothers or a specified, for example? And others data sets? <Geometry> element? 
       For while, Default Geometry. */
    te::gm::Geometry* g = dataset->getGeometry();
    if(g == 0)
      continue;

    // Verifies the SRID. Case differents, converts coordinates...
    if(g->getSRID() > 0)
      g->transform(srid);
    canvas->draw(g);

    delete g;
  }

  delete dataset;
  delete t;

  //canvas->setTextColor(te::color::RGBAColor(0, 255, 255, 255));
  //canvas->setTextContourEnabled(true);
  //canvas->setTextContourColor(te::color::RGBAColor(255, 0, 0, 255));
  //canvas->setTextDecorationColor(te::color::RGBAColor(255, 255, 0, 255));
  //canvas->setTextPointSize(80.);
  //canvas->setTextWeight(te::at::ExtraBold);
  //canvas->setTextUnderline(true);
  //canvas->drawText(20, 300, "Teste");

  // } end for each <Rule>
}

void te::map::LayerRenderer::draw(Layer* /*layer*/, te::se::FeatureTypeStyle* /*style*/, Canvas* /*canvas*/,
                                  const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}

void te::map::LayerRenderer::draw(Layer* /*layer*/, te::se::CoverageStyle* /*style*/, Canvas* /*canvas*/,
                                  const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}
