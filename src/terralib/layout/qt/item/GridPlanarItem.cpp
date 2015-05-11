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
  \file GridPlanarItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/GridMapModel.h"
#include "MapItem.h"

//Qt
#include <QStyleOptionGraphicsItem>
#include "../../core/WorldTransformer.h"

te::layout::GridPlanarItem::GridPlanarItem( ItemController* controller, Observable* o ) :
  GridMapItem(controller, o)
{  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::GridPlanarItem::~GridPlanarItem()
{

}

void te::layout::GridPlanarItem::drawGrid( QPainter* painter )
{
  GridMapModel* model = dynamic_cast<GridMapModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  QRectF parentBound = boundingRect();

  if(parentItem())
  {
    parentBound = parentItem()->boundingRect();
  }

  QPainterPath gridMapPath;
  gridMapPath.setFillRule(Qt::WindingFill);

  int heightRect = (int)parentBound.height();
  int widgetRect = (int)parentBound.width();

  te::color::RGBAColor rgbColor = model->getLineColor();
  QColor cLine(rgbColor.getRed(), rgbColor.getGreen(), rgbColor.getBlue(), rgbColor.getAlpha());

  painter->setPen(QPen(cLine, 0, Qt::SolidLine));

  QFont ft(model->getFontFamily().c_str(), model->getTextPointSize());

  painter->setFont(ft);

  m_maxHeigthTextMM = m_onePointMM * ft.pointSize();

  QString text = "A";

  for (int i = 0; i <= heightRect; i+=10)
  {
    QLineF lineOne = QLineF(parentBound.topLeft().x(), parentBound.topLeft().y() + i, parentBound.topRight().x(), parentBound.topRight().y() + i);

    QPointF pointInit(parentBound.topLeft().x() - (heightRect*.01), parentBound.topLeft().y() + i - (m_maxHeigthTextMM/2)); //esquerda
    drawText(pointInit, painter, text.toStdString(), true);
    QPointF pointFinal(parentBound.topRight().x() + (heightRect*.01), parentBound.topRight().y() + i  - (m_maxHeigthTextMM/2)); //direita
    drawText(pointFinal, painter, text.toStdString());

    painter->drawLine(lineOne);

    for (int j = 0; j <= widgetRect; j+=10)
    {
      QLineF lineTwo = QLineF(parentBound.topLeft().x() + j, parentBound.topLeft().y(), parentBound.bottomLeft().x() + j, parentBound.bottomLeft().y());

      QPointF pointInit(parentBound.topLeft().x() + j + (m_maxWidthTextMM/2), boundingRect().topLeft().y() /*- (widgetRect*.01)*/); //inferior
      drawText(pointInit, painter, text.toStdString(), true);
      QPointF pointFinal(parentBound.bottomLeft().x() + j  - (m_maxWidthTextMM/2), parentBound.bottomLeft().y() + (widgetRect*.01)); //superior
      drawText(pointFinal, painter, text.toStdString());

      painter->drawLine(lineTwo);
    }    
  }

  painter->restore();
}



