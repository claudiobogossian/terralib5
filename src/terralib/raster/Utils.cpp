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
  \file terralib/raster/Utils.cpp
 
  \brief Utility functions for the raster module.
*/

// TerraLib
#include "../datatype/Enums.h"
#include "../geometry/Coord2D.h"
#include "Exception.h"
#include "Utils.h"

// Boost
#include <boost/cstdint.hpp>

// STL
#include <cassert>

static int sg_pixelSize[] = {  0,
                               0,
                               0,
                               sizeof(char),
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
                               0
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
  double resX, resY, ulX, ulY;
  int srid;
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

  if(it == rinfo.end())
    resX = 1.0;

  resX = atof(it->second.c_str());

  it = rinfo.find("RESY");

  if(it == rinfo.end())
    resY = 1.0;

  resY = atof(it->second.c_str());

  it = rinfo.find("SRID");

  if(it == rinfo.end())
    srid = -1;

  srid = atoi(it->second.c_str());

  it = rinfo.find("ULX");

  if(it == rinfo.end())
    ulX = 0.0;

  ulX = atof(it->second.c_str());

  it = rinfo.find("ULY");

  if(it == rinfo.end())
    ulY = 0.0;

  ulY = atof(it->second.c_str());

  ulc = new te::gm::Coord2D(ulX, ulY);

  return (new te::rst::Grid(nCols, nRows, resX, resY, ulc, srid));
}

void te::rst::Copy(const te::rst::Raster& rin, te::rst::Raster& rout)
{
  assert(rin.getNumberOfBands() == rout.getNumberOfBands());

  const std::size_t nbands = rin.getNumberOfBands();

  for(std::size_t b = 0; b < nbands; b++)
  {
    Copy(*rin.getBand(b), *rout.getBand(b));
  }
}

void te::rst::Copy(const te::rst::Band& bin, te::rst::Band& bout)
{
  assert(*bin.getRaster()->getGrid() == *bout.getRaster()->getGrid());
  assert(bin.getProperty()->getType() == bout.getProperty()->getType());

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
