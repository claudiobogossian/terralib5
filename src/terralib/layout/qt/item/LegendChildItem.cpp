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
  \file LegendChildItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendChildItem.h"
//#include "../../core/pattern/mvc/ItemController.h"
//#include "../core/Scene.h"
//#include "../../core/pattern/mvc/Observable.h"
//#include "../../../color/RGBAColor.h"
//#include "../../../qt/widgets/Utils.h"
//#include "../../../geometry/Envelope.h"
//#include "../../../common/STLUtils.h"

// Qt
#include <QPixmap>
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "AbstractItem.h"

te::layout::LegendChildItem::LegendChildItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix ) 
  : AbstractItem<QGraphicsItem>(controller, model)
{  
  /*this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);

  m_nameClass = std::string(this->metaObject()->className());*/
}


te::layout::LegendChildItem::~LegendChildItem()
{

}

void te::layout::LegendChildItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  /*QRectF boundRect;
  boundRect = boundingRect();

  painter->save();
  painter->translate( -boundRect.bottomLeft().x(), -boundRect.topRight().y() );  
  QRectF rtSource( 0, 0, m_pixmap.width(), m_pixmap.height() );
  painter->drawPixmap(boundRect, m_pixmap, rtSource);
  painter->restore();  */
}



