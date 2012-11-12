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
#include "RasterVisualDockWidget.h"
#include "RasterVisualWidget.h"

#include "../../../maptools/RasterLayer.h"
#include "../../../raster/RasterProperty.h"

// Qt
#include <QtGui/QLayout> 

// STL


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

void te::qt::widgets::RasterVisualDockWidget::setRasterLayer(te::map::RasterLayer* rl)
{
  m_layer = rl;

  if(rl->getRasterSymbolizer())
  {
    te::rst::RasterProperty* prop = 0;
  
    prop = (te::rst::RasterProperty*)m_layer->getRasterProperty()->clone();

    te::se::RasterSymbolizer* rs = (te::se::RasterSymbolizer*)m_layer->getRasterSymbolizer()->clone();

    disconnect(m_visualWidget, SIGNAL(symbolizerChanged()), this, SLOT(onSymbolizerUpdated()));

    m_visualWidget->setBandProperty(prop->getBandProperties());

    m_visualWidget->setRasterSymbolizer(rs);

    connect(m_visualWidget, SIGNAL(symbolizerChanged()), this, SLOT(onSymbolizerUpdated()));

    delete rs;

    delete prop;
  }
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
