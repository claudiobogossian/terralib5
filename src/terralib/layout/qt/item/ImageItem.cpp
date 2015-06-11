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
  \file ImageItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ImageItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/ImageModel.h"

// Qt
#include <QStyleOptionGraphicsItem>

te::layout::ImageItem::ImageItem( ItemController* controller, Observable* o, bool invertedMatrix ) :
  ObjectItem(controller, o, invertedMatrix)
{
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::ImageItem::~ImageItem()
{

}

void te::layout::ImageItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);

  drawImage(painter);

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::ImageItem::drawImage( QPainter * painter )
{
  ImageModel* model = dynamic_cast<ImageModel*>(m_model);
  if(!model)
  {
    return;
  }

  QRectF boundRect = boundingRect();
  std::string fileName = model->getFileName();

  if(fileName.compare("") == 0)
    return;

  painter->save();

  QImage img(fileName.c_str());
  img = img.mirrored();

  QRectF sourceRect(0, 0, img.width(), img.height());

  painter->drawImage(boundRect, img, sourceRect);

  painter->restore();
}

