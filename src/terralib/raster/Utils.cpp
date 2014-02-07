/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/Utils.cpp

  \brief Utility functions for the raster module.
*/

// TerraLib
#include "../datatype/Enums.h"
#include "../geometry/Coord2D.h"
#include "BandIterator.h"
#include "Exception.h"
#include "RasterFactory.h"
#include "Utils.h"

// Boost
#include <boost/cstdint.hpp>

// STL
#include <cassert>

#include <limits>

static int sg_pixelSize[] = {  0,
                               0,
                               0,
                               sizeof(char) * 8,
                               sizeof(unsigned char),
                               sizeof(boost::int16_t),
                               sizeof(boost::uint16_t),
                               sizeof(boost::int32_t),
                               sizeof(boost::uint32_t),
                               sizeof(boost::int64_t),
                               sizeof(boost::uint64_t),
                               sizeof(bool),
                               sizeof(float),
                               sizeof(double),
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               sizeof(boost::int16_t) * 2,
                               sizeof(boost::int32_t) * 2,
                               sizeof(float) * 2,
                               sizeof(double) * 2,
                               0,
                               0,
                               0,
                               sizeof(unsigned char), // 4bits = 4/8 sizeof(unsigned char)
                               sizeof(unsigned char), // 2bits = 2/8 sizeof(unsigned char)
                               sizeof(unsigned char)  // 1bit = 1/8 sizeof(unsigned char)
                            };

int te::rst::GetPixelSize(int datatype)
{
  return sg_pixelSize[datatype];
}

std::vector<te::rst::BandProperty*> te::rst::GetBandProperties(const std::map<std::string, std::string>& rinfo)
{
  std::size_t nBands;
  int tBands;
  std::vector<te::rst::BandProperty*> bprops;

  std::map<std::string, std::string>::const_iterator it = rinfo.find("NBANDS");

  if(it == rinfo.end())
    throw Exception(("Number of bands must be set!"));

  nBands = atoi(it->second.c_str());

  it = rinfo.find("BANDSTYPE");

  if(it == rinfo.end())
    throw Exception(("Data type of bands must be set!"));

  tBands = atoi(it->second.c_str());

  for (std::size_t b = 0; b < nBands; b++)
    bprops.push_back(new te::rst::BandProperty(b, tBands));

  return bprops;
}

te::rst::Grid* te::rst::GetGrid(const std::map<std::string, std::string>& rinfo)
{
  unsigned nCols, nRows;
  double resX = 1.0, resY = 1.0, ulX =0.0, ulY=0.0;
  int srid = TE_UNKNOWN_SRS;
  te::gm::Coord2D* ulc;

  std::map<std::string, std::string>::const_iterator it = rinfo.find("NCOLS");

  if(it == rinfo.end())
    throw Exception(("Number of columns must be set!"));

  nCols = atoi(it->second.c_str());

  it = rinfo.find("NROWS");

  if(it == rinfo.end())
    throw Exception(("Number of rows must be set!"));

  nRows = atoi(it->second.c_str());

  it = rinfo.find("RESX");
  if(it != rinfo.end())
    resX = atof(it->second.c_str());

  it = rinfo.find("RESY");
  if(it != rinfo.end())
    resY = atof(it->second.c_str());

  it = rinfo.find("SRID");
  if(it != rinfo.end())
    srid = atoi(it->second.c_str());

  it = rinfo.find("ULX");
  if(it != rinfo.end())
    ulX = atof(it->second.c_str());

  it = rinfo.find("ULY");
  if(it != rinfo.end())
    ulY = atof(it->second.c_str());

  ulc = new te::gm::Coord2D(ulX, ulY);

  return (new te::rst::Grid(nCols, nRows, resX, resY, ulc, srid));
}

void te::rst::Copy(const te::rst::Raster& rin, te::rst::Raster& rout)
{
  assert(rin.getNumberOfBands() == rout.getNumberOfBands());
  assert(rout.getAccessPolicy() == te::common::RWAccess || rout.getAccessPolicy() == te::common::WAccess);

  const std::size_t nbands = rin.getNumberOfBands();
  const unsigned int nRows = rin.getNumberOfRows();
  const unsigned int nCols = rin.getNumberOfColumns();
  unsigned int row = 0;
  unsigned int col = 0;
  std::complex< double > value;

  for(std::size_t b = 0; b < nbands; b++)
  {
    if(rin.getBand(b)->getProperty()->getType() == rout.getBand(b)->getProperty()->getType())
    {
      Copy(*rin.getBand(b), *rout.getBand(b));
    }
    else
    {
      const te::rst::Band& bin = *rin.getBand(b);
      te::rst::Band& bout = *rout.getBand(b);

      for(row = 0 ; row < nRows ; ++row)
        for(col = 0 ; col < nCols ; ++col)
        {
          bin.getValue(col, row, value);

          bout.setValue(col, row, value);
        }
    }
  }
}

void te::rst::Copy(const te::rst::Band& bin, te::rst::Band& bout)
{
  assert(*bin.getRaster()->getGrid() == *bout.getRaster()->getGrid());

// when both bands have the same data type
  if (bin.getProperty()->getType() == bout.getProperty()->getType())
  {
    unsigned char* buffer = new unsigned char[bin.getBlockSize()];

    int nblocksx = bin.getProperty()->m_nblocksx;
    int nblocksy = bin.getProperty()->m_nblocksy;

    const int blkw = bin.getProperty()->m_blkw;
    const int blkh = bin.getProperty()->m_blkh;

  // when both rasters have the same block size, copy the entire blocks from in to out
    if((blkw == bout.getProperty()->m_blkw) &&
       (blkh == bout.getProperty()->m_blkh))
    {
      for(int y = 0; y < nblocksy; ++y)
      {
        for(int x = 0; x < nblocksx; ++x)
        {
          bin.read(x, y, buffer);
          bout.write(x, y, buffer);
        }
      }
    }
  // get all values from input block, and copy pixel by pixel to the output band
    else
    {
      std::complex<double> value;

      const unsigned int ncols = bin.getRaster()->getNumberOfColumns();
      const unsigned int nrows = bin.getRaster()->getNumberOfRows();

      for(int y = 0; y < nblocksy; ++y)
      {
        for(int x = 0; x < nblocksx; ++x)
        {
          unsigned int w = blkw * (x + 1);

          w = w > ncols ? ncols : w;

          unsigned int h = blkh * (y + 1);

          h = h > nrows ? nrows : h;

          for(int r = blkh * y; r < (int)h; ++r)
          {
            for(int c = blkw * x; c < (int)w; ++c)
            {
              bin.getValue(c, r, value);

              bout.setValue(c, r, value);
            }
          }
        }
      }
    }

    delete [] buffer;
  }
// if bands have different data types, use iterator
  else
  {
    te::rst::ConstBandIterator<double> rit = te::rst::ConstBandIterator<double>::begin(&bin);

    te::rst::ConstBandIterator<double> ritend = te::rst::ConstBandIterator<double>::end(&bin);

    while(rit != ritend)
    {
      bout.setValue(rit.getColumn(), rit.getRow(), *rit);

      ++rit;
    }
  }
}

void te::rst::Copy(unsigned int drow, unsigned int dcolumn, unsigned int height, unsigned int width, const Raster& rin, Raster& rout)
{
  assert(drow + height <= rin.getNumberOfRows());
  assert(dcolumn + width <= rin.getNumberOfColumns());

// define variables for interpolation
  std::vector<std::complex<double> > v;

  for (unsigned r = drow; r < drow + height; r++)
  {
    for (unsigned c = dcolumn; c < dcolumn + width; c++)
    {
      te::gm::Coord2D inputGeo = rin.getGrid()->gridToGeo(c, r);

      te::gm::Coord2D outputGrid = rout.getGrid()->geoToGrid(inputGeo.x, inputGeo.y);

      int x = Round(outputGrid.x);
      int y = Round(outputGrid.y);

      if((x >=0 && x < (int)rout.getNumberOfColumns()) && (y >=0 && y < (int)rout.getNumberOfRows()))
      {
        rin.getValues(c, r, v);
        rout.setValues(x, y, v);
      }
    }
  }
}

int te::rst::Round(double val)
{
  if (val>=0)
    return (int)(val+.5);
  else
    return (int)(val-.5);
}

te::rst::RasterPtr te::rst::CreateCopy(const te::rst::Raster& rin,
                                       const std::string& uri,
                                       const std::string &rType)
{
  std::map<std::string, std::string> rasterInfo;
  rasterInfo["URI"] = uri;

  std::vector<te::rst::BandProperty*> bandsProperties;

  unsigned int bandIndex = 0;

  for(bandIndex = 0; bandIndex < rin.getNumberOfBands(); ++bandIndex)
  {
    bandsProperties.push_back(new te::rst::BandProperty(*(rin.getBand(bandIndex )->getProperty())));
  }

  te::rst::RasterPtr outRasterPtr;

  outRasterPtr.reset( te::rst::RasterFactory::make( rType,  new te::rst::Grid(*(rin.getGrid())), bandsProperties, rasterInfo, 0, 0));

  if(outRasterPtr.get() == 0)
    return outRasterPtr;

  Copy(rin, *outRasterPtr);

  return outRasterPtr;
}

void te::rst::GetDataTypeRanges( const int& dataType, double& min, double& max )
{
  switch( dataType )
  {
    case te::dt::R4BITS_TYPE:
      min = 0;
      max = 15;
    break;

    case te::dt::R2BITS_TYPE:
      min = 0;
      max = 3;
    break;

    case te::dt::R1BIT_TYPE:
      min = 0;
      max = 1;
    break;

    case te::dt::UCHAR_TYPE:
      min = 0;
      max = 255;
    break;

    case te::dt::CHAR_TYPE:
      min = -127;
      max = 127;
    break;

    case te::dt::UINT16_TYPE:
      min = 0;
      max = (double)std::numeric_limits<unsigned short int>::max();
    break;

    case te::dt::INT16_TYPE:
      min = (double)std::numeric_limits<short int>::min();
      max = (double)std::numeric_limits<short int>::max();
    break;

    case te::dt::UINT32_TYPE:
      min = 0;
      max = (double)std::numeric_limits<unsigned int>::max();
    break;

    case te::dt::INT32_TYPE:
      min = (double)std::numeric_limits<int>::min();
      max = (double)std::numeric_limits<int>::max();
    break;

    case te::dt::FLOAT_TYPE:
      min = -(double)std::numeric_limits< float >::max();
      max = (double)std::numeric_limits< float >::max();
    break;

    case te::dt::DOUBLE_TYPE:
      min = -std::numeric_limits< double >::max();
      max = std::numeric_limits< double >::max();
    break;

    case te::dt::CINT16_TYPE:
      min = (double)std::numeric_limits<short int>::min();
      max = (double)std::numeric_limits<short int>::max();
    break;

    case te::dt::CINT32_TYPE:
      min = (double)std::numeric_limits<int>::min();
      max = (double)std::numeric_limits<int>::max();
    break;

    case te::dt::CFLOAT_TYPE:
      min = -(double)std::numeric_limits< float >::max();
      max = (double)std::numeric_limits< float >::max();
    break;

    case te::dt::CDOUBLE_TYPE:
      min = -std::numeric_limits< double >::max();
      max = std::numeric_limits< double >::max();
    break;

    default:
      throw te::rst::Exception("Invalid data type");
  }
}

void te::rst::FillRaster(te::rst::Raster* rin, const std::complex<double>& value)
{
  for (unsigned int b = 0; b < rin->getNumberOfBands(); b++)
    te::rst::FillBand(rin->getBand(b), value);
}

void te::rst::FillBand(te::rst::Band* bin, const std::complex<double>& value)
{
  for (unsigned int r = 0; r < bin->getRaster()->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < bin->getRaster()->getNumberOfColumns(); c++)
      bin->setValue(c, r, value);
}
