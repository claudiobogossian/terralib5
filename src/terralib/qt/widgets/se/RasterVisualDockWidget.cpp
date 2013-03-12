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
  \file terralib/qt/widgets/se/RasterVisualDockWidget.cpp

  \brief A dock widget used control the raster enhancement
*/

// TerraLib
#include "../../../maptools/DataSetLayer.h"
#include "../../../maptools/Utils.h"
#include "../../../raster/RasterProperty.h"
#include "../../../se/CoverageStyle.h"
#include "../../../se/RasterSymbolizer.h"
#include "../../../se/Rule.h"
#include "RasterVisualDockWidget.h"
#include "RasterVisualWidget.h"

// Qt
#include <QtGui/QLayout>

// STL
#include <cassert>

te::qt::widgets::RasterVisualDockWidget::RasterVisualDockWidget(const QString & title, QWidget * parent, Qt::WindowFlags flags)
  : QDockWidget(title, parent, flags), m_layer(0)
{
  m_visualWidget = new te::qt::widgets::RasterVisualWidget(this);

  m_visualWidget->setVerticalLayout();

  this->setWidget(m_visualWidget);

  m_visualWidget->show();

  this->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

  this->layout()->setAlignment(Qt::AlignTop);

  connect(m_visualWidget, SIGNAL(symbolizerChanged()), this, SLOT(onSymbolizerUpdated()));
}

te::qt::widgets::RasterVisualDockWidget::~RasterVisualDockWidget()
{

}

void te::qt::widgets::RasterVisualDockWidget::setLayer(te::map::DataSetLayer* layer)
{
  m_layer = layer;

  te::se::CoverageStyle* style = dynamic_cast<te::se::CoverageStyle*>(m_layer->getStyle());
  assert(style);
  assert(style->getRules().size() > 0);

  const te::se::Rule* rule = style->getRule(0);
  assert(!rule->getSymbolizers().empty());

  te::se::RasterSymbolizer* rs = dynamic_cast<te::se::RasterSymbolizer*>(rule->getSymbolizers()[0]);
  assert(rs);

  te::rst::RasterProperty* prop = te::map::GetRasterProperty(m_layer);

  disconnect(m_visualWidget, SIGNAL(symbolizerChanged()), this, SLOT(onSymbolizerUpdated()));

  m_visualWidget->setBandProperty(prop->getBandProperties());

  m_visualWidget->setRasterSymbolizer(static_cast<te::se::RasterSymbolizer*>(rs->clone()));

  connect(m_visualWidget, SIGNAL(symbolizerChanged()), this, SLOT(onSymbolizerUpdated()));

  delete rs;

  delete prop;
}

void te::qt::widgets::RasterVisualDockWidget::updateUi()
{
}

void te::qt::widgets::RasterVisualDockWidget::onSymbolizerUpdated()
{
  if(m_layer)
  {
    te::se::Rule* rule = new te::se::Rule();
    rule->push_back(m_visualWidget->getRasterSymbolizer());

    te::se::Style* s = new te::se::CoverageStyle();
    s->push_back(rule);

    m_layer->setStyle(s);

    emit symbolizerChanged();
  }
}
