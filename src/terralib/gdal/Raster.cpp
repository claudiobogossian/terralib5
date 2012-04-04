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
  \file Raster.cpp

  \brief This is a class that represents a GDAL Raster.
 */

// TerraLib
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../datatype/Enums.h"
#include "../datatype/SimpleData.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Utils.h"
#include "../srs/Converter.h"
#include "../srs/CoordinateSystemFactory.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"
#include "Utils.h"

// STL
#include <cassert>
#include <limits>
#include <stdint.h>

// GDAL
#include <gdal_priv.h>
#include <ogr_spatialref.h>

// Boost
#include <boost/lexical_cast.hpp>

te::gdal::Raster::Raster()
  : te::rst::Raster(0),
    m_gdataset(0)
{
}

te::gdal::Raster::Raster(const std::string& rinfo, te::common::AccessPolicy p)
  : te::rst::Raster(0, p),
    m_gdataset(0)
{
  GDALAllRegister();

  m_gdataset = te::gdal::GetRasterHandle(rinfo, m_policy);

  if(m_gdataset == 0)
    throw Exception(TR_GDAL("Data file can not be accessed."));

  m_grid = GetGrid(m_gdataset);

  GetBands(this, m_bands);
}

te::gdal::Raster::Raster(te::rst::Grid* grid,
                         const std::vector<te::rst::BandProperty*>& bprops,
                         const std::map<std::string, std::string>& optParams,
                         te::common::AccessPolicy p)
  : te::rst::Raster(grid, p)
{
  create(grid, bprops, optParams, 0, 0);
}

te::gdal::Raster::Raster(GDALDataset* gdataset, te::common::AccessPolicy p)
  : te::rst::Raster(0, p),
    m_gdataset(gdataset)
{
  m_grid = GetGrid(m_gdataset);

  GetBands(this, m_bands);

  m_name = m_gdataset->GetDescription();
}

te::gdal::Raster::Raster(const Raster& rhs)
  : te::rst::Raster(rhs),
    m_gdataset(rhs.m_gdataset),
    m_bands(rhs.m_bands)
{
}

te::gdal::Raster::~Raster()
{
  te::common::FreeContents(m_bands);

  if (m_gdataset)
  {
    std::string driverName = GetDriverName(m_gdataset->GetDescription());

    if ((driverName == "PNG" || driverName == "JPEG") &&
        (m_policy == te::common::WAccess || m_policy == te::common::RWAccess))
    {
      char** papszOptions = 0;

      GDALDriver* driverPtr = GetGDALDriverManager()->GetDriverByName(driverName.c_str());

      GDALDataset* poDataset = driverPtr->CreateCopy(m_gdataset->GetDescription(),
                                                     m_gdataset,
                                                     0,
                                                     papszOptions,
                                                     NULL,
                                                     NULL);
      GDALClose(poDataset);
    }

    GDALClose(m_gdataset);
  }

  if(m_deleter)
  {
    // deleting who?
    m_deleter = 0;
  }

}

void te::gdal::Raster::open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p)
{
  std::map<std::string, std::string>::const_iterator it = rinfo.find("URI");

// if URI is not specified, let's look for SOURCE
  if(it == rinfo.end())
  {
    it = rinfo.find("SOURCE");

    if(it == rinfo.end())
      throw Exception(TR_GDAL("At least the URI or SOURCE parameter must be informed!"));
  }

  m_gdataset = GetRasterHandle(it->second, p);

  if(m_gdataset == 0)
    throw Exception(TR_GDAL("Data file can not be accessed."));

  m_grid = GetGrid(m_gdataset);

  m_policy = p;

  GetBands(this, m_bands);

  m_name = m_gdataset->GetDescription();
}

std::map<std::string, std::string> te::gdal::Raster::getInfo() const
{
  return std::map<std::string, std::string>();
}

std::size_t te::gdal::Raster::getNumberOfBands() const
{
  return m_bands.size();
}

int te::gdal::Raster::getBandDataType(std::size_t i) const
{
  assert(i < m_bands.size());

  return m_bands[i]->getProperty()->getType();
}

const te::rst::Band* te::gdal::Raster::getBand(std::size_t i) const
{
  assert(i < m_bands.size());

  return m_bands[i];
}

te::rst::Band* te::gdal::Raster::getBand(std::size_t i)
{
  assert(i < m_bands.size());

  return m_bands[i];
}

const te::rst::Band& te::gdal::Raster::operator[](std::size_t i) const
{
  assert(i < m_bands.size());

  return *m_bands[i];
}

te::rst::Band& te::gdal::Raster::operator[](std::size_t i)
{
  assert(i < m_bands.size());

  return *m_bands[i];
}

GDALDataset* te::gdal::Raster::getGDALDataset()
{
  return m_gdataset;
}

te::dt::AbstractData* te::gdal::Raster::clone() const
{
  return new Raster(*this);
}

te::gdal::Raster& te::gdal::Raster::operator=(const te::gdal::Raster& rhs)
{
  te::rst::Raster::operator=(rhs);

  for (std::size_t b = 0; b < rhs.getNumberOfBands(); b++)
    static_cast<te::gdal::Band*>(m_bands[b])->operator=(*static_cast<te::gdal::Band*>(rhs.m_bands[b]));

  return *this;
}

te::rst::Raster* te::gdal::Raster::resample(int method, int scale, const std::map<std::string, std::string>& rinfo)
{
  assert(scale != 0);

  if (!(scale < 0 && method == 3))
    return te::rst::Raster::resample(method, scale, rinfo);

// create output parameters and raster
  te::rst::Grid* grid = new te::rst::Grid(*getResampledGrid(scale));

  std::vector<te::rst::BandProperty*> bands;

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    bands.push_back(new te::rst::BandProperty(*(getBand(b)->getProperty())));

  te::rst::Raster* rout = te::rst::RasterFactory::make(grid, bands, rinfo);

  int overviewScale[1] = { -scale };

  GDALDataset* inds = static_cast<te::gdal::Raster*>(this)->getGDALDataset();

  GDALDataset* outds = static_cast<te::gdal::Raster*>(rout)->getGDALDataset();

  int overviewIndex = -1;
  for (int ov = 0; ov < inds->GetRasterBand(1)->GetOverviewCount(); ov++)
    if (inds->GetRasterBand(1)->GetOverview(ov)->GetXSize() == outds->GetRasterBand(1)->GetXSize())
    {
      overviewIndex = ov;
      ov = inds->GetRasterBand(1)->GetOverviewCount();
    }
      
  if (overviewIndex == -1)
  {
    inds->BuildOverviews("CUBIC", 1, overviewScale, 0, NULL, GDALDummyProgress, NULL);

    overviewIndex = inds->GetRasterBand(1)->GetOverviewCount() - 1;
  }

  GByte* buffer = (GByte*) malloc(outds->GetRasterXSize() * outds->GetRasterYSize() * sizeof(GByte*));

  double geoT[6];
  outds->GetGeoTransform(geoT);
  outds->SetGeoTransform(geoT);

  for (int b = 0; b < inds->GetRasterCount(); b++)
  {
    GDALRasterBand* outband = outds->GetRasterBand(b + 1);

    GDALRasterBand* inband = inds->GetRasterBand(b + 1)->GetOverview(overviewIndex);

    inband->RasterIO(GF_Read, 0, 0, inband->GetXSize(), inband->GetYSize(), 
                      buffer, inband->GetXSize(), inband->GetYSize(), GDT_Byte, 0, 0);

    outband->RasterIO(GF_Write, 0, 0, inband->GetXSize(), inband->GetYSize(), 
                      buffer, inband->GetXSize(), inband->GetYSize(), GDT_Byte, 0, 0);
  }

  return rout;
}

te::rst::Raster* te::gdal::Raster::transform(int srid, double llx, double lly, double urx, double ury, double resx, double resy, const std::map<std::string, std::string>& rinfo)
{
  if (srid == getSRID())
    return 0;

  if (!te::gdal::RecognizesSRID(srid))
    throw Exception(TR_GDAL("Output SRID not recognized! Expecting a EPSG SRS id."));

  unsigned int ncols = getNumberOfColumns();
  unsigned int nrows = getNumberOfRows();

  te::gm::Envelope* roi = new te::gm::Envelope(llx, lly, urx, ury);
  if (!roi->isValid())
  {
    delete roi;

    roi = 0;
  }
  else
  {
    ncols = static_cast<unsigned int>((urx-llx)/getResolutionX())+1;

    nrows = static_cast<unsigned int>((ury-lly)/getResolutionY())+1;
  }

  te::gm::Envelope* env = this->getExtent(srid, roi);
  delete roi;

  if (resx == 0 || resy == 0) // maintain the same number of pixels
  {
    resx = env->getWidth()/ncols;

    resy = env->getHeight()/nrows;
  }
  else
  {
    ncols = static_cast<unsigned int>(env->getWidth()/resx) + 1;

    nrows = static_cast<unsigned int>(env->getHeight()/resy) + 1;
  }

  te::rst::Grid* g = new te::rst::Grid(ncols, nrows, resx, resy, env, srid);

  // copy the band definitions
  std::vector<te::rst::BandProperty*> bands;
  for (unsigned int b=0; b<this->getNumberOfBands(); ++b)
  {
    te::rst::BandProperty* bb = new te::rst::BandProperty(*this->getBand(b)->getProperty());

    bands.push_back(bb);
  }

  // create output raster
  te::rst::Raster* rout = te::rst::RasterFactory::make(g, bands, rinfo);

  // calls the reprojection algorithm
  if (te::gdal::ReprojectRaster(this, rout))
    return rout;

  return 0;
}

void te::gdal::Raster::transform(te::rst::Raster* outRaster)
{
  te::gdal::ReprojectRaster(this, outRaster);
}

void te::gdal::Raster::create(te::rst::Grid* g,
                             const std::vector<te::rst::BandProperty*> bands,
                             const std::map<std::string, std::string>& rinfo,
                             void* h, void (*deleter)(void*))
{
  m_grid = g;

  m_deleter = deleter;

// always assume that a created raster needs to be written
  m_policy = te::common::RWAccess;

  if (h)
  {
    intptr_t buffaddress = (intptr_t) h;

    std::string memraster  = "MEM:::DATAPOINTER=";
                memraster += boost::lexical_cast<std::string>(buffaddress);
                memraster += ",PIXELS=";
                memraster += te::common::Convert2String(m_grid->getNumberOfColumns());
                memraster += ",LINES=";
                memraster += te::common::Convert2String(m_grid->getNumberOfRows());
                memraster += ",BANDS=";
                memraster += boost::lexical_cast<std::string>(bands.size());
                memraster += ",DATATYPE=";
                memraster += GDALGetDataTypeName(GetGDALDataType(bands[0]->getType()));

    m_gdataset = GetRasterHandle(memraster.c_str(), m_policy);
  }
  else
  {
    m_gdataset = te::gdal::CreateRaster(g, bands, rinfo);

    te::common::FreeContents(bands);

    if (!m_gdataset)
    {
      std::string mess = TR_GDAL("Raster couldn't be created:");
      mess += m_name;
      throw Exception(mess);
    }

    GetBands(this, m_bands);
  }
}
