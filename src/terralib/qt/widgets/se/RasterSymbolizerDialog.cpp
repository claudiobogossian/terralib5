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
#include "../../../common/STLUtils.h"
#include "RasterSymbolizerDialog.h"
#include "ChannelSelectionWidget.h"
#include "OverlapBehaviorWidget.h"
#include "ContrastEnhancementWidget.h"
#include "ShadedReliefWidget.h"
#include "ImageOutlineWidget.h"
#include "ui_RasterSymbolizerDialogForm.h"

// Qt


// STL


te::qt::widgets::RasterSymbolizerDialog::RasterSymbolizerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::RasterSymbolizerDialogForm),
    m_channelSelectionWidget(0),
    m_overlapBehaviorWidget(0),
    m_contrastWidget(0),
    m_shadedReliefWidget(0),
    m_imageOutlineWidget(0)
{
  m_ui->setupUi(this);

  //connect tool buttons with slot functions
  connect(m_ui->m_channelSelectionToolButton, SIGNAL(clicked()), this, SLOT(onChannelSelectionClicked()));
  connect(m_ui->m_contrastToolButton, SIGNAL(clicked()), this, SLOT(onContrastEnhancementClicked()));
  connect(m_ui->m_imageOutlinetoolButton, SIGNAL(clicked()), this, SLOT(onImageOutlineClicked()));
  connect(m_ui->m_overlapBehaviorToolButton, SIGNAL(clicked()), this, SLOT(onOverlapBehaviorClicked()));
  connect(m_ui->m_shadedReliefToolButton, SIGNAL(clicked()), this, SLOT(onShadedReliefClicked()));
  
  m_ui->m_stackedWidget->setCurrentIndex(0);
}

te::qt::widgets::RasterSymbolizerDialog::~RasterSymbolizerDialog()
{

}

void te::qt::widgets::RasterSymbolizerDialog::updateUi()
{

}

void te::qt::widgets::RasterSymbolizerDialog::onChannelSelectionClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(1);

  if(!m_channelSelectionWidget)
  {
    m_channelSelectionWidget = new te::qt::widgets::ChannelSelectionWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_channelSelectionWidget);
  }
}

void te::qt::widgets::RasterSymbolizerDialog::onContrastEnhancementClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(2);

  if(!m_contrastWidget)
  {
    m_contrastWidget = new te::qt::widgets::ContrastEnhancementWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_contrastWidget);
  }

  this->adjustSize();
}

void te::qt::widgets::RasterSymbolizerDialog::onImageOutlineClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(3);

  if(!m_imageOutlineWidget)
  {
    m_imageOutlineWidget = new te::qt::widgets::ImageOutlineWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_imageOutlineWidget);
  }

  this->adjustSize();
}

void te::qt::widgets::RasterSymbolizerDialog::onOverlapBehaviorClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(4);

  if(!m_overlapBehaviorWidget)
  {
    m_overlapBehaviorWidget = new te::qt::widgets::OverlapBehaviorWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_overlapBehaviorWidget);
  }

  this->adjustSize();
}

void te::qt::widgets::RasterSymbolizerDialog::onShadedReliefClicked()
{
  m_ui->m_stackedWidget->setCurrentIndex(5);

  if(!m_shadedReliefWidget)
  {
    m_shadedReliefWidget = new te::qt::widgets::ShadedReliefWidget(m_ui->m_stackedWidget->currentWidget());

    QGridLayout* layout = new QGridLayout(m_ui->m_stackedWidget->currentWidget());

    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_shadedReliefWidget);
  }

  this->adjustSize();
}
