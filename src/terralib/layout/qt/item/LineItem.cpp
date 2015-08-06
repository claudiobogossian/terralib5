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

te::layout::LineItem::LineItem( ItemController* controller, Observable* o, bool invertedMatrix ) :
  ObjectItem(controller, o, invertedMatrix)
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

  setRect(m_poly.boundingRect());
}

void te::layout::LineItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  LineModel* model = dynamic_cast<LineModel*>(m_model);
  if(!model)
  {
    return;
  }

  if(m_poly.empty())
    return;

  QPainterPath path (m_poly[0]);
  for(int i = 0; i < m_poly.size() ; ++i)
  {
    path.lineTo(m_poly[i]);
  }

  const te::color::RGBAColor& color = model->getColor();
  QColor qColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

  painter->save();

  searchStyle();

  QPen pn(qColor, 0, m_penStyle.style());
  painter->setPen(pn);

  //draws the item
  painter->drawPath(path);

  painter->restore();
}

void te::layout::LineItem::searchStyle()
{
  LineModel* model = dynamic_cast<LineModel*>(m_model);
  if(!model)
  {
    return;
  }

  EnumLineStyleType* enumScale = model->getEnumLineStyleType();

  if(!enumScale)
  {
    return;
  }

  if(model->getCurrentLineStyleType() == enumScale->getStyleSolid())
  {
    m_penStyle.setStyle(Qt::SolidLine);
  }
  if(model->getCurrentLineStyleType() == enumScale->getStyleDash())
  {
    m_penStyle.setStyle(Qt::DashLine);
  }
  if(model->getCurrentLineStyleType() == enumScale->getStyleDot())
  {
    m_penStyle.setStyle(Qt::DotLine);
  }
  if(model->getCurrentLineStyleType() == enumScale->getStyleDashDot())
  {
    m_penStyle.setStyle(Qt::DashDotLine);
  }
  if(model->getCurrentLineStyleType() == enumScale->getStyleDashDotDot())
  {
    m_penStyle.setStyle(Qt::DashDotDotLine);
  }
  if(model->getCurrentLineStyleType() == enumScale->getStyleCustomDash())
  {
    m_penStyle.setStyle(Qt::CustomDashLine);
  }
}

