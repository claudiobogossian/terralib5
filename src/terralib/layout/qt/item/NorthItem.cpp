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
  \file NorthItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NorthItem.h"
#include "../../item/NorthModel.h"

te::layout::NorthItem::NorthItem( ItemController* controller, Observable* o, bool invertedMatrix ) :
  ObjectItem(controller, o, invertedMatrix)
{  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::NorthItem::~NorthItem()
{

}

void te::layout::NorthItem::drawItem( QPainter * painter )
{
  NorthModel* model = dynamic_cast<NorthModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  QPainterPath rect_path;
  rect_path.addRect(boundingRect());

  QColor cpen(0,0,0);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  te::color::RGBAColor clrBack = model->getBackgroundColor();

  QColor cbrush;
  cbrush.setRed(clrBack.getRed());
  cbrush.setGreen(clrBack.getGreen());
  cbrush.setBlue(clrBack.getBlue());
  cbrush.setAlpha(clrBack.getAlpha());

  painter->setBrush(cbrush);
  painter->drawPath(rect_path);

  painter->restore();
}
