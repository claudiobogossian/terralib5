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
  \file terralib/raster/Utils.cpp

  \brief Utility functions for the raster module.
*/

// TerraLib
#include "../datatype/Enums.h"
#include "../geometry/Coord2D.h"
#include "../common/MathUtils.h"
#include "BandIterator.h"
#include "Exception.h"
#include "PositionIterator.h"
#include "RasterFactory.h"
#include "Utils.h"

// Boost
#include <boost/cstdint.hpp>
#include <boost/random.hpp>
#include <boost/random/uniform_int_distribution.hpp>

// STL
#include <cassert>
#include <limits>
#include <vector>

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

te::rst::RasterPtr te::rst::CropRaster(const te::rst::Raster& rin, const te::gm::Polygon& pin, const std::map<std::string, std::string>& rinfo, const std::string& rType)
{
  std::vector< te::gm::Geometry const *> geometries;
  geometries.push_back( &pin );  
  
  return te::rst::RasterPtr( CropRaster( rin, geometries, rinfo, rType ).release() );
} 

std::unique_ptr< te::rst::Raster > te::rst::CropRaster(const te::rst::Raster& rin, 
  const std::vector< te::gm::Geometry const *> geometries,
  const std::map<std::string, std::string>& rinfo,
  const std::string& rType)
{
  std::unique_ptr< te::rst::Raster > outRasterPtr;
  
  // Guessing the output envelope
  // Checking input geometries
  
  te::gm::Envelope outEnvelope;
  
  {
    outEnvelope.m_llx = outEnvelope.m_lly = std::numeric_limits< double >::max();
    outEnvelope.m_urx = outEnvelope.m_ury = -1.0 * std::numeric_limits< double >::max();
    
    const std::size_t geometriesSize = geometries.size();
    std::size_t geometriesIdx = 0;
    te::gm::Geometry const * gPtr = 0;
    te::gm::Envelope const * gEnvelopePtr = 0;
    
    for( geometriesIdx = 0 ; geometriesIdx < geometriesSize ; ++geometriesIdx )
    {
      gPtr = geometries[ geometriesIdx ];
      if(gPtr == 0 )
      {
        return outRasterPtr;
      }
      
      if( 
          ( gPtr->getGeomTypeId() != te::gm::PolygonType )
          &&
          ( gPtr->getGeomTypeId() != te::gm::MultiPolygonType )
        )
      {
        return outRasterPtr;
      }
      
      gEnvelopePtr = gPtr->getMBR();
      
      outEnvelope.m_llx = std::min( outEnvelope.m_llx, gEnvelopePtr->m_llx );
      outEnvelope.m_lly = std::min( outEnvelope.m_lly, gEnvelopePtr->m_lly );
      outEnvelope.m_urx = std::max( outEnvelope.m_urx, gEnvelopePtr->m_urx );
      outEnvelope.m_ury = std::max( outEnvelope.m_ury, gEnvelopePtr->m_ury );
    }
  }
  
  // Creating the output raster
  
  unsigned int firstInputRow = 0;
  unsigned int firstInputCol = 0;
  
  {
    te::gm::Coord2D cllenv(rin.getGrid()->geoToGrid(outEnvelope.m_llx, 
      outEnvelope.m_lly));
    te::gm::Coord2D curenv(rin.getGrid()->geoToGrid(outEnvelope.m_urx, 
      outEnvelope.m_ury));
    
    firstInputRow =
      te::common::Round< double, unsigned int >(
        std::min( 
          (double)( rin.getGrid()->getNumberOfRows() - 1 )
          ,
          std::max( 
            0.0
            , 
            curenv.getY()
          )
        )
      );
    firstInputCol = 
      te::common::Round< double, unsigned int >(
        std::min( 
          (double)( rin.getGrid()->getNumberOfColumns() - 1 )
          ,
          std::max( 
            0.0
            , 
            cllenv.getX()
          )
        )
      );
    const unsigned int lastInputRow =
      te::common::Round< double, unsigned int >(
        std::min( 
          (double)( rin.getGrid()->getNumberOfRows() - 1 )
          ,
          std::max( 
            0.0
            , 
            cllenv.getY()
          )
        )
      );
    const unsigned int lastInputCol =
      te::common::Round< double, unsigned int >(
        std::min( 
          (double)( rin.getGrid()->getNumberOfColumns() - 1 )
          ,
          std::max( 
            0.0
            , 
            curenv.getX()
          )
        )
      ); 
    
    if( ( lastInputRow <= firstInputRow ) || ( lastInputCol <= firstInputCol ) )
    {
      return outRasterPtr;
    }
    
    const unsigned int outputWidth = lastInputCol - firstInputCol + 1;
    const unsigned int outputHeight = lastInputRow - firstInputRow + 1;
    
    if( ( outputHeight == 0 ) || ( outputHeight == 0 ) )
    {
      return outRasterPtr;
    }
    
    te::gm::Coord2D ulc( rin.getGrid()->gridToGeo( ((double)firstInputCol) - 0.5,
      ((double)firstInputRow) - 0.5 ) );
    
    te::rst::Grid* grid = new te::rst::Grid( outputWidth, outputHeight, 
      rin.getResolutionX(), rin.getResolutionY(), &ulc, rin.getSRID() );

    std::vector<te::rst::BandProperty*> bands;

    for (std::size_t b = 0; b < rin.getNumberOfBands(); b++)
    {
      bands.push_back(new te::rst::BandProperty(*(rin.getBand(b)->getProperty())));
      bands[ b ]->m_nblocksx = 1;
      bands[ b ]->m_nblocksy = outputHeight;
      bands[ b ]->m_blkw = outputWidth;
      bands[ b ]->m_blkh = 1;
    }

    outRasterPtr.reset( te::rst::RasterFactory::make(rType, grid, bands, rinfo, 0, 0) );  
    if( outRasterPtr.get() == 0 )
    {
      return outRasterPtr;
    }
  }
  
  // Data copy
  
  {
    const std::size_t geometriesSize = geometries.size();
    std::size_t geometriesIdx = 0;
    std::vector< te::gm::Geometry * > singleGgeomsPtrs;
    std::size_t singleGgeomsPtrsIdx = 0;
    std::size_t singleGgeomsPtrsSize = 0;
    std::vector<std::complex<double> > values;
    unsigned int outRow = 0;
    unsigned int outCol = 0;
    
    for( geometriesIdx = 0 ; geometriesIdx < geometriesSize ; ++geometriesIdx )
    {
      singleGgeomsPtrs.clear();
      te::gm::Multi2Single( (te::gm::Geometry*)geometries[ geometriesIdx ], singleGgeomsPtrs );
      
      singleGgeomsPtrsSize = singleGgeomsPtrs.size();
      
      for( singleGgeomsPtrsIdx = 0 ; singleGgeomsPtrsIdx < singleGgeomsPtrsSize ;
        ++singleGgeomsPtrsIdx )
      {
        assert( singleGgeomsPtrs[ singleGgeomsPtrsIdx ]->getGeomTypeId() == 
          te::gm::PolygonType );
        
        PolygonIterator< double > it = PolygonIterator< double >::begin( 
          outRasterPtr.get(), (te::gm::Polygon*)singleGgeomsPtrs[ singleGgeomsPtrsIdx ] );
        PolygonIterator< double > itEnd = PolygonIterator< double >::end( 
          outRasterPtr.get(), (te::gm::Polygon*)singleGgeomsPtrs[ singleGgeomsPtrsIdx ] );
        
        while( it != itEnd )
        {
          outRow = it.getRow();
          outCol = it.getColumn();
          
          rin.getValues( outCol + firstInputCol, outRow + firstInputRow, values );
          outRasterPtr->setValues( outCol, outRow, values );
          
          ++it;
        }
      }
    }
  }
  
  return outRasterPtr;
}

std::vector<te::gm::Point*> te::rst::GetRandomPointsInRaster(const te::rst::Raster& inputRaster, unsigned int numberOfPoints)
{
  std::vector<te::gm::Point*> randomPoints;
  double randX;
  double randY;

  boost::random::mt19937 generator((boost::random::mt19937::result_type) time(0));
  boost::random::uniform_int_distribution<> random_rows(0, inputRaster.getNumberOfRows() - 1);
  boost::random::uniform_int_distribution<> random_columns(0, inputRaster.getNumberOfColumns() - 1);

  for (unsigned int p = 0; p < numberOfPoints; p++)
  {
    inputRaster.getGrid()->gridToGeo(random_columns(generator), random_rows(generator), randX, randY);
    randomPoints.push_back(new te::gm::Point(randX, randY, inputRaster.getSRID()));
  }

  return randomPoints;
}

std::string te::rst::ConvertColorInterpTypeToString(const te::rst::ColorInterp& ci)
{
  if(ci == te::rst::UndefCInt)
    return TE_TR("Undefined");
  else if(ci == te::rst::GrayIdxCInt)
    return TE_TR("Gray");
  else if(ci == te::rst::PaletteIdxCInt)
    return TE_TR("Palette");
  else if(ci == te::rst::RedCInt)
    return TE_TR("Red");
  else if(ci == te::rst::GreenCInt)
    return TE_TR("Green");
  else if(ci == te::rst::BlueCInt)
    return TE_TR("Blue");
  else if(ci == te::rst::AlphaCInt)
    return TE_TR("Alpha");
  else if(ci == te::rst::HueCInt)
    return TE_TR("Hue");
  else if(ci == te::rst::SatCInt)
    return TE_TR("Saturation");
  else if(ci == te::rst::LigCInt)
    return TE_TR("Lightness");
  else if(ci == te::rst::CyanCInt)
    return TE_TR("Cyan");
  else if(ci == te::rst::MagentaCInt)
    return TE_TR("Magenta");
  else if(ci == te::rst::YellowCInt)
    return TE_TR("Yellow");
  else if(ci == te::rst::KeyCInt)
    return TE_TR("Key");
  else if(ci == te::rst::YCInt)
    return TE_TR("Y");
  else if(ci == te::rst::CbCInt)
    return TE_TR("Cb");
  else if(ci == te::rst::CrCInt)
    return TE_TR("Cr");

  return "";
}

std::string te::rst::ConvertPalleteInterpTypeToString(const te::rst::PaletteInterpretation& pi)
{
  if(pi == te::rst::UndefPalInt)
    return TE_TR("Undefined");
  else if(pi == te::rst::GrayPalInt)
    return TE_TR("Gray");
  else if(pi == te::rst::RGBPalInt)
    return TE_TR("RGB");
  else if(pi == te::rst::CMYKPalInt)
    return TE_TR("CMYK");
  else if(pi == te::rst::HSLPalInt)
    return TE_TR("HSL");

  return "";
}
