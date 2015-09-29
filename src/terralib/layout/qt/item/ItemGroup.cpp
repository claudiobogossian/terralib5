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
  \file ItemGroup.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemGroup.h"
#include "ItemGroupController.h"

// Qt
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

te::layout::ItemGroup::ItemGroup(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsItemGroup>(controller, invertedMatrix)
{

}

te::layout::ItemGroup::~ItemGroup()
{
}

QRectF te::layout::ItemGroup::boundingRect() const
{
  QRectF rect = this->childrenBoundingRect();
  if(rect.isValid() == true)
  {
    return rect;
  }

  return QGraphicsItemGroup::boundingRect();
}

void te::layout::ItemGroup::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  //we dont want the group to draw its selection, so we dont call the paint function
}

QVariant te::layout::ItemGroup::itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemChildAddedChange)
  {
    ItemGroupController* controller = dynamic_cast<ItemGroupController*>(m_controller);
    if(controller != 0)
    {
      controller->itemAdded();
    }

    QGraphicsItem* child = qvariant_cast<QGraphicsItem *>(value);
    if (child != 0)
    {
      child->setFlag(QGraphicsItem::ItemStacksBehindParent);
    }
  }
  else if (change == QGraphicsItem::ItemChildRemovedChange)
  {
    QGraphicsItem* child = qvariant_cast<QGraphicsItem *>(value);
    if (child != 0)
    {
      child->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
    }
  }

  return AbstractItem<QGraphicsItemGroup>::itemChange(change, value);
}

