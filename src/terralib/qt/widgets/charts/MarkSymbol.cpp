/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file MarkSymbol.cpp

  \brief This file contains a class that specialize the QwtSymbol to draw a Terralib Mark. 
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../se/Mark.h"
#include "../../../maptools/MarkRendererManager.h"
#include "MarkSymbol.h"
#include "Utils.h"

// Qt Widgets Utils
#include "../Utils.h"

// Qt
#include <QImage>
#include <QPainter>


te::qt::widgets::MarkSymbol::MarkSymbol(te::se::Mark* mark, std::size_t size) :
  QwtSymbol(UserStyle),
  m_mark(mark),
  m_size(size)
{
  
}

void te::qt::widgets::MarkSymbol::drawSymbols(QPainter* painter, const QPointF* point, int numPoints) const
{
  te::color::RGBAColor** rgba = te::map::MarkRendererManager::getInstance().render(m_mark, m_size);
  
  QImage* image = GetImage(rgba, m_size, m_size);

  for(int i=0; i<numPoints; ++i)
  {
    QPointF p = point[i];
    painter->drawImage(p, *image);
  }
  delete image;
  te::common::Free(rgba,m_size);
}


