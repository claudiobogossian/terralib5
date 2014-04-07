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
  \file QRectangleLayoutItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QRectangleLayoutItem.h"
#include "LayoutItemController.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutObservable.h"
#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"

te::layout::QRectangleLayoutItem::QRectangleLayoutItem( LayoutItemController* controller, LayoutObservable* o ) :
  QObjectLayoutItem(controller, o)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);
}

te::layout::QRectangleLayoutItem::~QRectangleLayoutItem()
{

}

void te::layout::QRectangleLayoutItem::updateObserver( ContextLayoutItem context )
{
  te::color::RGBAColor** rgba = context.getPixmap();  
  te::gm::Envelope box = m_model->getBox();
  
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
