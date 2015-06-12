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
  \file WKTActions.cpp

  \brief A class that implements the semanthic actions to grammar rules for well known text (WKT) format for Spatial Reference Systems.
         Basically, the class is responsible to generate a SRS element.
*/

// TerraLib
#include "Datum.h"
#include "Ellipsoid.h"
#include "GeographicCoordinateSystem.h"
#include "ProjectedCoordinateSystem.h"
#include "WKTActions.h"

// STL
#include <cstdlib>

te::srs::WKTActions::WKTActions()
  : m_srs(0),
    m_projCS(0),
    m_geoCS(0),
    m_datum(0),
    m_ellps(0),
    m_authorityName(""),
    m_authorityCode("")
{
  m_sevenParams.resize(7);
}

te::srs::WKTActions::~WKTActions()
{
}

void te::srs::WKTActions::reset()
{
  m_srs = 0;

  delete m_projCS;
  m_projCS = 0;

  delete m_geoCS;
  m_geoCS = 0;
  
  delete m_datum;    
  m_datum = 0;

  delete m_ellps;
  m_ellps = 0;
}

void te::srs::WKTActions::createProjectedCoordinateSystem()
{
  m_projCS = new te::srs::ProjectedCoordinateSystem();
  m_srs = m_projCS;
}

void te::srs::WKTActions::createGeographicCoordinateSystem()
{
  m_geoCS = new te::srs::GeographicCoordinateSystem();
  m_srs = m_geoCS;
}

void te::srs::WKTActions::createDatum()
{
  m_datum = new te::srs::Datum();
}

void te::srs::WKTActions::createSpheroid()
{
  m_ellps = new te::srs::Ellipsoid();
}

void te::srs::WKTActions::setName(const std::string& name)
{
  m_srs->setName(name);
}

void te::srs::WKTActions::setPrimeMeridian(const double& primem)
{
  m_geoCS->setPrimem(primem);
}

void te::srs::WKTActions::setUnitName(const std::string& name)
{
  m_srs->setUnitName(name);
}

void te::srs::WKTActions::setConversionFactor(const double& /*f*/)
{}

void te::srs::WKTActions::setDatumName(const std::string& name)
{
  m_datum->setName(name);
}

void te::srs::WKTActions::setDx(const double& dx)
{
  m_sevenParams[0] = dx;
}

void te::srs::WKTActions::setDy(const double& dy)
{
  m_sevenParams[1] = dy;
}

void te::srs::WKTActions::setDz(const double& dz)
{
  m_sevenParams[2] = dz;
}

void te::srs::WKTActions::setEx(const double& ex)
{
  m_sevenParams[3] = ex;
}

void te::srs::WKTActions::setEy(const double& ey)
{
  m_sevenParams[4] = ey;
}

void te::srs::WKTActions::setEz(const double& ez)
{
  m_sevenParams[5] = ez;
}

void te::srs::WKTActions::setPPM(const double& ppm)
{
  m_sevenParams[6] = ppm;
}

void te::srs::WKTActions::endToWGS84()
{
  m_datum->setToWGS84Params(m_sevenParams);
}

void te::srs::WKTActions::setAuthorityName(const std::string& name)
{
  m_authorityName = name;
}

void te::srs::WKTActions::setAuthorityCode(const std::string& code)
{
  m_authorityCode = code;
}

void te::srs::WKTActions::setSpheroidName(const std::string& name)
{
  m_ellps->setName(name);
}

void te::srs::WKTActions::setSemiMajorAxis(const double& rad)
{
  m_ellps->setRadium(rad);
}

void te::srs::WKTActions::setInverseFlattening(const double& invflat)
{
  m_ellps->setInverseFlattening(invflat);
}

void te::srs::WKTActions::setProjectionName(const std::string& name)
{
  m_projCS->setProjection(name);
}

void te::srs::WKTActions::setParameter(const std::string& name)
{
  m_parameter.first = name;
}

void te::srs::WKTActions::setParameterValue(const double& value)
{
  m_parameter.second = value;
  m_params.insert(m_parameter);
}

void te::srs::WKTActions::setAxisName(const std::string& name)
{
  m_axis.first = name;
}

void te::srs::WKTActions::setAxisValue(const std::string& value)
{
  m_axis.second = value;
  m_axes.insert(m_axis);
}

void te::srs::WKTActions::endSpheroid()
{
  m_datum->setEllipsoid(m_ellps);
  m_ellps = 0;
}

void te::srs::WKTActions::endDatum()
{
  m_geoCS->setDatum(m_datum);
  m_datum = 0;
}

void te::srs::WKTActions::endGeographicCoordinateSystem()
{
  if(!m_authorityName.empty() && !m_authorityCode.empty()) // The parser read AUTHORITY[name, code]?
  {
    m_geoCS->setSRID(atoi(m_authorityCode.c_str()), m_authorityName);

// clears authority to the next element use it
    m_authorityName.clear();
    m_authorityCode.clear();
  }

  if(m_projCS == 0)
  {
    m_geoCS = 0; // The reference is now only in m_srs
    return;
  }

  m_projCS->setGeographicCoordinateSystem(m_geoCS);
  m_srs = m_projCS;
}

void te::srs::WKTActions::endProjectedCoordinateSystem()
{
  if(!m_authorityName.empty() && !m_authorityCode.empty()) // The parser read AUTHORITY[name, code]?
  {
    m_projCS->setSRID(atoi(m_authorityCode.c_str()), m_authorityName);
    // Clears authority to the next element use it
    m_authorityName.clear();
    m_authorityCode.clear();
  }

  m_projCS->setParameters(m_params);
  m_projCS = 0; // The reference is now only in m_srs
}

te::srs::SpatialReferenceSystem* te::srs::WKTActions::getSRS()
{
  return m_srs;
}

