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
/*
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/ImageModel.h"
*/

// Qt
#include <QStyleOptionGraphicsItem>

te::layout::ImageItem::ImageItem( AbstractItemController* controller, AbstractItemModel* model )
  : AbstractItem<QGraphicsItem>(controller, model)
{
}

te::layout::ImageItem::~ImageItem()
{

}

const std::string& te::layout::ImageItem::getFileName() const
{
  return m_fileName;
}
void te::layout::ImageItem::setFileName(const std::string& fileName)
{
  m_fileName = fileName;
  if(m_fileName.empty() == true)
  {
    m_image = QImage();
  }
  else
  {
    if(m_image.load(m_fileName.c_str()) == true)
    {
      m_image = m_image.mirrored();
    }
  }
}

void te::layout::ImageItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& pFrameColor = m_controller->getProperty("frame_color");
  const te::color::RGBAColor& framwColor = pFrameColor.getValue().toColor();
  QColor qContourColor(framwColor.getRed(), framwColor.getGreen(), framwColor.getBlue(), framwColor.getAlpha());
  
  QPen pen(qContourColor, 0, Qt::SolidLine);

  QRectF boundRect = boundingRect();
  
  if(m_image.isNull() == true)
  {
    painter->save();

    painter->setPen(pen);

    painter->drawRect(boundRect);

    painter->restore();

    return;
  }

  QRectF sourceRect(0, 0, m_image.width(), m_image.height());

  painter->save();

  painter->setPen(pen);
  painter->setRenderHint( QPainter::Antialiasing, true );

  //draws the item
  painter->drawImage(boundRect, m_image, sourceRect);

  painter->restore();
}
