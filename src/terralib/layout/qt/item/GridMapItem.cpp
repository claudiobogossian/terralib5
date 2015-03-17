/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file GridMapItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridMapItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"

//Qt
#include <QStyleOptionGraphicsItem>
#include "../../item/GridMapModel.h"

te::layout::GridMapItem::GridMapItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);
  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::GridMapItem::~GridMapItem()
{

}

void te::layout::GridMapItem::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  te::color::RGBAColor** rgba = context.getPixmap();  

  if(!rgba)
    return;

  Utils* utils = context.getUtils();

  if(!utils)
    return;

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  if(!box.isValid())
    return;

  QPixmap pixmp;
  QImage* img = 0;
  
  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmp = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;
  
  setPixmap(pixmp);
  update();
}

void te::layout::GridMapItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  ObjectItem::paint(painter, option, widget);

  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);
  
  painter->save();

  QPainterPath gridMapPath;
  gridMapPath.setFillRule(Qt::WindingFill);

  int heightRect = (int)boundingRect().height();
  int widgetRect = (int)boundingRect().width();

  painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));
    
  for (int i = 0; i <= heightRect; i+=10)
  {
    QLineF lineOne = QLineF(boundingRect().topLeft().x(), boundingRect().topLeft().y() + i, boundingRect().topRight().x(), boundingRect().topRight().y() + i);
    painter->drawLine(lineOne);

    for (int j = 0; j <= widgetRect; j+=10)
    {
      QLineF lineTwo = QLineF(boundingRect().topLeft().x() + j, boundingRect().topLeft().y(), boundingRect().bottomLeft().x() + j, boundingRect().bottomLeft().y());
      painter->drawLine(lineTwo);
    }    
  }
    
  painter->restore();

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

QRectF te::layout::GridMapItem::boundingRect()
{
  if(parentItem())
  {
    if(parentItem()->boundingRect().isValid())
    {
      m_rect = parentItem()->boundingRect();
      return m_rect;
    }    
  }
  return m_rect;
}
