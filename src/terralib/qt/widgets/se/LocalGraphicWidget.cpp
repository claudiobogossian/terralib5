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

  \brief A widget used to build a graphic associated with an external graphic element that references a local image. e.g. a SVG file, a PNG file, etc.
*/

// TerraLib
#include "../../../se/ExternalGraphic.h"
#include "../../../se/AnchorPoint.h"
#include "../../../se/Displacement.h"
#include "../../../se/Graphic.h"
#include "../../../se/Utils.h"
#include "../../../xlink/SimpleLink.h"
#include "LocalGraphicWidget.h"
#include "LocalImageWidget.h"
#include "ui_LocalGraphicWidgetForm.h"

// STL
#include <cassert>

te::qt::widgets::LocalGraphicWidget::LocalGraphicWidget(QWidget* parent, Qt::WindowFlags f)
  : AbstractGraphicWidget(parent, f),
    m_ui(new Ui::LocalGraphicWidgetForm)
{
  m_ui->setupUi(this);

  // Local Image Widget
  m_localImageWidget = new te::qt::widgets::LocalImageWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_imageGroupBox);
  layout->addWidget(m_localImageWidget);

  // Setups initial graphic
  m_graphic->add(m_localImageWidget->getExternalGraphic());

  // Signals & slots
  connect(m_ui->m_graphicSizeDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(setGraphicSize(const QString&)));
  connect(m_ui->m_graphicAngleDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(setGraphicAngle(const QString&)));
  connect(m_ui->m_graphicOpacitySlider, SIGNAL(valueChanged(int)), SLOT(setGraphicOpacity(int)));
  connect(m_ui->m_displacementXDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicDisplacementChanged(const QString&)));
  connect(m_ui->m_displacementYDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicDisplacementChanged(const QString&)));
  connect(m_ui->m_anchorPointXDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicAnchorPointChanged(const QString&)));
  connect(m_ui->m_anchorPointYDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicAnchorPointChanged(const QString&)));
  connect(m_localImageWidget, SIGNAL(externalGraphicChanged(const QSize&)), SLOT(onExternalGraphicChanged(const QSize&)));
}

te::qt::widgets::LocalGraphicWidget::~LocalGraphicWidget()
{
}

bool te::qt::widgets::LocalGraphicWidget::setGraphic(const te::se::Graphic* graphic)
{
  assert(graphic);

  delete m_graphic;

  m_graphic = graphic->clone();

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

  updateUi();

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

  return QIcon(QPixmap::fromImage(img.scaledToWidth(size.width(), Qt::SmoothTransformation)));
}

void te::qt::widgets::LocalGraphicWidget::onExternalGraphicChanged(const QSize& size)
{
  m_graphic->setExternalGraphic(0, m_localImageWidget->getExternalGraphic());
  // Updating graphic size
  m_ui->m_graphicSizeDoubleSpinBox->setValue(size.height());
}

void te::qt::widgets::LocalGraphicWidget::onGraphicDisplacementChanged(const QString& /*text*/)
{
  setGraphicDisplacement(m_ui->m_displacementXDoubleSpinBox->text(), m_ui->m_displacementXDoubleSpinBox->text());
}

void te::qt::widgets::LocalGraphicWidget::onGraphicAnchorPointChanged(const QString& /*text*/)
{
  setGraphicAnchorPoint(m_ui->m_anchorPointXDoubleSpinBox->text(), m_ui->m_anchorPointYDoubleSpinBox->text());
}

void te::qt::widgets::LocalGraphicWidget::updateUi()
{
  // Size
  const te::se::ParameterValue* size = m_graphic->getSize();
  if(size)
    m_ui->m_graphicSizeDoubleSpinBox->setValue(te::se::GetDouble(size));

  // Rotation
  const te::se::ParameterValue* rotation = m_graphic->getRotation();
  if(rotation)
    m_ui->m_graphicAngleDoubleSpinBox->setValue(te::se::GetDouble(rotation));

  // Opacity
  const te::se::ParameterValue* opacity = m_graphic->getOpacity();
  if(opacity)
    m_ui->m_graphicOpacitySlider->setValue(te::se::GetDouble(opacity) * 100);

  // Displacement
  const te::se::Displacement* disp = m_graphic->getDisplacement();
  if(disp)
  {
    const te::se::ParameterValue* dispx = disp->getDisplacementX();
    if(dispx)
      m_ui->m_displacementXDoubleSpinBox->setValue(te::se::GetDouble(dispx));
    
    const te::se::ParameterValue* dispy = disp->getDisplacementY();
    if(dispy)
      m_ui->m_displacementYDoubleSpinBox->setValue(te::se::GetDouble(dispy));
  }

  // Anchor Point
  const te::se::AnchorPoint* ac = m_graphic->getAnchorPoint();
  if(ac)
  {
    const te::se::ParameterValue* acx = ac->getAnchorPointX();
    if(acx)
      m_ui->m_anchorPointXDoubleSpinBox->setValue(te::se::GetDouble(acx));

    const te::se::ParameterValue* acy = ac->getAnchorPointY();
    if(acy)
      m_ui->m_anchorPointYDoubleSpinBox->setValue(te::se::GetDouble(acy));
  }
}
