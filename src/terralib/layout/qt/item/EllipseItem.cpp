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
  \file EllipseItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EllipseItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/EllipseModel.h"

te::layout::EllipseItem::EllipseItem( ItemController* controller, Observable* o, bool invertedMatrix ) :
  ObjectItem(controller, o, invertedMatrix)
{
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::EllipseItem::~EllipseItem()
{

}

void te::layout::EllipseItem::drawItem( QPainter * painter )
{
  EllipseModel* model = dynamic_cast<EllipseModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  const te::color::RGBAColor& fillColor = model->getFillColor();
  const te::color::RGBAColor& contourColor = model->getContourColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);

  QPainterPath circle_path;
  circle_path.addEllipse(boundingRect());

  //draws the item
  painter->drawPath(circle_path);

  painter->restore();
}
