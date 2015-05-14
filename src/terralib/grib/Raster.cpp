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
  \file terralib/grib/Raster.cpp

  \brief A raster class for Grib.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"
#include "Enums.h"

// STL
#include <cassert>
#include <cmath>

// Boost
#include <boost/format.hpp>

grib_context* te::grib::Raster::sm_context(0);

te::grib::Raster::Raster()
  : m_file(0)
{
// get the context
  if(!sm_context)
    sm_context = grib_context_get_default();
}

/*
te::grib::Raster::Raster(te::rst::Grid* grid, te::common::AccessPolicy p)
  : te::rst::Raster(grid, p),
    m_file(0)
{
  throw Exception(TR_GRIB("Not implemented yet!"));
}
*/

te::grib::Raster::Raster(const Raster& rhs)
  : te::rst::Raster(rhs),
    m_file(0)
{
  throw Exception(TR_GRIB("Not implemented yet!"));
}

te::grib::Raster::~Raster()
{
  close();
}

const double TeEARTHRADIUS = 6378160.;

void te::grib::Raster::open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p)
{
// assure that raster is closed
  close();

// extract file name
  std::map<std::string, std::string>::const_iterator it = rinfo.find("URI");

  if ( it == rinfo.end() )
    throw Exception(TR_GRIB("Missing Grib filename"));

// open file
  std::string file_name = it->second;

  if(p == te::common::RAccess)
    m_file = fopen(file_name.c_str(), "r");
  else
    m_file = fopen(file_name.c_str(), "rw");

  if(m_file == 0)
    throw Exception((boost::format(TR_GRIB("Grib file can not be opened: %1%")) %file_name).str());

  int err = 0;

// for each message extract the equivalent band information
  grib_handle* handle = 0;

  int bandId = 0;

  while((handle = grib_handle_new_from_file(sm_context, m_file, &err)) != 0)
  {
    Band* band = new Band(this, bandId, handle);

    m_bands.push_back(band);

    ++bandId;
  }

  if(err)
    throw Exception((boost::format(TR_GRIB("Can not create Grib handle for file %1% due to the following error: %2%.")) % file_name % Band::getErrMsg(err)).str());

// get grid info from the first message (band)

// find projection type
  int gtype = m_bands[0]->getLong("dataRepresentationType");
  if(gtype == SATELLITE_REP)       //Satellite
    setGridSatelliteRep();
  else if (gtype == LATLNG_REP)    //Latlong
    setGridLatLngRep();
  else if (gtype == REDUCEDGG_REP) //Reduced gaussian grid
    setGridReducedGGRep();
  else  // if projection is not known, let's throw an exception! note: may be -1 could be used instead!
  {
    throw Exception((boost::format(TR_GRIB("Could not determine the data projection in the grib file: %1%.")) % file_name).str());
  }
}

void te::grib::Raster::open(void* h, const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p, void (*deleter)(void*))
{
  throw Exception(TR_GRIB("Not implemented yet!"));
  // http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:designimplementation:dataaccess:drivers:memory
}

std::map<std::string, std::string> te::grib::Raster::getInfo() const
{
  return std::map<std::string, std::string>();
}

std::size_t te::grib::Raster::getNumberOfBands() const
{
  return m_bands.size();
}

int te::grib::Raster::getBandDataType(std::size_t i) const
{
  assert(i < m_bands.size());

  te::rst::BandProperty* bp = m_bands[i]->getProperty();

  return bp->getType();
}

const te::rst::Band* te::grib::Raster::getBand(std::size_t i) const
{
  return m_bands[i];
}

te::rst::Band* te::grib::Raster::getBand(std::size_t i)
{
  return m_bands[i];
}

const te::rst::Band& te::grib::Raster::operator[](std::size_t i) const
{
  return *m_bands[i];
}

te::rst::Band& te::grib::Raster::operator[](std::size_t i)
{
  return *m_bands[i];
}

te::dt::AbstractData* te::grib::Raster::clone() const
{
  throw Exception(TR_GRIB("Not implemented yet!"));
}

void te::grib::Raster::close()
{
  te::common::FreeContents(m_bands);

  m_bands.clear();

  if(m_file)
  {
    fclose(m_file);
    m_file = 0;
  }

  delete m_grid;
  m_grid = 0;
}

void te::grib::Raster::setGridSatelliteRep()
{
  // number of rows and cols
  long nx = m_bands[0]->getLong("numberOfPointsAlongXAxis");
  long ny = m_bands[0]->getLong("numberOfPointsAlongYAxis");

  // altitude in number of radius of earth
  double altitude = m_bands[0]->getDouble("NrInRadiusOfEarth") * 0.000001;

  double pri = 2. * asin(1. / altitude) / m_bands[0]->getDouble("dy");
  double prj = 2.0* asin(1. / altitude) / m_bands[0]->getDouble("dx");

  // resolution
  double resx = (double)(std::atan(std::tan(pri) * (altitude - 1.) ) * TeEARTHRADIUS);
  double resy = (double)(std::atan(std::tan(prj) * (altitude - 1.) ) * TeEARTHRADIUS);

  // offset in image coordinates
  double offx = m_bands[0]->getDouble("xCoordinateOfOriginOfSectorImage");
  double offy = m_bands[0]->getDouble("yCoordinateOfOriginOfSectorImage");

  // bounding box coordinates
  double ulx = offx * resx;
  double uly = -(offy * resy);
  double lrx = (offx + nx - 1) * resx;
  double lry = -(offy + ny - 1) * resy;

  // find srid = ???
  int srid = -1;

  // bounding box
  te::gm::Envelope* e = new te::gm::Envelope(ulx, lry, lrx, uly);

  // upper-left corner
  //te::gm::Coord2D ulc(ulx, uly);

  // grid
  m_grid = new te::rst::Grid(nx, ny, resx, resy, e, srid);

  return;
}

void te::grib::Raster::setGridLatLngRep()
{
  // number of rows and cols
  long nx = m_bands[0]->getLong("numberOfPointsAlongAParallel");
  long ny = m_bands[0]->getLong("numberOfPointsAlongAMeridian");

  // bounding box coordinates
  double north = m_bands[0]->getDouble("latitudeOfFirstGridPointInDegrees");
  double west  = m_bands[0]->getDouble("longitudeOfFirstGridPointInDegrees");
  double south = m_bands[0]->getDouble("latitudeOfLastGridPointInDegrees");;
  double east  = m_bands[0]->getDouble("longitudeOfLastGridPointInDegrees");;
  //longitudesSanityCheck(west, east);

  // resolution
  double resx = (east-west)   / (nx-1);
  double resy = (north-south) / (ny-1);

  // find srid = ???
  int srid = -1;

  // bounding box
  te::gm::Envelope* e = new te::gm::Envelope(west, south, east, north);

  // upper-left corner
  //te::gm::Coord2D ulc(west, north);

  // grid
  m_grid = new te::rst::Grid(nx, ny, resx, resy, e, srid);

  return;
}

void te::grib::Raster::setGridReducedGGRep()
{

}
