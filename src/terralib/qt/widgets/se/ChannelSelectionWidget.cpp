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
  \file terralib/qt/widgets/se/ChannelSelectionWidget.cpp

  \brief A dialog used to build a ChannelSelection element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "ChannelSelectionWidget.h"
#include "SelectedChannelWidget.h"
#include "ui_ChannelSelectionWidgetForm.h"
#include "../../../raster.h"
#include "../../../se.h"

// Qt


// STL
#include <cassert>

te::qt::widgets::ChannelSelectionWidget::ChannelSelectionWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ChannelSelectionWidgetForm),
    m_scRed(new te::se::SelectedChannel),
    m_scGreen(new te::se::SelectedChannel),
    m_scBlue(new te::se::SelectedChannel),
    m_scMono(new te::se::SelectedChannel),
    m_cs(new te::se::ChannelSelection)
{
  m_ui->setupUi(this);

  //Selected Channel Widget
  m_sCWidget = new te::qt::widgets::SelectedChannelWidget(this);
  
  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_frame);
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(m_sCWidget);
  m_sCWidget->show();

  // Signals & slots
  connect(m_ui->m_rgbColorRadioButton, SIGNAL(clicked()), SLOT(onRGBColorSelected()));
  connect(m_ui->m_grayScaleRadioButton, SIGNAL(clicked()), SLOT(onGrayScalelSelected()));

  connect(m_ui->m_redRadioButton, SIGNAL(clicked()), SLOT(onRedChannelSelected()));
  connect(m_ui->m_greenRadioButton, SIGNAL(clicked()), SLOT(onGreenChannelSelected()));
  connect(m_ui->m_blueRadioButton, SIGNAL(clicked()), SLOT(onBlueChannelSelected()));
  connect(m_ui->m_monoRadioButton, SIGNAL(clicked()), SLOT(onGrayScalelSelected()));
   

  connect(m_sCWidget, SIGNAL(selectedChannelChanged()), SLOT(onSelectedChannelChanged()));

  initialize();
}

te::qt::widgets::ChannelSelectionWidget::~ChannelSelectionWidget()
{
  delete m_cs;
}

void te::qt::widgets::ChannelSelectionWidget::setChannelSelection(const te::se::ChannelSelection* cs)
{
  assert(cs);

  delete m_cs;

  m_cs = cs->clone();
}

te::se::ChannelSelection* te::qt::widgets::ChannelSelectionWidget::getChannelSelection() const
{
  return m_cs->clone();
}

void te::qt::widgets::ChannelSelectionWidget::setProperty(std::vector<te::rst::BandProperty*>& p)
{
  m_bands = p;

  updateUi();
}

void te::qt::widgets::ChannelSelectionWidget::initialize()
{
  m_cs->setRedChannel(m_scRed);
  m_cs->setGreenChannel(m_scGreen);
  m_cs->setBlueChannel(m_scBlue);
  m_cs->setGrayChannel(m_scMono);
}

void te::qt::widgets::ChannelSelectionWidget::updateUi()
{
  QStringList bandNames;

  for(size_t i = 0; i < m_bands.size(); ++i)
  {
    // if the band property does not have the description information
    // we must used the index information.
    if(m_bands[i]->m_description.empty())
    {
      QString bandInfo;
      bandInfo.setNum(m_bands[i]->m_idx);

      bandNames.push_back(bandInfo);
    }
    else
    {
      bandNames.push_back(m_bands[i]->m_description.c_str());
    }
  }

  if(m_sCWidget)
  {
    m_sCWidget->setChannelNames(bandNames);
  }
}

void te::qt::widgets::ChannelSelectionWidget::onRGBColorSelected()
{
  m_ui->m_redRadioButton->setEnabled(true);
  m_ui->m_greenRadioButton->setEnabled(true);
  m_ui->m_blueRadioButton->setEnabled(true);
  m_ui->m_monoRadioButton->setEnabled(false);

  m_ui->m_redRadioButton->setChecked(true);
  onRedChannelSelected();
}

void te::qt::widgets::ChannelSelectionWidget::onGrayScalelSelected()
{
  m_ui->m_redRadioButton->setEnabled(false);
  m_ui->m_greenRadioButton->setEnabled(false);
  m_ui->m_blueRadioButton->setEnabled(false);
  m_ui->m_monoRadioButton->setEnabled(true);

  m_ui->m_monoRadioButton->setChecked(true);


  m_sCWidget->setSelectedChannel(m_scMono);
}

void te::qt::widgets::ChannelSelectionWidget::onRedChannelSelected()
{
  if(m_ui->m_redRadioButton->isChecked())
  {
    m_sCWidget->setSelectedChannel(m_scRed);
  }
}

void te::qt::widgets::ChannelSelectionWidget::onGreenChannelSelected()
{
  if(m_ui->m_greenRadioButton->isChecked())
  {
    m_sCWidget->setSelectedChannel(m_scGreen);
  }
}

void te::qt::widgets::ChannelSelectionWidget::onBlueChannelSelected()
{
  if(m_ui->m_blueRadioButton->isChecked())
  {
    m_sCWidget->setSelectedChannel(m_scBlue);
  }
}
void te::qt::widgets::ChannelSelectionWidget::onMonoChannelSelected()
{
  if(m_ui->m_monoRadioButton->isChecked())
  {
    m_sCWidget->setSelectedChannel(m_scMono);
  }
}

void te::qt::widgets::ChannelSelectionWidget::onSelectedChannelChanged()
{
  if(m_ui->m_redRadioButton->isChecked())
  {
    m_scRed = m_sCWidget->getSelectedChannel();
  }
  else if(m_ui->m_greenRadioButton->isChecked())
  {
    m_scGreen = m_sCWidget->getSelectedChannel();
  }
  else if(m_ui->m_blueRadioButton->isChecked())
  {
    m_scBlue = m_sCWidget->getSelectedChannel();
  }
  else if(m_ui->m_monoRadioButton->isChecked())
  {
    m_scMono = m_sCWidget->getSelectedChannel();
  }
}
