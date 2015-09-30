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
#include "CreateLineItemTool.h"
#include "../View.h"
#include "../Scene.h"
#include "../BuildGraphicsItem.h"
#include "../../item/LineItem.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../core/enum/Enums.h"
#include "../../../../geometry/Point.h"


// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

te::layout::CreateLineItemTool::CreateLineItemTool(View* view, EnumType* itemType, QObject* parent)
  : AbstractLayoutTool(view, parent),
  m_controller(NULL),
  m_itemType(itemType)
{
  setCursor(Qt::ArrowCursor);

  if (!m_itemType)
  {
    m_itemType = Enums::getInstance().getEnumObjectType()->getLineItem();
  }
}

te::layout::CreateLineItemTool::~CreateLineItemTool()
{
}

bool te::layout::CreateLineItemTool::mousePressEvent(QMouseEvent* e)
{
  if (!m_view)
    return false;

  //Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if (e->button() != Qt::LeftButton)
    return false;

  Scene* scne = dynamic_cast<Scene*>(m_view->scene());

  if (!scne)
    return false;

  createItem();

  QPointF scenePos = m_view->mapToScene(e->pos());
  //QPointF itemPos = m_item->mapFromScene(scenePos);
  te::gm::Point p(scenePos.x(), scenePos.y());
  m_coords.push_back(p);
  setGeometry();
  m_view->viewport()->update();
  return true;
}

bool te::layout::CreateLineItemTool::mouseMoveEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::CreateLineItemTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::CreateLineItemTool::keyPressEvent( QKeyEvent* keyEvent )
{
  if(keyEvent->key() == Qt::Key_Escape)
  {
    m_coords.clear();
    m_view->resetDefaultConfig(true);
  }
  return true;
}

// o melhor seria a tool ter um método de desenho de linha para depois atualizar a geometria final no modelo
void te::layout::CreateLineItemTool::setGeometry()
{
  // cria lineString apenas para achar o lowerleft
  te::gm::LineString* lineString = new te::gm::LineString(m_coords.size(), te::gm::LineStringType);
  for (unsigned int i = 0; i < m_coords.size(); i++)
  {
    lineString->setPointN(i, m_coords[i]);
  }
  te::gm::GeometryShrPtr line(lineString);
  te::gm::Coord2D lowerLeft = line->getMBR()->getLowerLeft();
  m_item->setPos(lowerLeft.getX(), lowerLeft.getY());
  
  // atualiza posição dos pontos de acordo com a nova posição do item
  for (unsigned int i = 0; i < m_coords.size(); i++)
  {
    QPointF point(m_coords[i].getX(), m_coords[i].getY());
    QPointF point2Item = m_item->mapFromScene(point);
    lineString->setPointN(i, te::gm::Point(point2Item.x(), point2Item.y()));
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Property property(0);
  property.setName("geometry");
  property.setLabel("geometry");
  property.setVisible(false);
  property.setValue(line, dataType->getDataTypeGeometry());
  m_controller->setProperty(property);
}

void te::layout::CreateLineItemTool::createItem()
{
  Scene* sc = dynamic_cast<Scene*>(m_view->scene());
  if (!sc)
  {
    return;
  }

  if (m_coords.empty())
  {
    // create a new item
    BuildGraphicsItem buildItem(sc);
    m_item = buildItem.createItem(m_itemType);

    AbstractItem<QGraphicsItem> * itemView = dynamic_cast<AbstractItem<QGraphicsItem> *> (m_item);
    m_controller = itemView->getController();
  }
}

