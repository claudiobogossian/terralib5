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

  \brief A widget used to define the point symbolizer
         properties, such as:
           - general
           - mark
              - basic fill
              - basic stroke
*/


#include "../../../se/PointSymbolizer.h"
#include "../../../se/Graphic.h"
#include "GraphicProperty.h"
#include "PointSymbolizerProperty.h"

// Qt
#include <QGridLayout>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

// STL
#include <cassert>

te::qt::widgets::PointSymbolizerProperty::PointSymbolizerProperty(QWidget* parent) :  m_setLocalSymbol(false), m_symb(new te::se::PointSymbolizer)
{
  QGridLayout* layout = new QGridLayout(this);

  this->setLayout(layout);

  m_graphProp = new te::qt::widgets::GraphicProperty(this);

  layout->addWidget(m_graphProp);
  layout->setContentsMargins(0,0,0,0);
  
  connect(m_graphProp, SIGNAL(graphicChanged()), this, SLOT(onGraphicChanged()));

  // Setups initial point symbolizer
  m_symb->setGraphic(m_graphProp->getGraphic());
}

te::qt::widgets::PointSymbolizerProperty::~PointSymbolizerProperty()
{
}

void te::qt::widgets::PointSymbolizerProperty::setSymbolizer( te::se::PointSymbolizer* symb)
{
  m_symb = symb;

  m_setLocalSymbol = true;

  m_graphProp->setGraphic(m_symb->getGraphic());

  m_setLocalSymbol = false;
}

te::se::Symbolizer* te::qt::widgets::PointSymbolizerProperty::getSymbolizer() const
{
  return m_symb;
}

void te::qt::widgets::PointSymbolizerProperty::onGraphicChanged()
{
  m_symb->setGraphic(m_graphProp->getGraphic());

  if(!m_setLocalSymbol)
    emit symbolizerChanged();
}
