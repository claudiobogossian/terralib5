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
  \file WaitView.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "WaitView.h"
#include "View.h"

te::layout::WaitView::WaitView()
{

}

te::layout::WaitView::WaitView(View *view) : 
 m_view(view)
{

}

te::layout::WaitView::~WaitView()
{  
  
}

void te::layout::WaitView::addCoord( QPointF point )
{
  if(!m_view)
    return;
  
  QPointF poinT = m_view->mapToScene(point.toPoint());
  te::gm::Point* p = new te::gm::Point(poinT.x(), poinT.y());
  m_coords.push_back(p);
  QPoint received(point.x(), point.y());
  m_points.push_back(received);

  m_view->viewport()->update();
}

void te::layout::WaitView::clear()
{
  m_coords.clear();
  m_points.clear();
}

QVector<QPoint> te::layout::WaitView::getCoords()
{
  return m_points;
}

std::vector<te::gm::Point*> te::layout::WaitView::getCoordsW()
{
  return m_coords;
}
