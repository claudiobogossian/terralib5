/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/GTParameters.cpp

  \brief 2D Geometric transformation parameters.
*/

// TerraLib
#include "GTParameters.h"

te::gm::GTParameters::GTParameters()
{
}

te::gm::GTParameters::GTParameters( const GTParameters& other )
{ 
  this->operator=( other );
}

te::gm::GTParameters::~GTParameters()
{
}

te::common::AbstractParameters* te::gm::GTParameters::clone() const
{
  return new GTParameters( *this );
}

void te::gm::GTParameters::reset() throw( te::common::Exception )
{
  m_tiePoints.clear();
  m_directParameters.clear();
  m_inverseParameters.clear();
  m_modelParameters.reset();
}

const te::gm::GTParameters& te::gm::GTParameters::operator=( 
  const te::gm::GTParameters& params )
{
  m_tiePoints = params.m_tiePoints;
  m_directParameters = params.m_directParameters;
  m_inverseParameters = params.m_inverseParameters;
  
  if( params.m_modelParameters.get() )
  {
    m_modelParameters.reset( (GTModelParameters*)
      params.m_modelParameters->clone() );
  }
  else
  {
    m_modelParameters.reset();
  }
  
  return *this;
}
