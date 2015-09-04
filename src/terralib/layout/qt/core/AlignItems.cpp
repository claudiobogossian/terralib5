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
  \file AlignItems.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AlignItems.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/geometry/Coord2D.h"

// STL
#include <sstream>

// Qt
#include <QGraphicsScene>
#include <QGraphicsItem>

te::layout::AlignItems::AlignItems( QGraphicsScene* scene, PaperConfig* config ):
  m_scene(scene),
  m_config(config),
  m_minimunZValue(0)
{
 
}

te::layout::AlignItems::~AlignItems()
{

}

void te::layout::AlignItems::bringToFront()
{
  if (m_scene->selectedItems().isEmpty())
    return;

  QGraphicsItem* selectedItem = m_scene->selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal maxZValue = selectedItem->zValue();
  QGraphicsItem* itemMaxZValue = selectedItem;

  qreal zValue = selectedItem->zValue();
  foreach (QGraphicsItem *item, overlapItems) 
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if(it)
      {        
        if((item->zValue() >= zValue))
        {
          maxZValue = item->zValue();
          itemMaxZValue = item;     
        }
      }
    }
  }
  selectedItem->setZValue(maxZValue);
  if(itemMaxZValue)
  {
    itemMaxZValue->setZValue(zValue);
  }
}

void te::layout::AlignItems::sendToBack()
{
  if (m_scene->selectedItems().isEmpty())
    return;

  QGraphicsItem *selectedItem = m_scene->selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal minimumZValue = selectedItem->zValue();
  QGraphicsItem* itemMinimumZValue = selectedItem;

  qreal zValue = selectedItem->zValue();
  foreach (QGraphicsItem *item, overlapItems) 
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if(it)
      {
        if (item->zValue() <= zValue)
        {
          if (item->zValue() > m_minimunZValue) 
          {
            minimumZValue = item->zValue();
            itemMinimumZValue = item;
          }
        }
      }
    }
  }
  selectedItem->setZValue(minimumZValue);
  if(itemMinimumZValue)
  {
    itemMinimumZValue->setZValue(zValue);
  }
}

void te::layout::AlignItems::alignLeft()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;
  
  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  double dbLeft = 0;

  double ww = 0;
  double hh = 0;
  
  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbLeft = ppbx.getLowerLeftX();
    QPointF pot(dbLeft, items.first()->sceneBoundingRect().y());   
    items.first()->setPos(pot);
    return;
  }

  dbLeft = sourceRect.left();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      QPointF pt(dbLeft, item->scenePos().y());
      item->setPos(pt);
    }
  }
}

void te::layout::AlignItems::alignRight()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 

  double dbRight = 0;
  double w = 0;

  double ww = 0;
  double hh = 0;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbRight = ppbx.getUpperRightX();
    w = dbRight - items.first()->sceneBoundingRect().width();
    QPointF pot(w, items.first()->sceneBoundingRect().y());
    items.first()->setPos(pot);
    return;
  }

  dbRight = sourceRect.right();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      w = dbRight - item->sceneBoundingRect().width();
      QPointF pt(w, item->scenePos().y());
      item->setPos(pt);
    }
  }
}

void te::layout::AlignItems::alignTop()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  double dbBottom = 0;
  double h = 0;

  double ww = 0;
  double hh = 0;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbBottom = ppbx.getUpperRightY();
    h = dbBottom - items.first()->sceneBoundingRect().height();
    QPointF pot(items.first()->sceneBoundingRect().x(), h);
    items.first()->setPos(pot);
    return;
  }

  dbBottom = sourceRect.bottom();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      h = dbBottom - item->sceneBoundingRect().height();
      QPointF pt(item->scenePos().x(), h);
      item->setPos(pt);
    }
  }
}

void te::layout::AlignItems::alignBottom()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  double dbTop = 0;

  double ww = 0;
  double hh = 0;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbTop = ppbx.getLowerLeftY();
    QPointF pot(items.first()->sceneBoundingRect().x(), dbTop);
    items.first()->setPos(pot);
    return;
  }

  dbTop = sourceRect.top();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      QPointF pt(item->scenePos().x(), dbTop);
      item->setPos(pt);
    }
  }
}

void te::layout::AlignItems::alignCenterHorizontal()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  double dbCenterHrz = 0;
  double w = 0;

  double ww = 0;
  double hh = 0;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbCenterHrz = ppbx.getCenter().x;
    w = items.first()->sceneBoundingRect().width() / 2.;
    QPointF pot(dbCenterHrz - w, items.first()->sceneBoundingRect().y());
    items.first()->setPos(pot);
    return;
  }

  dbCenterHrz = sourceRect.center().x();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      w = item->sceneBoundingRect().width() / 2.;

      QPointF pt(dbCenterHrz - w, item->scenePos().y());
      item->setPos(pt);
    }
  }
}

void te::layout::AlignItems::alignCenterVertical()
{
  QRectF sourceRect = getSelectionItemsBoundingBox();

  if(!sourceRect.isValid())
    return;

  QList<QGraphicsItem *> items = m_scene->selectedItems(); 
  double dbCenterVrt = 0;
  double h = 0;

  double ww = 0;
  double hh = 0;

  m_config->getPaperSize(ww, hh);

  if(items.count() == 1)
  {
    te::gm::Envelope ppbx(0, 0, ww, hh);
    dbCenterVrt = ppbx.getCenter().y;
    h = items.first()->sceneBoundingRect().height() / 2.;
    QPointF pot(items.first()->sceneBoundingRect().x(), dbCenterVrt - h);
    items.first()->setPos(pot);
    return;
  }

  dbCenterVrt = sourceRect.center().y();

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      h = item->sceneBoundingRect().height() / 2.;

      QPointF pt(item->scenePos().x(), dbCenterVrt - h);
      item->setPos(pt);
    }
  }
}

QRectF te::layout::AlignItems::getSelectionItemsBoundingBox()
{
  QRectF sourceRect; 
  QList<QGraphicsItem *> items = m_scene->selectedItems(); 

  foreach(QGraphicsItem *item, items) 
  { 
    sourceRect = sourceRect.united(item->sceneBoundingRect()); 
  }

  return sourceRect;
}

int te::layout::AlignItems::getMinimumZValue()
{
  return m_minimunZValue;
}

void te::layout::AlignItems::setMinimumZValue(int minimum)
{
  m_minimunZValue = minimum;
}

