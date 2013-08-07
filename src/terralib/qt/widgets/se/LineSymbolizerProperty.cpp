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
  \file terralib/qt/widgets/se/LineSymbolizerProperty.cpp

  \brief A widget used to define the line symbolizer
         properties, such as:
           - basic stroke
*/

#include "LineSymbolizerProperty.h"

#include "../../../se/LineSymbolizer.h"

#include "BasicStrokePropertyItem.h"

// Qt
#include <QtGui/QGridLayout>

// STL
#include <cassert>

te::qt::widgets::LineSymbolizerProperty::LineSymbolizerProperty(QWidget* parent) : m_symb(new te::se::LineSymbolizer)
{
  QGridLayout* layout = new QGridLayout(this);

  this->setLayout(layout);

  m_propertyBrowser = new QtTreePropertyBrowser(this);

  layout->addWidget(m_propertyBrowser);

  m_propertyBrowser->setPropertiesWithoutValueMarked(true);
  m_propertyBrowser->setRootIsDecorated(false);
  m_propertyBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  m_bs = new te::qt::widgets::BasicStrokePropertyItem(m_propertyBrowser);

  connect(m_bs, SIGNAL(strokeChanged()), this, SLOT(onStrokeChanged()));
}

te::qt::widgets::LineSymbolizerProperty::~LineSymbolizerProperty()
{
}

void te::qt::widgets::LineSymbolizerProperty::setSymbolizer(te::se::LineSymbolizer* symb)
{
  m_symb = symb;

  if(m_symb->getStroke())
	m_bs->setStroke(m_symb->getStroke());
}

te::se::Symbolizer* te::qt::widgets::LineSymbolizerProperty::getSymbolizer() const
{
  return m_symb;
}

void te::qt::widgets::LineSymbolizerProperty::onStrokeChanged()
{
  m_symb->setStroke(m_bs->getStroke());

  emit symbolizerChanged();
}
