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
#include "TextController1.h"
#include "../../item/TextModel.h"

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

te::layout::TextItem::TextItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix ) 
  : AbstractItem<QGraphicsTextItem>(controller, model, true)
  , m_isInEdition(false)
{  
  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(false);

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

        TextController1* controller = dynamic_cast<TextController1*>(m_controller);
        if(controller != 0)
        {
          controller->sceneChanged();
        }
      }
    }
  }
  else if(change == QGraphicsItem::ItemSelectedHasChanged)
  {
    if(m_isInEdition && isSelected() == false)
    {
      leaveEditionMode();
    }
  }
  /*else if(change == QGraphicsItem::ItemPositionChange && !m_move)
  {
    // TODO: we need to fix the positioning of the item
    QPointF newPos = value.toPointF();

    double tx = transform().dx();
    double ty = transform().dy();

    newPos.setX(newPos.x() - tx);
    newPos.setY(newPos.y() - ty);
    return newPos;
  }*/

  return AbstractItem<QGraphicsTextItem>::itemChange(change, value);
}

void te::layout::TextItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
  if(event->button() == Qt::LeftButton)
  {
    if(m_isInEdition == false)
    {
      enterEditionMode();
    }
    else
    {
      setCursor(Qt::ArrowCursor);
    }
  }
}

QRectF te::layout::TextItem::boundingRect() const
{
  //if(m_isInEdition == true)
  if(true)
  {
    //when we are editing the item, we let the item handle the changes in the bounding box
    QRectF rect = QGraphicsTextItem::boundingRect();
    return QGraphicsTextItem::boundingRect();
  }
  else
  {
    return AbstractItem<QGraphicsTextItem>::boundingRect();
  }
}


void te::layout::TextItem::enterEditionMode()
{
  if(m_isInEdition == true)
  {
    return;
  }

  //If enabled is true, this item will accept hover events
  setTextInteractionFlags(Qt::TextEditorInteraction);
  setCursor(Qt::IBeamCursor);
  QTextCursor cursor(textCursor());
  cursor.clearSelection();
  setTextCursor(cursor);
  setFocus();

  m_isInEdition = true;
}

void te::layout::TextItem::leaveEditionMode()
{
  if(m_isInEdition == false)
  {
    return;
  }

  //Necessary clear the selection and focus of the edit 
  //after being completely closed and like this not cause bad behavior.
  QTextCursor cursor(textCursor());
  cursor.clearSelection();
  setTextCursor(cursor);
  setTextInteractionFlags(Qt::NoTextInteraction);
  unsetCursor();
  clearFocus();

  TextController1* controller = dynamic_cast<TextController1*>(m_controller);
  if(controller != 0)
  {
    controller->textChanged();
  }
  m_isInEdition = false;
}

void te::layout::TextItem::updateGeometry( int position, int charsRemoved, int charsAdded )
{
  setTextWidth(-1);
  setTextWidth(boundingRect().width());
  
  TextController1* controller = dynamic_cast<TextController1*>(m_controller);
  if(controller != 0)
  {
    controller->textChanged();
  }
}

