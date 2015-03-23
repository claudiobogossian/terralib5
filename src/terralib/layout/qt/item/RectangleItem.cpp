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
  \file RectangleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "RectangleItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/RectangleModel.h"

te::layout::RectangleItem::RectangleItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::RectangleItem::~RectangleItem()
{

}

void te::layout::RectangleItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  if(m_resizeMode)
  {
    ObjectItem::paint(painter, option, widget);
    return;
  }

  drawBackground(painter);

  drawRectangle(painter);

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::RectangleItem::drawRectangle( QPainter * painter )
{
  RectangleModel* model = dynamic_cast<RectangleModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  QPainterPath rect_path;
  rect_path.addRect(boundingRect());

  QColor cpen(0,0,0);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  te::color::RGBAColor clrBack = model->getBackgroundColor();

  QColor cbrush;
  cbrush.setRed(clrBack.getRed());
  cbrush.setGreen(clrBack.getGreen());
  cbrush.setBlue(clrBack.getBlue());
  cbrush.setAlpha(clrBack.getAlpha());

  painter->setBrush(cbrush);
  painter->drawPath(rect_path);

  painter->restore();
}
