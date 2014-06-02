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
  \file terralib/qt/widgets/se/RasterSymbolizerDialog.cpp

  \brief A dialog used to build a RasterSymbolizer element.
*/

// TerraLib
#include "../../../common.h"
#include "../../../raster.h"
#include "../../../se.h"
#include "../utils/HorizontalSliderWidget.h"
#include "ChannelSelectionWidget.h"
#include "ColorMapWidget.h"
#include "ContrastEnhancementWidget.h"
#include "ImageOutlineWidget.h"
#include "OverlapBehaviorWidget.h"
#include "RasterSymbolizerDialog.h"
#include "RasterSymbolizerWidget.h"
#include "ShadedReliefWidget.h"
#include "ui_RasterSymbolizerDialogForm.h"

te::qt::widgets::RasterSymbolizerDialog::RasterSymbolizerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::RasterSymbolizerDialogForm),
    m_channelSelectionWidget(0),
    m_colorMapWidget(0),
    m_overlapBehaviorWidget(0),
    m_contrastWidget(0),
    m_shadedReliefWidget(0),
    m_imageOutlineWidget(0),
    m_sliderWidget(0),
    m_rasterWidget(0),
    m_symbolizer(new te::se::RasterSymbolizer),
    m_colorMap(0),
    m_property(0),
    m_raster(0)
   
{
  m_ui->setupUi(this);

  //connect tool buttons with slot functions
  
  //connect(m_ui->m_opacityToolButton, SIGNAL(clicked()), this, SLOT(onOpacityClicked()));
  //connect(m_ui->m_channelSelectionToolButton, SIGNAL(clicked()), this, SLOT(onChannelSelectionClicked()));
  connect(m_ui->m_colorMapToolButton, SIGNAL(clicked()), this, SLOT(onColorMapClicked()));
  //connect(m_ui->m_contrastToolButton, SIGNAL(clicked()), this, SLOT(onContrastEnhancementClicked()));
  connect(m_ui->m_imageOutlinetoolButton, SIGNAL(clicked()), this, SLOT(onImageOutlineClicked()));
  connect(m_ui->m_overlapBehaviorToolButton, SIGNAL(clicked()), this, SLOT(onOverlapBehaviorClicked()));
  connect(m_ui->m_shadedReliefToolButton, SIGNAL(clicked()), this, SLOT(onShadedReliefClicked()));
  connect(m_ui->m_visualToolButton, SIGNAL(clicked()), this, SLOT(onVisualClicked()));
  
  m_ui->m_stackedWidget->setCurrentIndex(0);
}

te::qt::widgets::RasterSymbolizerDialog::~RasterSymbolizerDialog()
{
  delete m_symbolizer;
}

void te::qt::widgets::RasterSymbolizerDialog::setRasterProperty(te::rst::Raster* r, te::rst::RasterProperty* p)
{
  assert(r);
  m_raster = r;

  assert(p);
  m_property = p;

  updateUi();
}

void te::qt::widgets::RasterSymbolizerDialog::setRasterSymbolizer(const te::se::RasterSymbolizer* rs)
{
  assert(rs);

  delete m_symbolizer;

  m_symbolizer = static_cast<te::se::RasterSymbolizer*>(rs->clone());
}

te::se::Symbolizer* te::qt::widgets::RasterSymbolizerDialog::getRasterSymbolizer() const
{
  if(m_sliderWidget)
  {
    // if opacity value is betwen 0 and 1. The value used from slider must be
    // changed to this range.
    int opacity = m_sliderWidget->getValue();
    double seOpacity = opacity / 100.;
    QString qStrOpacity;
    qStrOpacity.setNum(seOpacity);

    m_symbolizer->setOpacity(new te::se::ParameterValue(qStrOpacity.toStdString()));
  }

  if(m_channelSelectionWidget)
  {
    m_symbolizer->setChannelSelection(m_channelSelectionWidget->getChannelSelection());
  }

  if(m_contrastWidget)
  {
    m_symbolizer->setContrastEnhancement(m_contrastWidget->getContrastEnhancement());
  }

  if(m_imageOutlineWidget)
  {
    m_symbolizer->setImageOutline(m_imageOutlineWidget->getImageOutline());
  }

  if(m_overlapBehaviorWidget)
  {
    m_symbolizer->setOverlapBehavior(m_overlapBehaviorWidget->getOverlapBehavior());
  }

  if(m_shadedReliefWidget)
  {
    m_symbolizer->setShadedRelief(m_shadedReliefWidget->getShadedRelief());
  }

  if(m_colorMapWidget)
  {
    m_symbolizer->setColorMap(m_colorMap);
  }

  if(m_rasterWidget)
  {
    te::se::RasterSymbolizer* rs = m_rasterWidget->getRasterSymbolizer();

    if(rs)
    {
      m_symbolizer->setGain(rs->getGain()->clone());
      m_symbolizer->setOffset(rs->getOffset()->clone());
      m_symbolizer->setOpacity(rs->getOpacity()->clone());
      m_symbolizer->setChannelSelection(rs->getChannelSelection()->clone());

      delete rs;
    }
  }

  return m_symbolizer->clone();
}

void te::qt::widgets::RasterSymbolizerDialog::updateUi()
{
  if(m_property)
  {
    if(m_channelSelectionWidget)
    {
      m_channelSelectionWidget->setProperty(m_property->getBandProperties());
    }
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onOpacityClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(1);

  if(!m_sliderWidget)
  {
    m_sliderWidget = new te::qt::widgets::HorizontalSliderWidget(m_ui->m_stackedWidget->currentWidget());

    m_sliderWidget->setTitle(tr("Opacity"));
    m_sliderWidget->setMinMaxValues(0, 100);
    m_sliderWidget->setDefaultValue(100);

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_sliderWidget);
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onChannelSelectionClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(2);

  if(!m_channelSelectionWidget)
  {
    m_channelSelectionWidget = new te::qt::widgets::ChannelSelectionWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_channelSelectionWidget);

    if(m_property)
    {
      m_channelSelectionWidget->setProperty(m_property->getBandProperties());
    }
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onColorMapClicked()
{
    m_ui->m_stackedWidget->setCurrentIndex(3);

  if(!m_colorMapWidget)
  {
    m_colorMapWidget = new te::qt::widgets::ColorMapWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_colorMapWidget);

    m_colorMap = new te::se::ColorMap();
    m_colorMapWidget->setColorMap(m_colorMap);

    if(m_raster)
    {
      m_colorMapWidget->setRaster(m_raster);
    }
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onContrastEnhancementClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(4);

  if(!m_contrastWidget)
  {
    m_contrastWidget = new te::qt::widgets::ContrastEnhancementWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_contrastWidget);
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onImageOutlineClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(5);

  if(!m_imageOutlineWidget)
  {
    m_imageOutlineWidget = new te::qt::widgets::ImageOutlineWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_imageOutlineWidget);

    if(m_symbolizer->getImageOutline())
    {
      m_imageOutlineWidget->setImageOutline(m_symbolizer->getImageOutline());
    }
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onOverlapBehaviorClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(6);

  if(!m_overlapBehaviorWidget)
  {
    m_overlapBehaviorWidget = new te::qt::widgets::OverlapBehaviorWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_overlapBehaviorWidget);
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onShadedReliefClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(7);

  if(!m_shadedReliefWidget)
  {
    m_shadedReliefWidget = new te::qt::widgets::ShadedReliefWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_shadedReliefWidget);
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onVisualClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(1);

  if(!m_rasterWidget)
  {
    m_rasterWidget = new te::qt::widgets::RasterSymbolizerWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_rasterWidget);

    if(m_property)
    {
      m_rasterWidget->setBandProperty(m_property->getBandProperties());
    }

    m_rasterWidget->setRasterSymbolizer(m_symbolizer);
  }
}
