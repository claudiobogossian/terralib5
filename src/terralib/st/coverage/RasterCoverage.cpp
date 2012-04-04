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
  \file RasterCoverage.cpp

  \brief This file contains a class to represent a raster coverage.
*/

// TerraLib
#include "../../raster/Raster.h"
#include "../../raster/Grid.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Coord2D.h"
#include "../../geometry/Point.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/SimpleProperty.h"

//ST
//#include "CoverageIterator.h"
#include "RasterCoverage.h"
#include "../observation/ObservationSet.h"
#include "../observation/ObservationSetIterator.h"
#include "../observation/Observation.h"
#include "../timeseries/TimeSeries.h"
#include "../interpolator/AbstractInterpolator.h"


te::st::RasterCoverage::RasterCoverage() 
  : m_observations(0),
    m_interpolator(0),
    m_boundary(0)
{
  m_observations = new te::st::ObservationSet();
} 

te::st::RasterCoverage::RasterCoverage(ObservationSet* obs, te::gm::Envelope* bound) 
  : m_observations(obs),
    m_interpolator(0),
    m_boundary(bound)
{
}

te::st::RasterCoverage::RasterCoverage(ObservationSet* obs, te::gm::Envelope* bound, AbstractInterpolator* interp)
  : m_observations(obs),
    m_interpolator(interp),
    m_boundary(bound)
{
}              
 
te::st::ObservationSet* te::st::RasterCoverage::getObservationSet() const
{
  return m_observations;
}

te::st::AbstractInterpolator* te::st::RasterCoverage::getInterpolator() const
{
  return m_interpolator;
}

te::gm::Geometry* te::st::RasterCoverage::getBoundary() const
{
  return 0;
}        
        
void te::st::RasterCoverage::snapshot(te::dt::DateTime* t, te::rst::Raster* output) const
{
}

void te::st::RasterCoverage::timeSeries(const te::gm::Point& p, std::vector<TimeSeries*>& result) const
{  
  if(m_observations->size()<1)
    return;
  
  //Get the first observed raster
  te::rst::Raster* r = dynamic_cast<te::rst::Raster*>(m_observations->begin().getObservedValue(0));
    
  //Get one time series for each band
  for(unsigned int b=0; b<r->getNumberOfBands(); b++)
  {
    //Get the data type of the b-th band
    te::dt::Property* prop = new te::dt::SimpleProperty("time series", r->getBandDataType(b)); 
    te::st::TimeSeries* ts = new te::st::TimeSeries(prop);
    result.push_back(ts);
  }

  TimeToObservationMap::const_iterator itb, ite;
  itb = m_observations->getObservations().begin();
  ite = m_observations->getObservations().end();
  
  while(itb!=ite)
  {
    te::rst::Raster* raster = dynamic_cast<te::rst::Raster*>(itb->second->getObservedValue(0));
    te::dt::DateTime* datetime = dynamic_cast<te::dt::DateTime*>(itb->first->clone());
        
    //get the values
    std::vector<double> values;
    te::gm::Coord2D gridCoord = raster->getGrid()->geoToGrid(p.getX(), p.getY());
    raster->getValues((unsigned int)gridCoord.x, (unsigned int)gridCoord.y, values);

    for(unsigned int b=0; b<values.size(); b++) 
    {
      te::dt::Double* v = new te::dt::Double(values[b]); 
      result[b]->addObservation(datetime, v);
    }
    ++itb;
  }
}
 
void te::st::RasterCoverage::addObservation(te::dt::DateTime* phTime, te::rst::Raster* raster)
{
  Observation* ob = new Observation(phTime, raster);
  m_observations->addObservation(ob);
}       

te::rst::Raster* te::st::RasterCoverage::getRaster(te::dt::DateTime* t)
{
  return 0;
}

te::st::RasterCoverage::~RasterCoverage()
{
  delete m_observations;
  delete m_interpolator;
  delete m_boundary;
} 




