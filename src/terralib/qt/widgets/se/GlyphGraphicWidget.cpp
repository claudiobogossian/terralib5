/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/GlyphGraphicWidget.cpp

  \brief A widget used to build a graphic associate with a glyph mark element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../se/AnchorPoint.h"
#include "../../../se/Displacement.h"
#include "../../../se/Graphic.h"
#include "../../../se/Mark.h"
#include "../../../se/Utils.h"
#include "../Utils.h"
#include "GlyphGraphicWidget.h"
#include "GlyphMarkWidget.h"
#include "ui_GlyphGraphicWidgetForm.h"

// STL
#include <cassert>

te::qt::widgets::GlyphGraphicWidget::GlyphGraphicWidget(QWidget* parent, Qt::WindowFlags f)
  : AbstractGraphicWidget(parent, f),
    m_ui(new Ui::GlyphGraphicWidgetForm)
{
  m_ui->setupUi(this);

  // Glyph Mark Widget
  m_glyphMarkWidget = new te::qt::widgets::GlyphMarkWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_glyphGroupBox);
  layout->addWidget(m_glyphMarkWidget);

  // Setups initial graphic
  m_graphic->add(m_glyphMarkWidget->getMark());

  // Signals & slots
  connect(m_ui->m_graphicSizeDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(setGraphicSize(const QString&)));
  connect(m_ui->m_graphicAngleDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(setGraphicAngle(const QString&)));
  connect(m_ui->m_displacementXDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicDisplacementChanged(const QString&)));
  connect(m_ui->m_displacementYDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicDisplacementChanged(const QString&)));
  connect(m_ui->m_anchorPointXDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicAnchorPointChanged(const QString&)));
  connect(m_ui->m_anchorPointYDoubleSpinBox, SIGNAL(valueChanged(const QString&)), SLOT(onGraphicAnchorPointChanged(const QString&)));
  connect(m_glyphMarkWidget, SIGNAL(markChanged()), SLOT(onMarkChanged()));
}

te::qt::widgets::GlyphGraphicWidget::~GlyphGraphicWidget()
{
}

bool te::qt::widgets::GlyphGraphicWidget::setGraphic(const te::se::Graphic* graphic)
{
  assert(graphic);

  delete m_graphic;

  m_graphic = graphic->clone();

  // Verifying if this widget can deal with the given graphic...
  const std::vector<te::se::Mark*> marks = m_graphic->getMarks();
  if(marks.empty())
    return false;

  te::se::Mark* mark = marks[0];
  const std::string* name = mark->getWellKnownName();
  if(name == 0)
    return false;

  std::size_t found = name->find("ttf://");
  if(found == std::string::npos)
    return false;

  // I know it!
  m_glyphMarkWidget->setMark(marks[0]);

  updateUi();

  return true;
}

QString te::qt::widgets::GlyphGraphicWidget::getGraphicType() const
{
  return tr("Glyph Marker");
}

QIcon te::qt::widgets::GlyphGraphicWidget::getGraphicIcon(const QSize& size) const
{
  te::se::Mark* mark = m_glyphMarkWidget->getMark();

  int dimension = size.width();
  te::color::RGBAColor** rgba = te::map::MarkRendererManager::getInstance().render(mark, dimension);
  QImage* img = te::qt::widgets::GetImage(rgba, dimension, dimension);

  QIcon icon = QIcon(QPixmap::fromImage(img->scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

  delete img;
  te::common::Free(rgba, dimension);
  delete mark;

  return icon;
}

void te::qt::widgets::GlyphGraphicWidget::onMarkChanged()
{
  m_graphic->setMark(0, m_glyphMarkWidget->getMark());
  emit graphicChanged();
}

void te::qt::widgets::GlyphGraphicWidget::onGraphicDisplacementChanged(const QString& /*text*/)
{
  setGraphicDisplacement(m_ui->m_displacementXDoubleSpinBox->text(), m_ui->m_displacementXDoubleSpinBox->text());
}

void te::qt::widgets::GlyphGraphicWidget::onGraphicAnchorPointChanged(const QString& /*text*/)
{
  setGraphicAnchorPoint(m_ui->m_anchorPointXDoubleSpinBox->text(), m_ui->m_anchorPointYDoubleSpinBox->text());
}

void te::qt::widgets::GlyphGraphicWidget::updateUi()
{
  // Size
  const te::se::ParameterValue* size = m_graphic->getSize();
  if(size)
    m_ui->m_graphicSizeDoubleSpinBox->setValue(te::se::GetDouble(size));

  // Rotation
  const te::se::ParameterValue* rotation = m_graphic->getRotation();
  if(rotation)
    m_ui->m_graphicAngleDoubleSpinBox->setValue(te::se::GetDouble(rotation));

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
