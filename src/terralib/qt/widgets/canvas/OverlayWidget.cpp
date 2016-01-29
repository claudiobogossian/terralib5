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
  \file terralib/qt/widgets/canvas/OverlayWidget.cpp

  \brief This file has the OverlayWidget class.
*/

// TerraLib
#include "../../../maptools/Utils.h"
#include "../../widgets/tools/Pan.h"
#include "../../widgets/tools/ZoomWheel.h"
#include "../canvas/Canvas.h"
#include "../canvas/MultiThreadMapDisplay.h"
#include "OverlayWidget.h"
#include "ui_OverlayWidgetForm.h"

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::OverlayWidget::OverlayWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::OverlayWidgetForm),
    m_panTool(0), m_zoomTool(0),
    m_draft(0), m_draftOriginal(0)
{
  m_ui->setupUi(this);

//setup form
  m_ui->m_applyToolButton->setIcon(QIcon::fromTheme("check"));
  connect(m_ui->m_applyToolButton, SIGNAL(clicked()), this, SLOT(onApplyToolButtonClicked()));
  connect(m_ui->m_opacityHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onOpacityChanged(int)));
  connect(m_ui->m_verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(onVSliderChanged(int)));
  connect(m_ui->m_horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onHSliderChanged(int)));
  connect(m_ui->m_hideToolButton, SIGNAL(toggled(bool)), this, SLOT(onHideToolButtonClicked(bool)));

//build map display
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(m_ui->m_widget->size(), m_ui->m_widget);
  displayLayout->addWidget(m_mapDisplay);
  displayLayout->setContentsMargins(0,0,0,0);

  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(onMapDisplayExtentChanged()));

//start tools
  m_panTool = new te::qt::widgets::Pan(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor);
  m_zoomTool = new te::qt::widgets::ZoomWheel(m_mapDisplay, 1.5);

  m_mapDisplay->installEventFilter(m_panTool);
  m_mapDisplay->installEventFilter(m_zoomTool);
}

te::qt::widgets::OverlayWidget::~OverlayWidget()
{
  delete m_panTool;
  delete m_zoomTool;
  delete m_draftOriginal;
}

Ui::OverlayWidgetForm* te::qt::widgets::OverlayWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::OverlayWidget::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
   std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    m_ui->m_upperComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
    m_ui->m_lowerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

void te::qt::widgets::OverlayWidget::onApplyToolButtonClicked()
{
  //get lower layer
  int lowerIdx = m_ui->m_lowerComboBox->currentIndex();
  QVariant lowerV = m_ui->m_lowerComboBox->itemData(lowerIdx, Qt::UserRole);
  te::map::AbstractLayerPtr lowerLayer = lowerV.value<te::map::AbstractLayerPtr>();

  //draw lower layer
  std::list<te::map::AbstractLayerPtr> list;
  list.push_back(lowerLayer);

  te::gm::Envelope e = lowerLayer->getExtent();

  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->setLayerList(list);
  m_mapDisplay->setSRID(lowerLayer->getSRID(), false);
  m_mapDisplay->setExtent(e, true);

  //draw upper layer
  int upperIdx = m_ui->m_upperComboBox->currentIndex();
  QVariant upperV = m_ui->m_upperComboBox->itemData(upperIdx, Qt::UserRole);
  te::map::AbstractLayerPtr upperLayer = upperV.value<te::map::AbstractLayerPtr>();

  const te::gm::Envelope& env = m_mapDisplay->getExtent();

  m_draft = m_mapDisplay->getDraftPixmap();
  m_draft->fill(Qt::transparent);

  // Prepares the canvas
  Canvas canvas(m_mapDisplay->width(), m_mapDisplay->height());
  canvas.setDevice(m_draft, false);
  canvas.setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

  upperLayer->draw(&canvas, env, m_mapDisplay->getSRID(), m_mapDisplay->getScale());

  m_mapDisplay->repaint();
}

void te::qt::widgets::OverlayWidget::onMapDisplayExtentChanged()
{
  //draw upper layer
  int upperIdx = m_ui->m_upperComboBox->currentIndex();
  QVariant upperV = m_ui->m_upperComboBox->itemData(upperIdx, Qt::UserRole);
  te::map::AbstractLayerPtr upperLayer = upperV.value<te::map::AbstractLayerPtr>();

  const te::gm::Envelope& env = m_mapDisplay->getExtent();

  m_draft = m_mapDisplay->getDraftPixmap();
  m_draft->fill(Qt::transparent);

  // Prepares the canvas
  {
    Canvas canvas(m_mapDisplay->width(), m_mapDisplay->height());
    canvas.setDevice(m_draft, false);
    canvas.setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

    upperLayer->draw(&canvas, env, m_mapDisplay->getSRID(), m_mapDisplay->getScale());
  }

  m_draftOriginal = new QPixmap(*m_mapDisplay->getDraftPixmap());

  updateSliderBarSize();

  m_mapDisplay->repaint();
}

void te::qt::widgets::OverlayWidget::onOpacityChanged(int value)
{
  drawUpperLayer();
}

void te::qt::widgets::OverlayWidget::onHideToolButtonClicked(bool flag)
{
  if(flag)
  {
    m_draft->fill(Qt::transparent);
    m_mapDisplay->repaint();
  }
  else
  {
    drawUpperLayer();
  }
}

void te::qt::widgets::OverlayWidget::drawUpperLayer()
{
  if(m_ui->m_hideToolButton->isChecked())
    return;

  QImage img = m_draftOriginal->toImage();

  m_draft->fill(Qt::transparent);

  QPainter p(m_draft);
  p.setOpacity(m_ui->m_opacityHorizontalSlider->value() / 255.);
  p.drawImage(0, 0, img, 0, 0, m_ui->m_horizontalSlider->value(), m_ui->m_verticalSlider->value());
  p.end();

  m_mapDisplay->repaint();
}

void te::qt::widgets::OverlayWidget::updateSliderBarSize()
{
  m_ui->m_horizontalSlider->setMaximum(m_mapDisplay->getWidth() - 1);
  m_ui->m_horizontalSlider->setValue(m_mapDisplay->getWidth() - 1);
  m_ui->m_verticalSlider->setMaximum(m_mapDisplay->getHeight() - 1);
  m_ui->m_verticalSlider->setValue(m_mapDisplay->getHeight() - 1);
}

void te::qt::widgets::OverlayWidget::onVSliderChanged(int value)
{
  drawUpperLayer();
}

void te::qt::widgets::OverlayWidget::onHSliderChanged(int value)
{
  drawUpperLayer();
}
