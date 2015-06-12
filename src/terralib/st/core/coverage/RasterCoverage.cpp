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
  \file RasterCoverage.cpp

  \brief This file contains an abstract class to represent a coverage.
*/

//TerraLib
#include "../../../datatype/AbstractData.h"
#include "../../../datatype/SimpleData.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Utils.h"
#include "../../../raster/RasterProperty.h"
#include "../../../raster/Grid.h"
#include "../../../raster/PositionIterator.h"

//Boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

//ST
#include "RasterCoverage.h"

te::st::RasterCoverage::RasterCoverage() :
  m_raster(),
  m_time(),
  m_sextent(),
  m_ptypes(),
  m_pnames()
{
}

te::st::RasterCoverage::RasterCoverage(te::rst::Raster* r, te::dt::DateTime* t) :
  m_raster(r),
  m_time(t),
  m_sextent(),
  m_ptypes(),
  m_pnames()
{
  
  m_sextent.reset(te::gm::GetGeomFromEnvelope(m_raster->getGrid()->getExtent(), m_raster->getGrid()->getSRID()));

  for(std::size_t i=0; i<m_raster->getNumberOfBands(); ++i)
    m_ptypes.push_back(m_raster->getBandDataType(i));
}

void te::st::RasterCoverage::set(te::rst::Raster* r, te::dt::DateTime* t)
{
  m_raster.reset(r);
  m_time.reset(t);
  m_ptypes.clear();
  m_pnames.clear();

  m_sextent.reset(te::gm::GetGeomFromEnvelope(m_raster->getGrid()->getExtent(), m_raster->getGrid()->getSRID()));
 
  for(std::size_t i=0; i<m_raster->getNumberOfBands(); ++i)
    m_ptypes.push_back(m_raster->getBandDataType(i));
}

te::rst::Raster* te::st::RasterCoverage::getRaster()
{
  return m_raster.get();
}

te::dt::DateTime* te::st::RasterCoverage::getTime()
{
  return m_time.get();
}

te::st::Coverage* te::st::RasterCoverage::clone() const
{
   return new te::st::RasterCoverage( static_cast<te::rst::Raster*>(m_raster->clone()), 
                                      static_cast<te::dt::DateTime*>(m_time->clone()));
}

te::st::CoverageType te::st::RasterCoverage::getType() const
{
  return RASTER_COVERAGE;
}
        
te::gm::Geometry* te::st::RasterCoverage::getSpatialExtent() const
{
  return m_sextent.get();
}

te::dt::DateTime* te::st::RasterCoverage::getTime() const
{
  return m_time.get();
}

unsigned int te::st::RasterCoverage::getNumberOfProperties() const
{
  return m_raster->getNumberOfBands(); 
}

const std::vector<int>& te::st::RasterCoverage::getPropertyTypes() const
{
  return m_ptypes;
}

const std::vector<std::string>& te::st::RasterCoverage::getPropertyNames() const
{
  return m_pnames;
}

void te::st::RasterCoverage::getValue(const te::gm::Point& l, boost::ptr_vector<te::dt::AbstractData>& result) const
{
  std::vector<double> aux;
  getDouble(l,aux);
  for(unsigned int i = 0; i<aux.size(); ++i)
    result.push_back(new te::dt::Double(aux[i]));
  return;
}

std::auto_ptr<te::dt::AbstractData> te::st::RasterCoverage::getValue(const te::gm::Point& l, unsigned int p) const
{
  double result = getDouble(l,p);
  return std::auto_ptr<te::dt::AbstractData>(new te::dt::Double(result)); 
}

void te::st::RasterCoverage::getValue( const te::gm::Polygon& l, unsigned int p, 
                                      boost::ptr_vector<te::dt::AbstractData>& result) const
{
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(m_raster.get(), &l);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(m_raster.get(), &l);
  while(it!=itend)
  {
    double value = it[p];
    result.push_back(std::auto_ptr<te::dt::AbstractData> (new te::dt::Double(value))); 
    ++it;
  }
  return;
}

void te::st::RasterCoverage::getValue( const te::gm::Polygon& l, 
                                      boost::ptr_vector<te::dt::AbstractData>& result) const
{
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(m_raster.get(), &l);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(m_raster.get(), &l);
  while(it!=itend)
  {
    std::vector<double> values = *it;
    for(unsigned int i=0; i<values.size(); ++i)
      result.push_back(std::auto_ptr<te::dt::AbstractData> (new te::dt::Double(values[i]))); 
    ++it;
  }
  return;
}

void te::st::RasterCoverage::getInt(const te::gm::Point& l, std::vector<int>& result) const
{
  std::vector<double> aux;
  getDouble(l,aux);
  for(unsigned int i = 0; i<aux.size(); ++i)
    result.push_back((int)aux[i]);
  return;
}

int te::st::RasterCoverage::getInt(const te::gm::Point& l, unsigned int p) const
{
  return (int)getDouble(l,p);
}

void te::st::RasterCoverage::getInt(const te::gm::Polygon& l, unsigned int p, std::vector<int>& result) const
{
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(m_raster.get(), &l);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(m_raster.get(), &l);
  while(it!=itend)
  {
    result.push_back((int)it[p]); 
    ++it;
  }
  return;
}

void te::st::RasterCoverage::getInt(const te::gm::Polygon& l, std::vector<int>& result) const
{
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(m_raster.get(), &l);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(m_raster.get(), &l);
  while(it!=itend)
  {
    std::vector<double> values = *it;
    for(unsigned int i=0; i<values.size(); ++i)
      result.push_back((int)values[i]); 
    ++it;
  }
  return;
}
        
void te::st::RasterCoverage::getDouble(const te::gm::Point& l, std::vector<double>& result) const
{
  te::gm::Coord2D coord = m_raster->getGrid()->geoToGrid(l.getX(), l.getY());
  m_raster->getValues((unsigned int)coord.x, (unsigned int)coord.y, result);
  return;
}

double te::st::RasterCoverage::getDouble(const te::gm::Point& l, unsigned int p) const
{
  te::gm::Coord2D coord = m_raster->getGrid()->geoToGrid(l.getX(), l.getY());
  double result;
  m_raster->getValue((unsigned int)coord.x, (unsigned int)coord.y, result, p);
  return result;
}

void te::st::RasterCoverage::getDouble(const te::gm::Polygon& l, unsigned int p, std::vector<double>& result) const
{
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(m_raster.get(), &l);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(m_raster.get(), &l);
  while(it!=itend)
  {
    result.push_back(it[p]); 
    ++it;
  }
  return;
}

void te::st::RasterCoverage::getDouble(const te::gm::Polygon& l, std::vector<double>& result) const
{
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(m_raster.get(), &l);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(m_raster.get(), &l);
  while(it!=itend)
  {
    std::vector<double> values = *it;
    for(unsigned int i=0; i<values.size(); ++i)
      result.push_back(values[i]); 
    ++it;
  }
  return;
}

std::auto_ptr<te::rst::Raster> te::st::RasterCoverage::getRaster() const
{
  return std::auto_ptr<te::rst::Raster>(static_cast<te::rst::Raster*>(m_raster->clone()));
}

std::auto_ptr<te::rst::Raster> te::st::RasterCoverage::getRaster(unsigned int /*p*/) const
{
  //falta indicar a resolução do raster que será gerado, etc, etc...
  return std::auto_ptr<te::rst::Raster>();
}

te::st::RasterCoverage::~RasterCoverage()
{
}
 



