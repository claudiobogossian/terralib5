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
  \file PolygonItemItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PolygonItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/PolygonModel.h"

// Qt
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QObject>

te::layout::PolygonItem::PolygonItem( ItemController* controller, Observable* o ) :
  LineItem(controller, o)
{ 
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::PolygonItem::~PolygonItem()
{

}

void te::layout::PolygonItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);

  drawPolygon(painter);

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::PolygonItem::drawPolygon( QPainter * painter )
{
  LineModel* model = dynamic_cast<LineModel*>(m_model);
  if(!model)
  {
    return;
  }

  if(m_poly.empty())
    return;

  te::color::RGBAColor clrLne = model->getLineColor();

  QColor cpen;
  cpen.setRed(clrLne.getRed());
  cpen.setGreen(clrLne.getGreen());
  cpen.setBlue(clrLne.getBlue());
  cpen.setAlpha(clrLne.getAlpha());

  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->save();
  painter->drawPolygon(m_poly);
  painter->restore();
}

