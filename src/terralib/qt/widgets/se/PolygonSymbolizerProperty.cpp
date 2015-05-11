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
  \file terralib/qt/widgets/se/PolygonSymbolizerProperty.cpp

  \brief A widget used to define the polygon symbolizer
         properties, such as:
           - basic fill
           - basic stroke
*/

// Terralib
#include "../../../se/Fill.h"
#include "../../../se/PolygonSymbolizer.h"
#include "BasicFillPropertyItem.h"
#include "BasicStrokePropertyItem.h"
#include "GraphicProperty.h"
#include "PolygonSymbolizerProperty.h"

// Qt
#include <QGridLayout>
#include <QToolBox>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

// STL
#include <cassert>

te::qt::widgets::PolygonSymbolizerProperty::PolygonSymbolizerProperty(QWidget* parent) :  m_setLocalSymbol(false), m_symb(new te::se::PolygonSymbolizer)
{
  QGridLayout* layout = new QGridLayout(this);
  this->setLayout(layout);

  QToolBox* tb = new QToolBox(this);
  layout->addWidget(tb);

  QtTreePropertyBrowser* basicPropBrowser = new QtTreePropertyBrowser(this);
  basicPropBrowser->setIndentation(10);
  basicPropBrowser->setPropertiesWithoutValueMarked(true);
  basicPropBrowser->setRootIsDecorated(false);
  basicPropBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  layout->addWidget(basicPropBrowser);
  
  m_bf = new te::qt::widgets::BasicFillPropertyItem(basicPropBrowser);
  m_bs = new te::qt::widgets::BasicStrokePropertyItem(basicPropBrowser);

  tb->addItem(basicPropBrowser, "Basic Symbology");

  m_graph = new te::qt::widgets::GraphicProperty(this);

  tb->addItem(m_graph, "Graphic Symbology");

  connect(m_bf, SIGNAL(fillChanged()), this, SLOT(onFillChanged()));
  connect(m_bs, SIGNAL(strokeChanged()), this, SLOT(onStrokeChanged()));
  connect(m_graph, SIGNAL(graphicChanged()), this, SLOT(onPolyGraphicChanged()));
}

te::qt::widgets::PolygonSymbolizerProperty::~PolygonSymbolizerProperty()
{
}

void te::qt::widgets::PolygonSymbolizerProperty::setSymbolizer(te::se::PolygonSymbolizer* symb)
{
  m_symb = symb;

  m_setLocalSymbol = true;

  if(m_symb->getFill())
  {
    if(m_symb->getFill()->getGraphicFill())
      m_graph->setGraphic(m_symb->getFill()->getGraphicFill());
    else
      m_bf->setFill(m_symb->getFill());
  }

  if(m_symb->getStroke())
    m_bs->setStroke(m_symb->getStroke());

  m_setLocalSymbol = false;
}

te::se::Symbolizer* te::qt::widgets::PolygonSymbolizerProperty::getSymbolizer() const
{
  return m_symb;
}

void te::qt::widgets::PolygonSymbolizerProperty::onStrokeChanged()
{
  m_symb->setStroke(m_bs->getStroke());

  emit symbolizerChanged();
}

void te::qt::widgets::PolygonSymbolizerProperty::onFillChanged()
{
  m_symb->setFill(m_bf->getFill());

  emit symbolizerChanged();
}

void te::qt::widgets::PolygonSymbolizerProperty::onPolyGraphicChanged()
{
  if(m_symb->getFill())
  {
    te::se::Fill* fill = m_symb->getFill()->clone();
  
    fill->setGraphicFill(m_graph->getGraphic());

    m_symb->setFill(fill);

    if(!m_setLocalSymbol)
      emit symbolizerChanged();
  }
}
