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
  \file GridGeodesicItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridGeodesicItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/GridMapModel.h"
#include "MapItem.h"
#include "../../core/WorldTransformer.h"
#include "../../item/MapModel.h"

//Qt
#include <QStyleOptionGraphicsItem>

te::layout::GridGeodesicItem::GridGeodesicItem( ItemController* controller, Observable* o ) :
  GridMapItem(controller, o)
{  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::GridGeodesicItem::~GridGeodesicItem()
{

}

void te::layout::GridGeodesicItem::drawGrid( QPainter* painter )
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
    MapItem* item = dynamic_cast<MapItem*>(parentItem());
    if(item)
    {
      MapModel* model = dynamic_cast<MapModel*>(item->getModel());
      double x = model->getDisplacementX();
      double y = model->getDisplacementY();
      parentBound = QRectF(x, y, model->getMapBox().getWidth(), model->getMapBox().getHeight());
    }
    else
    {
      parentBound = parentItem()->boundingRect();
    }
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

  // PostScript to mm
  m_maxHeigthTextMM = m_onePointMM * ft.pointSize();

  QString text = "Geo";

  for (int i = 0; i <= heightRect; i+=10)
  {
    QLineF lineOne = QLineF(parentBound.topLeft().x(), parentBound.topLeft().y() + i, parentBound.topRight().x(), parentBound.topRight().y() + i);

    QPointF pointInit(parentBound.topLeft().x(), parentBound.topLeft().y() + i - (m_maxHeigthTextMM/2)); //left
    drawText(pointInit, painter, text.toStdString(), true);
    QPointF pointFinal(parentBound.topRight().x(), parentBound.topRight().y() + i  - (m_maxHeigthTextMM/2)); //right
    drawText(pointFinal, painter, text.toStdString());

    painter->drawLine(lineOne);

    for (int j = 0; j <= widgetRect; j+=10)
    {
      QLineF lineTwo = QLineF(parentBound.topLeft().x() + j, parentBound.topLeft().y(), parentBound.bottomLeft().x() + j, parentBound.bottomLeft().y());

      QPointF pointInit(parentBound.topLeft().x() + j + (m_maxWidthTextMM/2), boundingRect().topLeft().y() + (m_maxHeigthTextMM)); //lower
      drawText(pointInit, painter, text.toStdString(), true);
      QPointF pointFinal(parentBound.bottomLeft().x() + j  - (m_maxWidthTextMM/2), parentBound.bottomLeft().y()); //upper
      drawText(pointFinal, painter, text.toStdString());

      painter->drawLine(lineTwo);
    }    
  }

  painter->restore();
}



