/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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

#include "PointSymbolizerProperty.h"
#include "../../../se/PointSymbolizer.h"
#include "../../../se/Graphic.h"

#include "GraphicPropertyItem.h"

// Qt
#include <QtGui/QGridLayout>

// STL
#include <cassert>

te::qt::widgets::PointSymbolizerProperty::PointSymbolizerProperty(QWidget* parent) : m_symb(new te::se::PointSymbolizer)
{
  QGridLayout* layout = new QGridLayout(this);

  this->setLayout(layout);

  m_propertyBrowser = new QtTreePropertyBrowser(this);

  layout->addWidget(m_propertyBrowser);

  m_propertyBrowser->setPropertiesWithoutValueMarked(true);
  m_propertyBrowser->setRootIsDecorated(false);
  m_propertyBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  m_generalProp = new te::qt::widgets::GraphicPropertyItem(m_propertyBrowser);

  connect(m_generalProp, SIGNAL(graphicChanged()), this, SLOT(onGraphicChanged()));

  // Setups initial point symbolizer
  m_symb->setGraphic(m_generalProp->getGraphic());
}

te::qt::widgets::PointSymbolizerProperty::~PointSymbolizerProperty()
{
}

void te::qt::widgets::PointSymbolizerProperty::setSymbolizer( te::se::PointSymbolizer* symb)
{
  m_symb = symb;

  m_generalProp->setGraphic(m_symb->getGraphic());
}

te::se::Symbolizer* te::qt::widgets::PointSymbolizerProperty::getSymbolizer() const
{
  return m_symb;
}

void te::qt::widgets::PointSymbolizerProperty::onGraphicChanged()
{
  m_symb->setGraphic(m_generalProp->getGraphic());

  emit symbolizerChanged();
}
