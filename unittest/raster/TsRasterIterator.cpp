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
  \file TsRaterIterator.cpp
 
  \brief A test suit for the Raster iterator class.
 */

#include "TsRasterIterator.h"
#include "../Config.h"

#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/utils/Utils.h>
#include <terralib/geometry/Utils.h>
#include <terralib/memory/DataSet.h>
#include <terralib/memory/DataSetItem.h>
#include <terralib/raster/RasterIterator.h>
#include <terralib/raster/PositionIterator.h>
#include <terralib/rp/Functions.h>

//#include <terralib/dataaccess/datasource/DataSourceFactory.h>

#include <boost/shared_ptr.hpp>

#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION( TsRasterIterator );

void TsRasterIterator::CreateTestRaster( unsigned int nBands, unsigned int nLines, 
  unsigned int nCols, boost::shared_ptr< te::rst::Raster >& rasterPointer, bool zero )
{
  std::vector< te::rst::BandProperty * > bandsProps;
  for( unsigned int bandsPropsIdx = 0 ; bandsPropsIdx < nBands ; ++bandsPropsIdx )
  {
    bandsProps.push_back( new te::rst::BandProperty( bandsPropsIdx, 
      te::dt::UINT32_TYPE ) );    
  }
  
  rasterPointer.reset( te::rst::RasterFactory::make( "MEM", 
    new te::rst::Grid( nCols, nLines ), bandsProps, 
    std::map< std::string, std::string >(), 0, 0 ) );
    
  unsigned int band = 0;
  unsigned int line = 0;
  unsigned int col = 0;
  double pixelValue = 0;
  
  for( band = 0 ; band < nBands ; ++band )
    for( line = 0 ; line < nLines ; ++line )
      for( col = 0 ; col < nCols ; ++col )
      {
        rasterPointer->setValue( col, line, pixelValue, band );
        if (!zero)
          ++pixelValue;
      }
}

void TsRasterIterator::Copy2DiskShp(std::vector<te::gm::Polygon*> polygons, std::string shpName) {
    std::auto_ptr< te::mem::DataSet> memDataSetPtr;

    std::auto_ptr<te::da::DataSetType> dataSetTypePtr(new te::da::DataSetType(shpName));

    te::gm::GeometryProperty* propPtr = new te::gm::GeometryProperty("polygon", 0,
        te::gm::PolygonType, true);
    propPtr->setSRID(polygons[0]->getSRID());
    dataSetTypePtr->add(propPtr);

    memDataSetPtr.reset(new te::mem::DataSet(dataSetTypePtr.get()));

    for (unsigned int polygonsIdx = 0; polygonsIdx < polygons.size(); polygonsIdx++)
    {
        te::mem::DataSetItem* dsItemPtr = new te::mem::DataSetItem(memDataSetPtr.get());
        dsItemPtr->setGeometry(0, polygons[polygonsIdx]);

        memDataSetPtr->add(dsItemPtr);
    }

    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = "./testsData";
    connInfo["DRIVER"] = "ESRI Shapefile";

    std::auto_ptr<te::da::DataSource> dsOGR(te::da::DataSourceFactory::make("OGR"));
    dsOGR->setConnectionInfo(connInfo);
    dsOGR->open();

    memDataSetPtr->moveBeforeFirst();

    te::da::Create(dsOGR.get(), dataSetTypePtr.get(), memDataSetPtr.get());

    dsOGR->close();
}

te::gm::LinearRing* TsRasterIterator::createSquare(const double& xCenter, const double& yCenter, 
                                                   const double& size, int srid)
{
    te::gm::LinearRing* s = new te::gm::LinearRing(5, te::gm::LineStringType, srid);
    double halfSize = size * 0.5;
    s->setPoint(0, xCenter - halfSize, yCenter - halfSize); // lower left 
    s->setPoint(1, xCenter - halfSize, yCenter + halfSize); // upper left
    s->setPoint(2, xCenter + halfSize, yCenter + halfSize); // upper rigth
    s->setPoint(3, xCenter + halfSize, yCenter - halfSize); // lower rigth
    s->setPoint(4, xCenter - halfSize, yCenter - halfSize); // closing
    return s;
}

void TsRasterIterator::tcRasterIteratorConstructor1()
{
}

void TsRasterIterator::tcRasterIteratorConstructor2()
{
}

void TsRasterIterator::tcRasterIteratorConstructor3()
{
}

void TsRasterIterator::tcRasterIteratorGetRow()
{
}

void TsRasterIterator::tcRasterIteratorGetCol()
{
}

void TsRasterIterator::PolygonIteratorTest1()
{
  boost::shared_ptr< te::rst::Raster > rasterPointer;
  CreateTestRaster( 3, 3, 3, rasterPointer );
  
  std::auto_ptr< te::gm::Polygon > polPtr( (te::gm::Polygon*)
    te::gm::GetGeomFromEnvelope( rasterPointer->getGrid()->getExtent(),
      rasterPointer->getGrid()->getSRID() ) );
    
  double pixelValueCounter = 0;

  for( unsigned int band = 0 ; band < rasterPointer->getNumberOfBands() ; 
    ++band )
  {    
    te::rst::PolygonIterator< double > it = 
      te::rst::PolygonIterator< double >::begin( rasterPointer.get(),
      polPtr.get() );
    const te::rst::PolygonIterator< double > itEnd = 
      te::rst::PolygonIterator< double >::end( rasterPointer.get(),
      polPtr.get() );      
      
    int col = 0;
    int row = 0;
    int count = 0;
    while (it != itEnd)
    {
      CPPUNIT_ASSERT_DOUBLES_EQUAL(pixelValueCounter, it[band], 0.0000000001);
      CPPUNIT_ASSERT_EQUAL(row, (int)it.getRow());
      CPPUNIT_ASSERT_EQUAL(col, (int)it.getColumn());

      ++pixelValueCounter;
      ++it;
      count++;

      if (col < rasterPointer->getNumberOfColumns() - 1)
        col++;
      else {
        row++;
        col = 0;
      }
    }

    CPPUNIT_ASSERT_EQUAL(count, 9);
  }
}


void TsRasterIterator::PolygonIteratorTest2()
{
    boost::shared_ptr< te::rst::Raster > rasterPointer;
    CreateTestRaster(1, 8, 8, rasterPointer, true);

    std::vector<te::gm::Polygon* > polVector;
    te::gm::Polygon* externalPol = (te::gm::Polygon*) te::gm::GetGeomFromEnvelope(
        rasterPointer->getGrid()->getExtent(), rasterPointer->getGrid()->getSRID());
    polVector.push_back(externalPol);

    // Create Internal polygon 1
    te::gm::Polygon* internalPol_1 = new te::gm::Polygon(0, te::gm::PolygonType, 
                                                         rasterPointer->getSRID());
    te::gm::LinearRing* l1 = createSquare(2, -2, 1, rasterPointer->getSRID());
    internalPol_1->add(l1);
    polVector.push_back(internalPol_1);

    // Create internal polygon 2
    te::gm::Polygon* internalPol_2 = new te::gm::Polygon(0, te::gm::PolygonType, 
                                                         rasterPointer->getSRID());
    te::gm::LinearRing* l2 = createSquare(2, -5.5, 3, rasterPointer->getSRID());
    internalPol_2->add(l2);
    polVector.push_back(internalPol_2);

    // Create internal polygon 3
    te::gm::Polygon* internalPol_3 = new te::gm::Polygon(0, te::gm::PolygonType, 
                                                         rasterPointer->getSRID());
    te::gm::LinearRing* l3 = createSquare(6, -6, 2.5, rasterPointer->getSRID());
    internalPol_3->add(l3);
    polVector.push_back(internalPol_3);

    // Create Internal polygon 4
    te::gm::Polygon* internalPol_4 = new te::gm::Polygon(0, te::gm::PolygonType, 
                                                         rasterPointer->getSRID());
    te::gm::LinearRing* l4 = new te::gm::LinearRing(6, te::gm::LineStringType, rasterPointer->getSRID());

    l4->setPoint(0, 4.5, -1.5); // starting
    l4->setPoint(1, 6.5, -2.25);
    l4->setPoint(2, 7.5, -1.75);
    l4->setPoint(3, 7.5, -0.5);
    l4->setPoint(4, 5.5, -0.5);
    l4->setPoint(5, 4.5, -1.5); // closing

    internalPol_4->add(l4);
    polVector.push_back(internalPol_4);

    // Adding internal polygons to the external polygon
    externalPol->add((te::gm::LinearRing*)l1->clone());
    externalPol->add((te::gm::LinearRing*)l2->clone());
    externalPol->add((te::gm::LinearRing*)l3->clone());
    externalPol->add((te::gm::LinearRing*)l4->clone());

    double pixelValueCounter = 0;

    for (unsigned int band = 0; band < rasterPointer->getNumberOfBands(); ++band)
    {
        for (int countPol = 0; countPol < polVector.size(); countPol++) 
        {
            te::rst::PolygonIterator< double > it =
                te::rst::PolygonIterator< double >::begin(rasterPointer.get(),
                polVector[countPol]);
            const te::rst::PolygonIterator< double > itEnd =
                te::rst::PolygonIterator< double >::end(rasterPointer.get(),
                polVector[countPol]);

            int col = 0;
            int row = 0;
            while (it != itEnd)
            {
                if (it[band] == 0)
                    rasterPointer->setValue(it.getColumn(), it.getRow(), (countPol + 1), band);
                else
                    rasterPointer->setValue(it.getColumn(), it.getRow(), 10, band);

                ++it;
            }
        }
    }

    //Copy2DiskShp(polVector, "PolygonIteratorTestPolygons");
    //te::rp::Copy2DiskRaster(*rasterPointer.get(), "./testsData/testPolygonIterator.tif");

    std::map<std::string, std::string> rasterInfo;
    rasterInfo["URI"] = TERRALIB_DATA_DIR + std::string("/testPolygonIteratorTempate.tif");

    te::rst::Raster* templateRaster = te::rst::RasterFactory::open(rasterInfo);

    for (int lin = 0; lin < templateRaster->getNumberOfRows(); lin++)
    {
      for (int col = 0; col < templateRaster->getNumberOfColumns(); col++)
      {
          double templateValue = 0, currentValue = 0;
          templateRaster->getValue(col, lin, templateValue);
          rasterPointer->getValue(col, lin, currentValue);

          CPPUNIT_ASSERT_EQUAL(templateValue, currentValue);
      }
    }
}