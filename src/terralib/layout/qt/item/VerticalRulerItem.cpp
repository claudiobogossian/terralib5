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
  \file VerticalRulerItem.cpp
   
  \brief 

  \ingroup layout
*/


// TerraLib
#include "VerticalRulerItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"

te::layout::VerticalRulerItem::VerticalRulerItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{
  m_printable = false;
  m_canChangeGraphicOrder = false;

  /*this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges 
    | QGraphicsItem::ItemIgnoresTransformations);*/
}

te::layout::VerticalRulerItem::~VerticalRulerItem()
{

}

void te::layout::VerticalRulerItem::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  te::color::RGBAColor** rgba = context.getPixmap();

  if(!rgba)
    return;

  Utils* utils = context.getUtils();

  if(!utils)
    return;

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  if(!box.isValid())
    return;

  QPixmap pixmap;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}

