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
  \file terralib/qt/widgets/se/VisualDockWidget.cpp

  \brief A dock widget used control the vectorial visual using se elements
      and a property browser to show its properties.
*/

// TerraLib
#include "VisualDockWidget.h"

#include "PolygonSymbolizerProperty.h"
#include "LineSymbolizerProperty.h"
#include "PointSymbolizerProperty.h"
#include "RasterVisualWidget.h"
#include "StyleControllerWidget.h"

#include "../../../dataaccess/utils/Utils.h"
#include "../../../se.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../raster/RasterProperty.h"


// Qt
#include <QtGui/QLayout>
#include <QtGui/QScrollArea>


te::qt::widgets::VisualDockWidget::VisualDockWidget(const QString & title, QWidget * parent, Qt::WindowFlags flags)
  : QDockWidget(title, parent, flags), m_currentLayer(0)
{
  //build form
  this->setWidget(buildUi());
}

te::qt::widgets::VisualDockWidget::~VisualDockWidget()
{

}

void te::qt::widgets::VisualDockWidget::setStyle(te::se::Style* style, te::map::AbstractLayer* layer)
{
  m_styleController->setStyle(style);

  m_currentLayer = layer;

  setTabStatus(false);
}

void te::qt::widgets::VisualDockWidget::setTabStatus(bool status)
{
  m_tabWidget->setTabEnabled(0, status); //polygons
  m_tabWidget->setTabEnabled(1, status); //lines
  m_tabWidget->setTabEnabled(2, status); //points
  m_tabWidget->setTabEnabled(3, status); //raster
}

void te::qt::widgets::VisualDockWidget::updateUi()
{
}

QWidget* te::qt::widgets::VisualDockWidget::buildUi()
{
  //widget
  QWidget* w = new QWidget(this);

  QGridLayout* layout = new QGridLayout(w);

  //style explorer
  m_styleController = new te::qt::widgets::StyleControllerWidget(w);
  layout->addWidget(m_styleController, 0, 0);
  m_styleController->setMaximumHeight(300);

  connect(m_styleController, SIGNAL(symbolizerSelected(te::se::Symbolizer*)), this, SLOT(onSymbolizerSelected(te::se::Symbolizer*)));
  connect(this, SIGNAL(symbolizerChanged(te::se::Symbolizer*)), m_styleController, SLOT(onSymbolizerChanged(te::se::Symbolizer*)));

  //tab widget
  m_tabWidget = new QTabWidget(w);
  m_tabWidget->setTabPosition(QTabWidget::South);
  layout->addWidget(m_tabWidget, 1, 0);
  m_tabWidget->setMinimumWidth(360);

  //polygons
  m_polyWidget = new te::qt::widgets::PolygonSymbolizerProperty(m_tabWidget);

  m_tabWidget->addTab(m_polyWidget, tr("Polygons"));

  connect(m_polyWidget, SIGNAL(symbolizerChanged()), this, SLOT(onPolygonSymbolizerChanged()));

  //lines
  m_lineWidget = new te::qt::widgets::LineSymbolizerProperty(m_tabWidget);

  m_tabWidget->addTab(m_lineWidget, tr("Lines"));

  connect(m_lineWidget, SIGNAL(symbolizerChanged()), this, SLOT(onLineSymbolizerChanged()));

  //points
  m_pointWidget = new te::qt::widgets::PointSymbolizerProperty(m_tabWidget);

  m_tabWidget->addTab(m_pointWidget, tr("Points"));

  connect(m_pointWidget, SIGNAL(symbolizerChanged()), this, SLOT(onPointSymbolizerChanged()));

  //raster
  QWidget* dummyW = new QWidget(m_tabWidget);

  m_tabWidget->addTab(dummyW, tr("Raster"));

  QGridLayout* ldummy = new QGridLayout(dummyW);

  QScrollArea* sa = new QScrollArea(dummyW);

  ldummy->addWidget(sa);

  m_visualWidget = new te::qt::widgets::RasterVisualWidget(sa);
  m_visualWidget->setVerticalLayout();

  sa->setWidget(m_visualWidget);

  connect(m_visualWidget, SIGNAL(symbolizerChanged()), this, SLOT(onRasterSymbolizerChanged()));

  setTabStatus(false);

  return w;
}

void te::qt::widgets::VisualDockWidget::onSymbolizerSelected(te::se::Symbolizer* s)
{
  setTabStatus(false);

  if(s->getType() == "PolygonSymbolizer")
  {
    m_tabWidget->setCurrentWidget(m_polyWidget);

    m_tabWidget->setTabEnabled(0, true);

    m_polyWidget->setSymbolizer(dynamic_cast<te::se::PolygonSymbolizer*>(s));
  }
  else if(s->getType() == "LineSymbolizer")
  {
    m_tabWidget->setCurrentWidget(m_lineWidget);

    m_tabWidget->setTabEnabled(1, true);

    m_lineWidget->setSymbolizer(dynamic_cast<te::se::LineSymbolizer*>(s));
  }
  else if(s->getType() == "PointSymbolizer")
  {
    m_tabWidget->setCurrentWidget(m_pointWidget);

    m_tabWidget->setTabEnabled(2, true);

    m_pointWidget->setSymbolizer(dynamic_cast<te::se::PointSymbolizer*>(s));
  }
  else if(s->getType() == "RasterSymbolizer")
  {
    m_tabWidget->setCurrentIndex(3);

    m_tabWidget->setTabEnabled(3, true);

    te::rst::RasterProperty* prop = te::da::GetFirstRasterProperty(m_currentLayer->getSchema());

    m_visualWidget->setRasterSymbolizer(dynamic_cast<te::se::RasterSymbolizer*>(s));

    m_visualWidget->setBandProperty(prop->getBandProperties());
  }
}

void te::qt::widgets::VisualDockWidget::onPolygonSymbolizerChanged()
{
  te::se::Symbolizer* s = m_polyWidget->getSymbolizer();

  emit symbolizerChanged(s);
}

void te::qt::widgets::VisualDockWidget::onLineSymbolizerChanged()
{
  te::se::Symbolizer* s = m_lineWidget->getSymbolizer();

  emit symbolizerChanged(s);
}

void te::qt::widgets::VisualDockWidget::onPointSymbolizerChanged()
{
  te::se::Symbolizer* s = m_pointWidget->getSymbolizer();

  emit symbolizerChanged(s);
}

void te::qt::widgets::VisualDockWidget::onRasterSymbolizerChanged()
{
  te::se::Symbolizer* s = m_visualWidget->getRasterSymbolizer();

  emit symbolizerChanged(s);
}
