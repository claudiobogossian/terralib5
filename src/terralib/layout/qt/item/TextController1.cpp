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

// TerraLib
#include "TextController1.h"

#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../qt/item/TextItem.h"

#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QFontMetricsF>

te::layout::TextController1::TextController1( te::layout::AbstractItemModel* model)
  : AbstractItemController(model)
{
}

te::layout::TextController1::~TextController1()
{
}

QSizeF te::layout::TextController1::updateView()
{
  TextItem* view = dynamic_cast<TextItem*>(m_view);
  if(view != 0)
  {
    const Property& pText = getProperty("text");
    const Property& pFont = getProperty("font");

    const std::string& textModel = pText.getValue().toString();
    Font ft = pFont.getValue().toFont();

    QFont qft;
    qft.setFamily(ft.getFamily().c_str());
    qft.setBold(ft.isBold());
    qft.setItalic(ft.isItalic());
    qft.setPointSizeF(ft.getPointSize());
    qft.setKerning(ft.isKerning());
    qft.setStrikeOut(ft.isStrikeout());
    qft.setUnderline(ft.isUnderline());

    view->setFont(qft);
    view->setPlainText(textModel.c_str());
    view->adjustSize();

    //we get the size in pixels
    QSizeF size = view->document()->size(); 
    QRectF boundingRect(QPointF(0, 0), size);

    //then we convert to millimiters
    boundingRect = view->mapRectToScene(boundingRect);

    //we finally return the new size in millimiters
    return boundingRect.size();
  }

  return QSizeF();
}

void te::layout::TextController1::updateModel(const QSizeF& size)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property propertyWidth(0);
  propertyWidth.setName("width");
  propertyWidth.setValue(size.width(), dataType->getDataTypeDouble());

  Property propertyHeight(0);
  propertyHeight.setName("height");
  propertyHeight.setValue(size.height(), dataType->getDataTypeDouble());

  Properties properties("");
  properties.addProperty(propertyWidth);
  properties.addProperty(propertyHeight);

  m_model->setProperties(properties);
}

void te::layout::TextController1::update(const te::layout::Subject* subject)
{
  const Property& pWidth = getProperty("width");
  const Property& pHeight = getProperty("height");

  //gets the current size of the view
  QSizeF currentSize(pWidth.getValue().toDouble(), pHeight.getValue().toDouble());

  //updates the view with the current model configuration
  QSizeF newSize = updateView();

  //if the size of the view needed to be changed, we update the model
  if(currentSize != newSize)
  {
    updateModel(newSize);
    return;
  }

  AbstractItemController::update(subject);
}

bool te::layout::TextController1::itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & currentValue, QVariant& newValue )
{
  if(change == QGraphicsItem::ItemSceneHasChanged)
  {
    TextItem* item = dynamic_cast<TextItem*>(m_view);
    if(item != 0)
    {
      Scene* myScene = dynamic_cast<Scene*>(item->scene());

      if(myScene != 0)
      {
        QTransform transfView = myScene->sceneTransform();
        QTransform transfItem =item->transform();
        if(transfItem != transfView.inverted())
        {
          QTransform transf = transfView.inverted();
          item->setTransform(transf);

          QSizeF size = updateView();
          updateModel(size);
        }
      }
    }
  }

  return false;
}
