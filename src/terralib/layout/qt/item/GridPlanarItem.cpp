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

  double wMargin = mapModel->getDisplacementX();
  double hMargin = mapModel->getDisplacementY();

  te::gm::Envelope newBoxMM(wMargin, hMargin, boxMM.getWidth() + wMargin, boxMM.getHeight() + hMargin);

  model->setMapScale(scale); // put visit
  model->calculateGaps(box);

  drawVerticalLines(painter, box, newBoxMM, scale);
  drawHorizontalLines(painter, box, newBoxMM, scale);

}

void te::layout::GridPlanarItem::drawVerticalLines(QPainter* painter, te::gm::Envelope geoBox, te::gm::Envelope boxMM, double scale)
{
  painter->save();

  GridPlanarModel* model = dynamic_cast<GridPlanarModel*>(m_model);
  if(!model)
  {
    return;
  }

  if(!hasLayer())
  {
    return;
  }

  Utils* utils = Context::getInstance().getUtils();

  te::color::RGBAColor clrLine = model->getLineColor();
  QColor lineColor = rgbaToQColor(clrLine);

  QPen pn(lineColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  // Draw a horizontal line and the y coordinate change(vertical)

  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double y1 = initVerticalLines(geoBox);

  double wtxt = 0;
  double htxt = 0;

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
  painter->save();

  GridPlanarModel* model = dynamic_cast<GridPlanarModel*>(m_model);
  if(!model)
  {
    return;
  }

  if(!hasLayer())
  {
    return;
  }

  Utils* utils = Context::getInstance().getUtils();

  te::color::RGBAColor clrLine = model->getLineColor();
  QColor lineColor = rgbaToQColor(clrLine);

  QPen pn(lineColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  // Draw a vertical line and the x coordinate change(horizontal)
  
  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double x1 = initHorizontalLines(geoBox);
  
  utils = Context::getInstance().getUtils();

  double wtxt = 0;
  double htxt = 0;
;
  for( ; x1 < geoBox.getUpperRightX() ; x1 += model->getLneHrzGap())
  {
    if(x1 < geoBox.getLowerLeftX())
      continue;

    double lly = 0;
    double ury = 0;
    double x = 0;
    transf.system1Tosystem2(x1, geoBox.getLowerLeftY(), x, lly);
    transf.system1Tosystem2(x1, geoBox.getUpperRightY(), x, ury);

    te::gm::Envelope newBox(x, lly, x, ury);

    if(lly > ury)
    {
      double ycopy = lly;
      lly = ury;
      ury = ycopy;
    }

    QLineF line(x, lly, x, ury);
    painter->drawLine(line);

    std::ostringstream convert;
    convert.precision(10);
    double number = x1 / (double)model->getUnit();
    convert << number;

    utils->textBoundingBox(wtxt, htxt, convert.str());

    if(model->isVisibleAllTexts())
    {
      if(model->isBottomText())
      {
        drawText(QPointF(x - (wtxt/2.), lly - model->getLneVrtDisplacement() - htxt), painter, convert.str());
      }

      if(model->isTopText())
      {
        drawText(QPointF(x - (wtxt/2.), ury + model->getLneVrtDisplacement()), painter, convert.str());
      }
    }
  }

  painter->restore();
}

double te::layout::GridPlanarItem::initVerticalLines( te::gm::Envelope geoBox )
{
  double yInit = 0;

  GridPlanarModel* model = dynamic_cast<GridPlanarModel*>(m_model);
  if(!model)
  {
    return yInit;
  }
  
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

  return yInit;
}

double te::layout::GridPlanarItem::initHorizontalLines( te::gm::Envelope geoBox )
{
  double xInit = 0;

  GridPlanarModel* model = dynamic_cast<GridPlanarModel*>(m_model);
  if(!model)
  {
    return xInit;
  }

  xInit = model->getInitialGridPointX();
  if(xInit < geoBox.getLowerLeftX())
  {
    double difx = geoBox.getLowerLeftX() - xInit;
    int nParts = (int)(difx/model->getLneHrzGap());
    if(nParts == 0)
    {
      xInit = model->getInitialGridPointX();
    }
    else
    {
      xInit = xInit + (nParts * model->getLneHrzGap());
    }
  }

  return xInit;
}

bool te::layout::GridPlanarItem::hasLayer()
{
  bool result = false;

  MapItem* item = dynamic_cast<MapItem*>(parentItem());
  if(!item)
  {
    return result;    
  }

  MapModel* mapModel = dynamic_cast<MapModel*>(item->getModel());
  if(!mapModel)
  {
    return result;    
  }

  if(!mapModel->getLayer())
  {
    return result;
  }

  return true;
}







