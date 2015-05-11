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
  \file PointCoverage.cpp

  \brief This file contains an abstract class to represent a coverage.
*/

//TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/AbstractData.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Point.h"
#include "../../../raster/Raster.h"

//Boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

//ST
#include "PointCoverage.h"
#include "../interpolator/AbstractPointCoverageInterp.h"
#include "../interpolator/IDWInterpolator.h"

te::st::PointCoverage::PointCoverage():
  m_sextent(0),
  m_textent(0),
  m_observations(),
  m_interpolator(&IDWInterpolator::getInstance()),
  m_rTree(),
  m_np(0),
  m_ptypes(),
  m_pnames(),
  m_tp(-1)
{
  //create the internal empty RTree 
  m_rTree.reset(new te::sam::rtree::Index<std::size_t>());
}

te::st::PointCoverage::PointCoverage(AbstractPointCoverageInterp* interp, te::gm::Geometry* sextent, 
                      te::dt::DateTime*  textent, int np, const std::vector<int>& ptypes, 
                      const std::vector<std::string>& pnames) :
  m_sextent(sextent),
  m_textent(textent),
  m_observations(),
  m_interpolator(interp),
  m_rTree(),
  m_np(np),
  m_ptypes(),
  m_pnames(),
  m_tp(-1)
{
  //create the internal empty RTree 
  m_rTree.reset(new te::sam::rtree::Index<std::size_t>());
}

  te::st::PointCoverage::PointCoverage(AbstractPointCoverageInterp* interp, te::gm::Geometry* sextent, 
                      te::dt::DateTime*  textent, int np, const std::vector<int>& ptypes, 
                      const std::vector<std::string>& pnames, int tp) :
  m_sextent(sextent),
  m_textent(textent),
  m_observations(),
  m_interpolator(interp),
  m_rTree(),
  m_np(np),
  m_ptypes(),
  m_pnames(),
  m_tp(tp)
{
  //create the internal empty RTree 
  m_rTree.reset(new te::sam::rtree::Index<std::size_t>());
}

te::st::PointCoverageIterator te::st::PointCoverage::begin() const
{
  return PointCoverageIterator(m_observations.begin());
}

te::st::PointCoverageIterator te::st::PointCoverage::end() const
{
  return PointCoverageIterator(m_observations.end());
}

void te::st::PointCoverage::add(const te::gm::Point& l, te::dt::AbstractData* value)
{
  boost::ptr_vector<te::dt::AbstractData> aux;
  aux.push_back(value);
  
  PointCoverageItem* item = new PointCoverageItem(l, aux);
  PointCoverageObservation obs(item);
  add(obs);
}

void te::st::PointCoverage::add(const te::gm::Point& l, boost::ptr_vector<te::dt::AbstractData>& value)
{
  PointCoverageItem* item = new PointCoverageItem(l, value);
  PointCoverageObservation obs(item);
  add(obs);
}

void te::st::PointCoverage::add(PointCoverageObservation& obs)
{
  m_observations.push_back(obs);
  std::size_t p = m_observations.size()-1; 
  
  //add to RTree
  const te::gm::Envelope* env = obs->first.getMBR();
  m_rTree->insert(*env, p);
}

te::st::Coverage* te::st::PointCoverage::clone() const
{
  PointCoverage* result = new PointCoverage(m_interpolator, 
                      static_cast<te::gm::Geometry*>(m_sextent->clone()), 
                      static_cast<te::dt::DateTime*>(m_textent->clone()), m_np,
                      m_ptypes, m_pnames);
  
  PointCoverageObservationSet::const_iterator it = m_observations.begin();
  while(it!=m_observations.end())
  {
    PointCoverageItem* item = it->get();
    std::auto_ptr< boost::ptr_vector<te::dt::AbstractData> > data(item->second.clone()); 
    result->add(item->first, *data.release()); 
    ++it;
  }
  return result;
}

te::st::CoverageType te::st::PointCoverage::getType() const
{
  return POINT_COVERAGE;
}
        
te::gm::Geometry* te::st::PointCoverage::getSpatialExtent() const
{
  return m_sextent.get();
}

te::dt::DateTime* te::st::PointCoverage::getTime() const
{
  return m_textent.get();
}

unsigned int te::st::PointCoverage::getNumberOfProperties() const
{
  return m_np;
}

const std::vector<int>& te::st::PointCoverage::getPropertyTypes() const
{
  return m_ptypes;
}

const std::vector<std::string>& te::st::PointCoverage::getPropertyNames() const
{
  return m_pnames;
}

void te::st::PointCoverage::getValue(const te::gm::Point& l, boost::ptr_vector<te::dt::AbstractData>& result) const
{
  for(unsigned int i = 0; i<m_np; ++i)
    result.push_back(m_interpolator->estimate(*this,i,l));
  return;
}

std::auto_ptr<te::dt::AbstractData> te::st::PointCoverage::getValue(const te::gm::Point& l, unsigned int p) const
{
  return std::auto_ptr<te::dt::AbstractData>(m_interpolator->estimate(*this,p,l)); 
}

void te::st::PointCoverage::getValue( const te::gm::Polygon& /*l*/, unsigned int /*p*/, 
                                      boost::ptr_vector<te::dt::AbstractData>& /*result*/) const
{
  //TEM QUE SETAR UMA RESOLUÇÃO/GRADE PARA PODER IDENTIFICAR UM CONJUNTO DE PONTOS DENTRO
  //DESSE POLIGONO
  return;
}

void te::st::PointCoverage::getValue( const te::gm::Polygon& /*l*/, 
                                      boost::ptr_vector<te::dt::AbstractData>& /*result*/) const
{
  //TEM QUE SETAR UMA RESOLUÇÃO/GRADE PARA PODER IDENTIFICAR UM CONJUNTO DE PONTOS DENTRO
  //DESSE POLIGONO
  return;
}

void te::st::PointCoverage::getInt(const te::gm::Point& l, std::vector<int>& result) const
{
  for(unsigned int i = 0; i<m_np; ++i)
  {
     std::auto_ptr<te::dt::AbstractData> aux = getValue(l,i);
     result.push_back(atoi(aux->toString().c_str()));
  }
}

int te::st::PointCoverage::getInt(const te::gm::Point& l, unsigned int p) const
{
  std::auto_ptr<te::dt::AbstractData> result = getValue(l,p);
  return atoi(result->toString().c_str());
}

void te::st::PointCoverage::getInt(const te::gm::Polygon& /*l*/, unsigned int /*p*/, std::vector<int>& /*result*/) const
{
  //TEM QUE SETAR UMA RESOLUÇÃO/GRADE PARA PODER IDENTIFICAR UM CONJUNTO DE PONTOS DENTRO
  //DESSE POLIGONO
  return;
}

void te::st::PointCoverage::getInt(const te::gm::Polygon& /*l*/, std::vector<int>& /*result*/) const
{
  //TEM QUE SETAR UMA RESOLUÇÃO/GRADE PARA PODER IDENTIFICAR UM CONJUNTO DE PONTOS DENTRO
  //DESSE POLIGONO
  return;
}
        
void te::st::PointCoverage::getDouble(const te::gm::Point& l, std::vector<double>& result) const
{
  for(unsigned int i = 0; i<m_np; ++i)
  {
     std::auto_ptr<te::dt::AbstractData> aux = getValue(l,i);
     result.push_back(atof(aux->toString().c_str()));
  }
}

double te::st::PointCoverage::getDouble(const te::gm::Point& l, unsigned int p) const
{
  std::auto_ptr<te::dt::AbstractData> result = getValue(l,p);
  return atof(result->toString().c_str());
}

void te::st::PointCoverage::getDouble(const te::gm::Polygon& /*l*/, unsigned int /*p*/, std::vector<double>& /*result*/) const
{
  //TEM QUE SETAR UMA RESOLUÇÃO/GRADE PARA PODER IDENTIFICAR UM CONJUNTO DE PONTOS DENTRO
  //DESSE POLIGONO
  return;
}

void te::st::PointCoverage::getDouble(const te::gm::Polygon& /*l*/, std::vector<double>& /*result*/) const
{
  //TEM QUE SETAR UMA RESOLUÇÃO/GRADE PARA PODER IDENTIFICAR UM CONJUNTO DE PONTOS DENTRO
  //DESSE POLIGONO
  return;
}

std::auto_ptr<te::rst::Raster> te::st::PointCoverage::getRaster() const
{
  //falta indicar a resolução do raster que ser?gerado, etc, etc...
  return std::auto_ptr<te::rst::Raster>();
}

std::auto_ptr<te::rst::Raster> te::st::PointCoverage::getRaster(unsigned int /*p*/) const
{
  //falta indicar a resolução do raster que ser?gerado, etc, etc...
  return std::auto_ptr<te::rst::Raster>();
}

te::st::PointCoverage::~PointCoverage()
{
}
 




