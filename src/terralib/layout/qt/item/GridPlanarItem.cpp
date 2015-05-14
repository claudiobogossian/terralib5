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
#include "../../item/GridPlanarModel.h"
#include "MapItem.h"
#include "../../core/WorldTransformer.h"
#include "../../item/MapModel.h"
#include "../../../maptools/AbstractLayer.h"

//Qt
#include <QStyleOptionGraphicsItem>

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
  if(!parentItem())
  {
    return;
  }

  GridPlanarModel* model = dynamic_cast<GridPlanarModel*>(m_model);
  if(!model)
  {
    return;
  }

  MapItem* item = dynamic_cast<MapItem*>(parentItem());
  if(!item)
  {
    return;    
  }

  MapModel* mapModel = dynamic_cast<MapModel*>(item->getModel());
  if(!mapModel)
  {
    return;    
  }

  if(!mapModel->getLayer())
  {
    return;
  }

  int srid = mapModel->getLayer()->getSRID();
  if(srid <= 0)
    return;

  double scale = mapModel->getScale();
  te::gm::Envelope box = mapModel->getWorldInMeters();
  te::gm::Envelope boxMM = mapModel->getMapBox();

  model->setMapScale(scale); // put visit
  model->calculateGaps(box);

  drawVerticalLines(painter, box, boxMM, scale);

}

void te::layout::GridPlanarItem::drawVerticalLines(QPainter* painter, te::gm::Envelope geoBox, te::gm::Envelope boxMM, double scale)
{
  painter->save();

  GridPlanarModel* model = dynamic_cast<GridPlanarModel*>(m_model);
  if(!model)
  {
    return;
  }

  MapItem* item = dynamic_cast<MapItem*>(parentItem());
  if(!item)
  {
    return;    
  }

  MapModel* mapModel = dynamic_cast<MapModel*>(item->getModel());
  if(!mapModel)
  {
    return;    
  }

  if(!mapModel->getLayer())
  {
    return;
  }

  Utils* utils = Context::getInstance().getUtils();

  // Draw a horizontal line and the y coordinate change(vertical)

  double			y1;
  double			yInit;

  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  yInit = model->getInitialGridPointY();
  if(yInit < geoBox.getLowerLeftY())
  {
    double dify = geoBox.getLowerLeftY() - yInit;
    int nParts = (int)(dify/model->getLneVrtGap());
    if(nParts == 0)
    {
      yInit = model->getInitialGridPointY();
    }
    else
    {
      yInit = yInit + (nParts * model->getLneVrtGap());
    }
  }

  double wtxt = 0;
  double htxt = 0;

  y1 = yInit;
  for( ; y1 < geoBox.getUpperRightY() ; y1 += model->getLneVrtGap())
  {
    if(y1 < geoBox.getLowerLeftY())
      continue;

    double llx = 0;
    double urx = 0;
    double y = 0;
    transf.system1Tosystem2(geoBox.getLowerLeftX(), y1, llx, y);
    transf.system1Tosystem2(geoBox.getUpperRightX(), y1, urx, y);

    QLineF line(llx, y, urx, y);
    painter->drawLine(line);

    std::ostringstream convert;
    convert.precision(10);
    double number = y1 / (double)model->getUnit();
    convert << number;

    utils->textBoundingBox(wtxt, htxt, convert.str());

    if(model->isVisibleAllTexts())
    {
      if(model->isLeftText())
      {
        drawText(QPointF(llx - model->getLneHrzDisplacement() - wtxt, y), painter, convert.str());
      }

      if(model->isRightText())
      {
        drawText(QPointF(urx + model->getLneHrzDisplacement(), y), painter, convert.str());
      }
    }
  }
  painter->restore();
}

void te::layout::GridPlanarItem::drawHorizontalLines(QPainter* painter, te::gm::Envelope geoBox, te::gm::Envelope boxMM, double scale)
{
  
}



