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

#include "GTParameters.h"

te::gm::GTParameters::GTParameters()
{
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
  m_modelParameters.clear();
}

const te::common::AbstractParameters& te::gm::GTParameters::operator=( const 
  te::common::AbstractParameters& params )
{
  reset();
  
  GTParameters const* paramsPtr = 
    dynamic_cast< GTParameters const* >(&params );
    
  if( paramsPtr )
  {
    m_tiePoints = paramsPtr->m_tiePoints;
    m_modelParameters = paramsPtr->m_modelParameters;
  }
  
  return *this;
}
