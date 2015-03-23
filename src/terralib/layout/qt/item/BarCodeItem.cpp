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
  \file BarCodeItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BarCodeItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/BarCodeModel.h"
#include "../../core/Font.h"

// Qt
#include <QTextDocument>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>

te::layout::BarCodeItem::BarCodeItem( ItemController* controller, Observable* o ) :
  TextItem(controller, o)
{
  m_invertedMatrix = true;
  m_nameClass = std::string(this->metaObject()->className());

  BarCodeModel* model = dynamic_cast<BarCodeModel*>(m_model);
  if(model)
  {
    Font font = model->getFont();
    QFont ft(font.getFamily().c_str(), font.getPointSize());
    document()->setDefaultFont(ft);
    document()->setPlainText("Teste BarCode");
  }  
}

te::layout::BarCodeItem::~BarCodeItem()
{

}

void te::layout::BarCodeItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /* = 0 */ )
{
  QGraphicsTextItem::paint(painter, option, widget);
}
