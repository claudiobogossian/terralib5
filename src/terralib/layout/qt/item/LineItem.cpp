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
  \file LineItemItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LineItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/LineModel.h"

// STL
#include <vector>

// Qt
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QObject>

te::layout::LineItem::LineItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::LineItem::~LineItem()
{

}

void te::layout::LineItem::updateObserver( ContextItem context )
{
  ObjectItem::updateObserver(context);

  LineModel* model = dynamic_cast<LineModel*>(m_model);
  if(!model)
  {
    return;
  }

  if(!m_poly.empty())
    return;

  std::vector<te::gm::Point*> coords = model->getCoords();

  if (coords.empty())
    return;

  for (unsigned int i = 0; i < coords.size(); ++i)
  {
    QPointF pt(coords[i]->getX(), coords[i]->getY());

    QPointF mlocal = mapFromScene(pt);
    m_poly.push_back(mlocal);   
  }
}

void te::layout::LineItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);

  drawLine(painter);

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::LineItem::drawLine( QPainter * painter )
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

  QPainterPath path (m_poly[0]);

  for(int i = 0; i < m_poly.size() ; ++i)
  {
    path.lineTo(m_poly[i]);
  }

  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->save();
  painter->drawPath(path);
  painter->restore();
}


