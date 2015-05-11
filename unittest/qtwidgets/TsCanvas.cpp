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
  \file TsCanvas.cpp
 
  \brief Test suite for the Qt Canvas implementation.
 */


// Unit-Test TerraLib
#include "TsCanvas.h"

// TerraLib
#include <terralib/color.h>
#include <terralib/common.h>
#include <terralib/common/StringUtils.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools.h>
#include <terralib/postgis.h>
#include <terralib/qt/widgets.h>
#include <terralib/annotationtext/Text.h>
#include <terralib/annotationtext/Element.h>
#include <terralib/annotationtext/Attributes.h>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>

// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsCanvas );

void TsCanvas::setUp()
{
}

void TsCanvas::tearDown()
{
}

void TsCanvas::tcTransformation()
{
#ifdef TE_COMPILE_ALL
  int argc = 0;
  QApplication app(argc, 0);

// create the canvas and adjust the world-device transformation parameters
  te::qt::widgets::Canvas canvas(800, 600);

  canvas.setWindow(-180.0, -90.0, +180.0, +90.0);

// adjust line pen
  canvas.setLineColor(te::color::RGBAColor(255, 0, 0, 255));

// draw a line
  te::gm::LineString l(2, te::gm::Geometry::LineStringType);
  l.setPoint(0, -90.0, -45.0);
  l.setPoint(1, +90.0, +45.0);

  canvas.draw(&l);

// draw a polygon
  te::gm::LinearRing* shell = new te::gm::LinearRing(4, te::gm::Geometry::LineStringType);
  shell->setPoint(0, -90.0, -45.0);
  shell->setPoint(1, +90.0, -45.0);
  shell->setPoint(2, 0.0, +45.0);
  shell->setPoint(3, -90.0, -45.0);

  te::gm::LinearRing* hole1 = new te::gm::LinearRing(4, te::gm::Geometry::LineStringType);
  hole1->setPoint(0, -50.0, -30.0);
  hole1->setPoint(1, -40.0, -30.0);
  hole1->setPoint(2, -45.0, -10.0);
  hole1->setPoint(3, -50.0, -30.0);

  te::gm::LinearRing* hole2 = new te::gm::LinearRing(4, te::gm::Geometry::LineStringType);
  hole2->setPoint(0, -30.0, -30.0);
  hole2->setPoint(1, -20.0, -30.0);
  hole2->setPoint(2, -25.0, -10.0);
  hole2->setPoint(3, -30.0, -30.0);

  te::gm::Polygon p(3, te::gm::Geometry::PolygonType);
  p.setRingN(0, shell);
  p.setRingN(1, hole1);
  p.setRingN(2, hole2);

  canvas.setPolygonContourColor(te::color::RGBAColor(0, 0, 255, 255));
  canvas.setPolygonFillColor(te::color::RGBAColor(0, 255, 0, 100));

  canvas.draw(&p); 


// save a PNG to disk
  canvas.save("canvas.png", te::map::Canvas::PNG);

  return;
#endif
}

void TsCanvas::tcPerformance()
{
//#ifdef TE_COMPILE_ALL
  int argc = 0;
  QApplication app(argc, 0);

// connecting to a PostGIS database
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  connInfo["PG_USER"] = "postgres";
  //connInfo["PG_PASSWORD"] = "sitim110";
  connInfo["PG_DB_NAME"] = "terralib4";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";

  te::da::DataSource* ds = 0;

  CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not instantiate the PostGIS data source!",
                                  ds = te::da::DataSourceFactory::make("POSTGIS"));

  CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not open the PostGIS data source!",
                                  ds->open(connInfo));

  te::da::DataSourceTransactor* transactor = ds->getTransactor();
  te::da::DataSourceCatalogLoader* cl = transactor->getCatalogLoader();

  CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not load the PostGIS data source catalog!",
                                  cl->loadCatalog(true));


  clock_t begin = 0;
  clock_t end = 0;  

  te::da::DataSourceCatalog* catalog = ds->getCatalog();

  size_t nLayers = catalog->getNumberOfDataSets();

  for(size_t i = 0; i < nLayers; ++i)
  {
    
// get DataSetType information: geometry-column, feature-name, feature extent
    te::da::DataSetTypePtr dt = catalog->getDataSetType(i);

    //if(!(ft->getName() == "public.goias_geologia" || dt->getName() == "public.br_uf_2010" || dt->getName() == "public.sp_setores_censitarios"))
    //  continue;
    
    if(dt->hasGeom() == false)
      continue;

    //int geomCol = dt->getDefaultGeomPropertyPos();

    te::gm::GeometryProperty* gcol = static_cast<te::gm::GeometryProperty*>(dt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));
    int geomCol = dt->getPropertyPosition(gcol->getName());

    const te::gm::Envelope* extent = cl->getExtent(gcol);
    if(extent == 0)
      continue;

    te::qt::widgets::Canvas canvas(800, 600);

    //QPrinter* printer = new QPrinter(QPrinter::HighResolution);
    //QPrintDialog pd(printer);
    //if (pd.exec() != QDialog::Accepted)
    //  continue;
    //canvas.setDevice(printer);

    te::gm::LineString* ls = new te::gm::LineString(5, te::gm::LineStringType);
    ls->setPoint(0, extent->m_llx, extent->m_lly);
    ls->setPoint(1, extent->m_urx, extent->m_lly);
    ls->setPoint(2, extent->m_urx, extent->m_ury);
    ls->setPoint(3, extent->m_llx, extent->m_ury);
    ls->setPoint(4, extent->m_llx, extent->m_lly);

// set canvas definitions
    double wwi = extent->m_llx;
    double ww = extent->m_urx - extent->m_llx;
    double whi = extent->m_lly;
    double wh = extent->m_ury - extent->m_lly;

    double llx = wwi;
    double urx = wwi + ww;
    double lly = whi;
    double ury = whi + wh;
    canvas.calcAspectRatio(llx, lly, urx, ury);
    wwi = llx;
    whi = lly;
    ww = urx - llx;
    wh = ury - lly;
    canvas.setWindow(llx-ww/10., lly-wh/10., urx+ww/10, ury+wh/10.);

    if(gcol->getType() /*getGeometryType()*/ == te::gm::MultiPolygonType ||
       gcol->getGeometryType() == te::gm::PolygonType)
    {
      canvas.setPolygonFillColor(te::color::RGBAColor(255, 255, 0, 255));
      canvas.setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
    }
    else if(gcol->getGeometryType() == te::gm::MultiLineStringType)
    {
      canvas.setLineColor(te::color::RGBAColor(0, 255, 0, 255));
    }
    else
    {
      canvas.setPointColor(te::color::RGBAColor(255, 0, 0, 255));
    }

// retrieve the data from the database
    begin = clock();

    te::da::DataSet* dataset = transactor->getDataSet(dt->getName());
    
    end = clock();

    std::cout << "Testing " << dt->getName() << std::endl;
    std::cout << "retrieving time: " << end - begin << " miliseconds" << std::endl;

// check if there is data
    if(dataset->isEmpty())
    {
      delete dataset;
      continue;
    }

// draw data
    begin = clock();

    while(dataset->moveNext())
    {
      te::gm::Geometry* g = dataset->getGeometry(geomCol);
      canvas.draw(g);
      delete g;
    }
    canvas.setLineColor(te::color::RGBAColor(255, 0, 0, 255));
    canvas.draw(ls);
    
    end = clock();

    std::cout << "drawing time: " << end - begin << " miliseconds" << std::endl << std::endl;

    double ix = extent->m_llx;
    double iy = extent->m_lly;
    double w = extent->m_urx - extent->m_llx;
    double h = extent->m_ury - extent->m_lly;

    te::gm::Envelope* bb = new te::gm::Envelope(0, 0, ix+w/2., iy + h/2.);
    te::gm::Point* pt = new te::gm::Point(0, 0, 0, bb);
    te::at::Text text;
    te::at::Element* textElement = new te::at::Element("Instituto Nacional de Pesquisas espaciais", pt);
    te::at::Attributes* attr = new te::at::Attributes("Verdana", 12.);
    textElement->setAttributes(attr);
    text.add(textElement);

    bb->init(0, 0, ix+w/2., iy+h/2. - h/10);
    te::gm::Point* pt2 = new te::gm::Point(0, 0, 0, bb);

    std::string value = "Multi line text\ncenter justification\nteste\ncom fonte\nVerdana\npointSize 12\nResolution = ";
    value += te::common::Convert2String(canvas.getResolution()) + "dpi";
    te::at::Element* textElement2 = new te::at::Element(value, pt2);
    text.add(textElement2);

    bb->init(0, 0, ix + w/20., iy+h/2.-h/10);
    te::gm::Point* pt3 = new te::gm::Point(0, 0, 0, bb);
    value = "Multi line text\nLeft justification\nteste\ncom fonte\nArial\npointSize 10\nResolution = ";
    value += te::common::Convert2String(canvas.getResolution()) + "dpi";
    te::at::Element* textElement3 = new te::at::Element(value, pt3);
    te::at::Attributes* attr2 = new te::at::Attributes("Arial", 10., te::at::NormalStyle, te::at::NormalWeight, te::at::Start);
    attr2->setTextColor("green");
    textElement3->setAttributes(attr2);
    text.add(textElement3);

    bb->init(0, 0, ix+w-w/20., iy+h/2.-h/10.);
    te::gm::Point* pt4 = new te::gm::Point(0, 0, 0, bb);
    value = "Multi line text\nRight justification\nteste\ncom fonte\nScript MT Bold\npointSize 10\nResolution = ";
    value += te::common::Convert2String(canvas.getResolution()) + "dpi";
    te::at::Element* textElement4 = new te::at::Element(value, pt4);
    te::at::Attributes* attr3 = new te::at::Attributes("Script MT Bold", 10., te::at::NormalStyle, te::at::NormalWeight, te::at::End);
    attr3->setTextColor("red");
    textElement4->setAttributes(attr3);
    text.add(textElement4);

    canvas.draw(&text);

    std::string fileName = dt->getName() + ".png";
    canvas.save(fileName.c_str(), te::map::PNG);

    delete dataset;
  }

  delete transactor;

  CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close the PostGIS data source!",
                                  ds->close());

  delete ds;
//#endif
}

