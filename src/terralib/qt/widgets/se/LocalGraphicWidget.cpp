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
  \file terralib/qt/widgets/se/LocalGraphicWidget.cpp

  \brief A widget used to build a graphic associated with an external graphic element 
         that references a local image. e.g. a SVG file, a PNG file, etc.
*/

// TerraLib
#include "../../../se/ExternalGraphic.h"
#include "../../../se/Graphic.h"
#include "../../../xlink/SimpleLink.h"
#include "LocalGraphicWidget.h"
#include "LocalImageWidget.h"
#include "ui_GraphicWidgetForm.h"

// Qt
#include <QtGui/QGridLayout>

te::qt::widgets::LocalGraphicWidget::LocalGraphicWidget(QWidget* parent, Qt::WindowFlags f)
  : GraphicWidget(parent, f)
{
  // Local Image Widget
  m_localImageWidget = new te::qt::widgets::LocalImageWidget(this);

  // Image Page
  QWidget* imagePage = new QWidget(this);
  
  // Adjusting...
  QGridLayout* layout = new QGridLayout(imagePage);
  layout->addWidget(m_localImageWidget);

  // Adding on graphic tab...
  m_ui->m_graphicTabWidget->addTab(imagePage, tr("&Image"));

  // Setups initial graphic
  m_graphic->add(m_localImageWidget->getExternalGraphic());

  // Signals & slots
  connect(m_localImageWidget, SIGNAL(externalGraphicChanged(const QSize&)), SLOT(onExternalGraphicChanged(const QSize&)));
}

te::qt::widgets::LocalGraphicWidget::~LocalGraphicWidget()
{
}

bool te::qt::widgets::LocalGraphicWidget::setGraphic(const te::se::Graphic* graphic)
{
  GraphicWidget::setGraphic(graphic);

  // Verifying if this widget can deal with the given graphic...
  const std::vector<te::se::ExternalGraphic*> extGraphics = m_graphic->getExternalGraphics();
  if(extGraphics.empty())
    return false;

  te::se::ExternalGraphic* g = extGraphics[0];
  const te::xl::SimpleLink* link = g->getOnlineResource();
  if(link == 0)
    return false;
  
  const std::string href = link->getHref();
  if(href.empty())
    return false;

  QImage img;
  if(!img.load(href.c_str()))
    return false;

  // I know it!
  m_localImageWidget->setExternalGraphic(g);

  return true;
}

QString te::qt::widgets::LocalGraphicWidget::getGraphicType() const
{
  return tr("Local Image");
}

QIcon te::qt::widgets::LocalGraphicWidget::getGraphicIcon(const QSize& size) const
{
  const std::vector<te::se::ExternalGraphic*> extGraphics = m_graphic->getExternalGraphics();
  if(extGraphics.empty())
    return QIcon();

  te::se::ExternalGraphic* g = extGraphics[0];
  const te::xl::SimpleLink* link = g->getOnlineResource();
  if(link == 0)
    return QIcon();

  const std::string href = link->getHref();
  if(href.empty())
    return QIcon();

  QImage img;
  if(!img.load(href.c_str()))
    return QIcon();

  return QIcon(QPixmap::fromImage(img.scaledToWidth(size.width(),Qt::SmoothTransformation)));
}

void te::qt::widgets::LocalGraphicWidget::onExternalGraphicChanged(const QSize& size)
{
  m_graphic->setExternalGraphic(0, m_localImageWidget->getExternalGraphic());
  // Updating graphic size
  m_ui->m_graphicSizeDoubleSpinBox->setValue(size.height()); // It will emit graphicChanged() signal;
}
