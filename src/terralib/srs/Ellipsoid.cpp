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
 \file terralib/srs/Ellipsoid.cpp
 */

// TerraLib
#include "../common/Translator.h"
#include "Ellipsoid.h"

// STL
#include <sstream> 

te::srs::Ellipsoid::Ellipsoid(const std::string& name, double rad, double invflat):
  m_name(name),
  m_radium(rad),
  m_invFlattening(invflat)
{}
  
const std::string& te::srs::Ellipsoid::getName() const 
{ 
  return m_name; 
}

void te::srs::Ellipsoid::setName(const std::string& name) 
{ 
  m_name = name; 
}

double te::srs::Ellipsoid::getRadium() const 
{ 
  return m_radium; 
}

void te::srs::Ellipsoid::setRadium(const double& rad) 
{ 
  m_radium = rad; 
}

double te::srs::Ellipsoid::getInverseFlattening() const 
{ 
  return m_invFlattening; 
}

void te::srs::Ellipsoid::setInverseFlattening(const double& invflat) 
{ 
  m_invFlattening = invflat; 
}

std::string 
te::srs::Ellipsoid::getWKT() const
{
  std::ostringstream sstr;
  sstr.precision(10);

  std::string wkt = "SPHEROID[\"";
	wkt += m_name;
	wkt += "\",";
  sstr << m_radium;
	wkt += sstr.str();	
	wkt += ",";
  sstr.str("");
  sstr << m_invFlattening;
	wkt += sstr.str();
	wkt += "]";
	return wkt;
}

