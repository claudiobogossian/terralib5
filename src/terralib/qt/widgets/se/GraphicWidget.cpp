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
  \file terralib/qt/widgets/se/GraphicWidget.cpp

  \brief A base widget used to configure a graphic.
*/

// TerraLib
#include "../../../maptools/Utils.h"
#include "../../../se/AnchorPoint.h"
#include "../../../se/Displacement.h"
#include "../../../se/ParameterValue.h"
#include "../../../se/Graphic.h"
#include "GraphicWidget.h"
#include "ui_GraphicWidgetForm.h"

// STL
#include <cassert>

te::qt::widgets::GraphicWidget::GraphicWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::GraphicWidgetForm),
    m_graphic(new te::se::Graphic)
{
  m_ui->setupUi(this);

  // Signals & slots
  connect(m_ui->m_graphicSizeDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicSizeDoubleSpinBoxValueChanged(const QString&)));
  connect(m_ui->m_graphicAngleDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicAngleDoubleSpinBoxValueChanged(const QString&)));
  connect(m_ui->m_graphicOpacitySlider, SIGNAL(valueChanged(int)), SLOT(onGraphicOpacitySliderValueChanged(int)));
  connect(m_ui->m_displacementXDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicDisplacementChanged(const QString&)));
  connect(m_ui->m_displacementYDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicDisplacementChanged(const QString&)));
  connect(m_ui->m_anchorPointXDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicAnchorPointChanged(const QString&)));
  connect(m_ui->m_anchorPointYDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicAnchorPointChanged(const QString&)));
}

te::qt::widgets::GraphicWidget::~GraphicWidget()
{
  delete m_graphic;
}

bool te::qt::widgets::GraphicWidget::setGraphic(const te::se::Graphic* graphic)
{
  assert(graphic);

  delete m_graphic;

  m_graphic = graphic->clone();

  updateUi();

  return true;
}

void te::qt::widgets::GraphicWidget::updateUi()
{
  // Size
  const te::se::ParameterValue* size = m_graphic->getSize();
  if(size)
    m_ui->m_graphicSizeDoubleSpinBox->setValue(te::map::GetDouble(size));

  // Rotation
  const te::se::ParameterValue* rotation = m_graphic->getRotation();
  if(rotation)
    m_ui->m_graphicAngleDoubleSpinBox->setValue(te::map::GetDouble(rotation));

  // Opacity
  const te::se::ParameterValue* opacity = m_graphic->getOpacity();
  if(opacity)
    m_ui->m_graphicOpacitySlider->setValue(te::map::GetDouble(opacity) * 100);

  // Displacement
  const te::se::Displacement* disp = m_graphic->getDisplacement();
  if(disp)
  {
    const te::se::ParameterValue* dispx = disp->getDisplacementX();
    if(dispx)
      m_ui->m_displacementXDoubleSpinBox->setValue(te::map::GetDouble(dispx));
    
    const te::se::ParameterValue* dispy = disp->getDisplacementY();
    if(dispy)
      m_ui->m_displacementYDoubleSpinBox->setValue(te::map::GetDouble(dispy));
  }

  // Anchor Point
  const te::se::AnchorPoint* ac = m_graphic->getAnchorPoint();
  if(ac)
  {
    const te::se::ParameterValue* acx = ac->getAnchorPointX();
    if(acx)
      m_ui->m_anchorPointXDoubleSpinBox->setValue(te::map::GetDouble(acx));

    const te::se::ParameterValue* acy = ac->getAnchorPointY();
    if(acy)
      m_ui->m_anchorPointYDoubleSpinBox->setValue(te::map::GetDouble(acy));
  }
}

te::se::Graphic* te::qt::widgets::GraphicWidget::getGraphic() const
{
  return m_graphic->clone();
}

void te::qt::widgets::GraphicWidget::onGraphicSizeDoubleSpinBoxValueChanged(const QString& text)
{
  m_graphic->setSize(new te::se::ParameterValue(text.toStdString()));
  emit graphicChanged();
}

void te::qt::widgets::GraphicWidget::onGraphicAngleDoubleSpinBoxValueChanged(const QString& text)
{
  m_graphic->setRotation(new te::se::ParameterValue(text.toStdString()));
  emit graphicChanged();
}

void te::qt::widgets::GraphicWidget::onGraphicOpacitySliderValueChanged(int value)
{
  double opacity = value / 100.0;
  m_graphic->setOpacity(new te::se::ParameterValue(QString::number(opacity, 'g', 2).toStdString()));
  emit graphicChanged();
}

void te::qt::widgets::GraphicWidget::onGraphicDisplacementChanged(const QString& /*text*/)
{
  te::se::Displacement* disp = new te::se::Displacement;
  disp->setDisplacementX(new te::se::ParameterValue(m_ui->m_displacementXDoubleSpinBox->text().toStdString()));
  disp->setDisplacementY(new te::se::ParameterValue(m_ui->m_displacementYDoubleSpinBox->text().toStdString()));
  m_graphic->setDisplacement(disp);
  emit graphicChanged();
}

void te::qt::widgets::GraphicWidget::onGraphicAnchorPointChanged(const QString& /*text*/)
{
  te::se::AnchorPoint* ac = new te::se::AnchorPoint;
  ac->setAnchorPointX(new te::se::ParameterValue(m_ui->m_anchorPointXDoubleSpinBox->text().toStdString()));
  ac->setAnchorPointY(new te::se::ParameterValue(m_ui->m_anchorPointYDoubleSpinBox->text().toStdString()));
  m_graphic->setAnchorPoint(ac);
  emit graphicChanged();
}
