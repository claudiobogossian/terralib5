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
  \file QHorizontalRulerLayoutItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QHorizontalRulerLayoutItem.h"
#include "LayoutItemController.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutItemModelObservable.h"
#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"

// Qt
#include <QtGui/QGraphicsRectItem>
#include <QPainter>

te::layout::QHorizontalRulerLayoutItem::QHorizontalRulerLayoutItem( LayoutItemController* controller, LayoutObservable* o ) :
  QObjectLayoutItem(controller, o)
{
  m_printable = false;
}

te::layout::QHorizontalRulerLayoutItem::~QHorizontalRulerLayoutItem()
{

}

void te::layout::QHorizontalRulerLayoutItem::updateObserver( ContextLayoutItem context )
{
  te::color::RGBAColor** rgba = context.getPixmap();

  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  QPixmap pixmap;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmap.fill(Qt::transparent);
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}


