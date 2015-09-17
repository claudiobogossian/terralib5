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

// TerraLib
#include "ZoomClickTool.h"

// Qt
#include <QtGui/QMouseEvent>

te::layout::ZoomClickTool::ZoomClickTool(View* view, const QCursor& cursor, const double& zoomFactor, const ZoomType& type, QObject* parent)
  : ZoomTool(view, zoomFactor, type, parent)
{
  setCursor(cursor);
}

te::layout::ZoomClickTool::~ZoomClickTool()
{
}

bool te::layout::ZoomClickTool::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  QPointF pt(e->pos());
  applyZoom(pt);

  return true;
}

