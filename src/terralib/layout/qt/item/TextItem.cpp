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
  \file TextItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextItem.h"
#include "TextController.h"
#include "../../item/TextModel.h"
#include "../../qt/core/Scene.h"

// STL
#include <string>

// Qt
#include <QTextDocument>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTextOption>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

te::layout::TextItem::TextItem(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsTextItem>(controller, true)
{  
  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(false);
  setCursor(Qt::ArrowCursor); // default cursor

  connect(document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(updateGeometry(int, int, int)));
}

te::layout::TextItem::~TextItem()
{

}

void te::layout::TextItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  //asks the item to paint itself
  QGraphicsTextItem::paint(painter, option, widget);
}

QVariant te::layout::TextItem::itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemSceneHasChanged)
  {
    Scene* myScene = dynamic_cast<Scene*>(this->scene());
    if(myScene != 0)
    {
      QTransform transfView = myScene->sceneTransform();
      QTransform transfItem = this->transform();
      if(transfItem != transfView.inverted())
      {
        QTransform transf = transfView.inverted();
        this->setTransform(transf);

        TextController* controller = dynamic_cast<TextController*>(m_controller);
        if(controller != 0)
        {
          controller->sceneChanged();
        }
      }
    }
  }
  return AbstractItem<QGraphicsTextItem>::itemChange(change, value);
}

void te::layout::TextItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  QGraphicsTextItem::mousePressEvent(event);
  if (m_isEditionMode)
  {
    setCursor(Qt::IBeamCursor);
  }
}

void te::layout::TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
  if(event->button() == Qt::LeftButton)
  {
    if(m_isEditionMode == true)
    {
      enterEditionMode();
    }
  }
}

void te::layout::TextItem::keyPressEvent(QKeyEvent * event)
{
  QGraphicsTextItem::keyPressEvent(event);
}

QRectF te::layout::TextItem::boundingRect() const
{
  //when we are editing the item, we let the item handle the changes in the bounding box
  QRectF rect = QGraphicsTextItem::boundingRect();
  return rect;
}

void te::layout::TextItem::enterEditionMode()
{
  //If enabled is true, this item will accept hover events
  setTextInteractionFlags(Qt::TextEditorInteraction);
  setCursor(Qt::IBeamCursor);
  QTextCursor cursor(textCursor());
  cursor.clearSelection();
  setTextCursor(cursor);
  setFocus();
}

void te::layout::TextItem::leaveEditionMode()
{
  //Necessary clear the selection and focus of the edit 
  //after being completely closed and like this not cause bad behavior.
  QTextCursor cursor(textCursor());
  cursor.clearSelection();
  setTextCursor(cursor);
  setTextInteractionFlags(Qt::NoTextInteraction);
  unsetCursor();
  clearFocus();
  setCursor(Qt::ArrowCursor);

  TextController* controller = dynamic_cast<TextController*>(m_controller);
  if(controller != 0)
  {
    controller->textChanged();
  }
}

void te::layout::TextItem::updateGeometry(int position, int charsRemoved, int charsAdded)
{
  setTextWidth(-1);
  setTextWidth(boundingRect().width());
  
  TextController* controller = dynamic_cast<TextController*>(m_controller);
  if(controller != 0)
  {
    controller->textChanged();
  }
}

