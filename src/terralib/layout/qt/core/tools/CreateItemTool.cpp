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
#include "CreateItemTool.h"
#include "../View.h"
#include "../Scene.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"

// Qt
#include <QtGui/QMouseEvent>

te::layout::CreateItemTool::CreateItemTool(View* view, EnumType* itemType, QObject* parent)
  : RubberBandTool(view, parent),
  m_itemType(itemType)
{
  setCursor(Qt::ArrowCursor);
}

te::layout::CreateItemTool::~CreateItemTool()
{
}

bool te::layout::CreateItemTool::mousePressEvent(QMouseEvent* e)
{
  if (!m_itemType)
    return false;

  if (e->button() != Qt::LeftButton)
    return false;

  m_started = true;
  m_rect = QRectF();

  return RubberBandTool::mousePressEvent(e);
}

bool te::layout::CreateItemTool::mouseMoveEvent(QMouseEvent* e)
{
  if (!m_itemType)
    return false;

  if (!m_started)
    return false;

  return RubberBandTool::mouseMoveEvent(e);
}

bool te::layout::CreateItemTool::mouseReleaseEvent(QMouseEvent* e)
{
  if (!m_itemType)
    return false;

  m_started = false;

  if (!m_view)
    return false;

  //Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if (e->button() != Qt::LeftButton)
    return false;

  Scene* sc = dynamic_cast<Scene*>(m_view->scene());
  if (!sc)
    return false;

  View* view = dynamic_cast<View*>(m_view);
  if (!view)
    return false;

  if (m_rubberBand == 0)
  {
    return false;
  }

  QRect rect = m_rubberBand->geometry().normalized();

  RubberBandTool::mouseReleaseEvent(e);

  //Conversion to world coordinates
  QPolygonF poly = m_view->mapToScene(rect);

  QRectF bounding = poly.boundingRect();
  
  // initial point
  te::gm::Coord2D coord(bounding.x(), bounding.y());

  double width = bounding.width();
  double height = bounding.height();

  if (width == 0. && height == 0.)
  {
    QPointF scenePos = m_view->mapToScene(m_origin);
    coord.x = scenePos.x();
    coord.y = scenePos.y();
  }
  
  // create a new item
  QGraphicsItem* item = sc->createItem(m_itemType, coord, width, height);
  if (!item)
    return false;

  //sc->update();

  return true;
}




