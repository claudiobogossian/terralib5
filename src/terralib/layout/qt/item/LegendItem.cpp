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
  \file LegendItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendItem.h"
#include "..\..\core\pattern\mvc\AbstractItemController.h"
#include "../../core/pattern/singleton/Context.h"
//#include "../../core/pattern/mvc/ItemController.h"
//#include "../core/Scene.h"
//#include "../../core/pattern/mvc/Observable.h"
//#include "../../../color/RGBAColor.h"
//#include "../../../qt/widgets/Utils.h"
//#include "../../../geometry/Envelope.h"
//#include "../../../geometry/Geometry.h"
//#include "../../../geometry/Polygon.h"
//#include "../../../geometry/LinearRing.h"
//#include "../../../common/STLUtils.h"
//#include "../../item/LegendModel.h"
//#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/GroupingItem.h"
#include "../../../maptools/Grouping.h"
#include "../../../maptools/Canvas.h"
#include "../../../maptools/CanvasConfigurer.h"
#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../se/Symbolizer.h"
#include "../../../se/PolygonSymbolizer.h"
#include "../../../se/Fill.h"
//#include "../../../se/Utils.h"
//#include "../../../color/RGBAColor.h"


// Qt
#include <QPixmap>
#include <QStyle>
#include <QStyleOption>
#include <QFont>
#include <QPaintDevice>
#include <QColor>
#include <QMatrix>

te::layout::LegendItem::LegendItem(AbstractItemController* controller, AbstractItemModel* model )
  : AbstractItem<QGraphicsItem>(controller, model, true)
  //m_move(false)
{  
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);

  //m_nameClass = std::string(this->metaObject()->className());

  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
}

te::layout::LegendItem::~LegendItem()
{

}

//void te::layout::LegendItem::updateObserver( ContextItem context )
//{
//  if(!m_model)
//    return;
//
//  Utils* utils = Context::getInstance().getUtils();
//
//  if(!utils)
//    return;
//
//  te::gm::Envelope box = m_model->getBox();
//
//  if(!box.isValid())
//    return;
//  
//  double widthInPixels = utils->mm2pixel(box.getWidth());
//  double heightInPixels = utils->mm2pixel(box.getHeight());
//
//  this->setRect(QRectF(0, 0, widthInPixels, heightInPixels));
//
//  refresh();
//
//  update();
//}

void te::layout::LegendItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  te::layout::Utils* utils = Context::getInstance().getUtils(); 
  te::map::Canvas* canvas = Context::getInstance().getCanvas();

  const Property& pFont = m_controller->getProperty("font");
  const Property& pFontColor = m_controller->getProperty("font_color");
  const Property& pLayers = m_controller->getProperty("layers");

  const Font& font = pFont.getValue().toFont();
  const te::color::RGBAColor& fontColor = pFontColor.getValue().toColor();
  const std::list<te::map::AbstractLayerPtr>& layerList = pLayers.getValue().toGenericVariant().toLayerList();

  double borderDisplacement(1);
  double displacementBetweenTitleAndSymbols(7);
  double displacementBetweenSymbols(7);
  double displacementBetweenSymbolsAndText(2);
  double symbolsize(5);

  int borderDisplacementInPixels = utils->mm2pixel(borderDisplacement);
  int dispBetweenSymbolAndTextInPixels = utils->mm2pixel(displacementBetweenSymbolsAndText);
  int dispBetweenSymbolsInPixels = utils->mm2pixel(displacementBetweenSymbols);
  int dispBetweenTitleAndSymbolsInPixels = utils->mm2pixel(displacementBetweenTitleAndSymbols);
  int symbolSizeInPixels = utils->mm2pixel(symbolsize);

  //int borderDisplacementInPixels = utils->mm2pixel(legendModel->getBorderDisplacement());
  //int dispBetweenSymbolAndTextInPixels = utils->mm2pixel(legendModel->getDisplacementBetweenSymbolAndText());
  //int dispBetweenSymbolsInPixels = utils->mm2pixel(legendModel->getDisplacementBetweenSymbols());
  //int dispBetweenTitleAndSymbolsInPixels = utils->mm2pixel(legendModel->getDisplacementBetweenTitleAndSymbols());
  //int symbolSizeInPixels = utils->mm2pixel(legendModel->getSymbolSize());


  if(layerList.empty() == true)
  {
    return;
  }

  const te::map::AbstractLayerPtr& layer = *layerList.begin();

  std::string title = layer->getTitle();

  QString qTitle (title.c_str());

  QRectF boundRect = this->boundingRect();
  QMatrix matrix = painter->matrix();

  QFont qfont (QString(font.getFamily().c_str()), font.getPointSize());
  QColor qFontColor (fontColor.getRed(), fontColor.getGreen(), fontColor.getBlue(), fontColor.getAlpha());

  double x1 = boundRect.x() + borderDisplacementInPixels;
  double y1 = boundRect.y() - borderDisplacementInPixels;

  canvas->setTextPointSize(font.getPointSize());
  canvas->setTextUnderline(font.isUnderline());
  canvas->setTextStrikeOut(font.isStrikeout());
  canvas->setTextColor(fontColor);

  double wtxtInPixels = 0.;
  double htxtInPixels = 0.;

  utils->textBoundingBox(wtxtInPixels, htxtInPixels, title);
  
  QRectF rectTitle (x1, y1, wtxtInPixels, htxtInPixels);

  painter->setFont(qfont);
  painter->setBrush(qFontColor);

  painter->drawText(rectTitle, qTitle);

  y1 += dispBetweenTitleAndSymbolsInPixels;

  double widthBox = 0.;
  double heightBox = 0.;

  te::map::Grouping* grouping = layer->getGrouping();

  if (grouping != 0 && grouping->isVisible() == true)
  {
    std::string propertyName = grouping->getPropertyName();

    const std::vector<te::map::GroupingItem*>& items = grouping->getGroupingItems();

    te::map::GroupingType type = grouping->getType();

    double labelX1 = x1 + symbolSizeInPixels + dispBetweenSymbolAndTextInPixels;

    for (unsigned int i = 0; i < items.size(); ++i)
    {
      std::string label = propertyName;
      label += ": ";

      te::map::GroupingItem* item = items[i];

      if (type == te::map::UNIQUE_VALUE)
      {
        label += item->getValue();
      }
      else
      {
        const std::string& upperLimit = item->getUpperLimit();
        const std::string& lowerLimit = item->getLowerLimit();

        label += lowerLimit;
        label += " ~ ";
        label += upperLimit;
      }
      painter->save();
      painter->setFont(qfont);
      painter->setBrush(qFontColor);

      utils->textBoundingBox(wtxtInPixels, htxtInPixels, label);

      QRectF labelRect (labelX1, y1, wtxtInPixels, htxtInPixels);
      QString qLabel (label.c_str());
      painter->drawText(labelRect, qLabel);

      painter->restore();

      const std::vector<te::se::Symbolizer*>& symbolizers = item->getSymbolizers();

      te::qt::widgets::Canvas geomCanvas (painter->device());
      geomCanvas.setMatrix(matrix);

      foreach (te::se::Symbolizer* symbol, symbolizers)
      {
        double offset = 2.0;
        QRectF geomRect (x1, y1, symbolSizeInPixels, symbolSizeInPixels);
        
        te::gm::Geometry* geom = 0;
        if (symbol->getType() == "PolygonSymbolizer")
        {
          te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
          te::gm::LinearRing* ring = new te::gm::LinearRing(5, te::gm::LineStringType);
          ring->setPoint(0, x1 + offset, y1 + offset);
          ring->setPoint(1, x1 + geomRect.width() - offset, y1 + offset);
          ring->setPoint(2, x1 + geomRect.width() - offset, y1 + geomRect.height() - offset);
          ring->setPoint(3, x1 + offset, y1 + geomRect.height() - offset);
          ring->setPoint(4, x1 + offset, y1 + offset);
          polygon->setRingN(0, ring);
          geom = polygon;
        }
        else if (symbol->getType() == "LineSymbolizer")
        {
          te::gm::LineString* line = new te::gm::LineString(2, te::gm::LineStringType);
          line->setPoint(0, x1 + offset, y1 + geomRect.height() * 0.5);
          line->setPoint(1, x1 + geomRect.width() - offset, y1 + geomRect.height() * 0.5);
          geom = line;
        }
        else if (symbol->getType() == "PointSymbolizer")
        {
          geom = new te::gm::Point( x1 +geomRect.width() * 0.5, y1 +geomRect.height() * 0.5);
        }

        // Configuring...
        te::map::CanvasConfigurer cc(&geomCanvas);
        cc.config(symbol);
        
        // Let's draw!
        geomCanvas.draw(geom);

        if(widthBox < (labelRect.width() + symbolSizeInPixels))
        {
          widthBox = labelRect.width() + symbolSizeInPixels;
        }
        
        heightBox+= symbolSizeInPixels + dispBetweenSymbolsInPixels;
      }

      y1 += dispBetweenSymbolsInPixels;
    }
  }

  //this->setRect(QRectF(0, 0, widthBox, heightBox));
}

//QVariant te::layout::LegendItem::itemChange( GraphicsItemChange change, const QVariant & value )
//{
//  if(change == QGraphicsItem::ItemPositionChange && !m_move)
//  {
//    // value is the new position.
//    QPointF newPos = value.toPointF();
//    double h = 0;
//
//    newPos.setX(newPos.x() - transform().dx());
//    newPos.setY(newPos.y() - transform().dy() + h);
//    return newPos;
//  }
//  else if(change == QGraphicsItem::ItemPositionHasChanged)
//  {
//    refresh();
//    m_move = false;
//  }
//
//  return QGraphicsItem::itemChange(change, value);
//}
//
//void te::layout::LegendItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
//{
//  m_move = true;
//
//  QGraphicsItem::mouseMoveEvent(event);
//}
