/*  Copyright (C) 2013-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file AbstractView.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractView.h"

te::layout::AbstractView::AbstractView():
  m_visibleRulers(true)
{
  m_zoomFactors[0.42] = "42%";
  m_zoomFactors[0.5] = "50%";
  m_zoomFactors[0.7] = "70%";
  m_zoomFactors[1.] = "100%";
  m_zoomFactors[1.5] = "150%";
  m_zoomFactors[2.] = "200%";
  m_zoomFactors[3.] = "300%";
}

bool te::layout::AbstractView::getVisibleRulers()
{
  return m_visibleRulers;
}

void te::layout::AbstractView::setVisibleRulers( bool visible )
{
  m_visibleRulers = visible;
}

void te::layout::AbstractView::addZoomFactor( double factor, std::string text )
{
  m_zoomFactors[factor] = text;
}

void te::layout::AbstractView::clearZoomFactors()
{
  m_zoomFactors.clear();
}

double te::layout::AbstractView::nextFactor( double currentFactor )
{
  double zoomFactor = 0;
  std::map<double, std::string>::iterator it;
  
  it = m_zoomFactors.find(currentFactor);

  if(it != m_zoomFactors.end())
  {
    ++it;
    if(it != m_zoomFactors.end())
    {
      zoomFactor = it->first;
      return zoomFactor;
    }
  }

  return zoomFactor;
}

double te::layout::AbstractView::previousFactor( double currentFactor )
{
  double zoomFactor = 0;
  std::map<double, std::string>::iterator it;

  it = m_zoomFactors.find(currentFactor);

  if(it != m_zoomFactors.end())
  {
    --it;
    if(it != m_zoomFactors.end())
    {
      zoomFactor = it->first;
      return zoomFactor;
    }
  }

  return zoomFactor;
}
