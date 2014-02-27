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
  \file terralib/qt/widgets/se/RasterSymbolizerWidget.cpp

  \brief A widget used to configure a Raster Symbolizer SE element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../raster.h"
#include "../../../se/Utils.h"
#include "HorizontalSliderWidget.h"
#include "RasterSymbolizerWidget.h"
#include "ui_RasterSymbolizerWidgetForm.h"

// Qt
#include <QLabel>

// STL
#include <cassert>

#define GAIN_CONSTANT_VALUE 0.2
#define OFFSET_CONSTANT_VALUE 10

te::qt::widgets::RasterSymbolizerWidget::RasterSymbolizerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::RasterSymbolizerWidgetForm),
    m_sliderWidget(0),
    m_symbolizer(0),
    m_contrastRed(0),
    m_contrastGreen(0),
    m_contrastBlue(0),
    m_contrastMono(0),
    m_scRed(0),
    m_scGreen(0),
    m_scBlue(0),
    m_scMono(0),
    m_cs(new te::se::ChannelSelection)
{
  m_ui->setupUi(this);

  // add opacity scrool bar
  m_sliderWidget = new te::qt::widgets::HorizontalSliderWidget(m_ui->m_opacityWidget);
  m_sliderWidget->setTitle(tr("Opacity"));
  m_sliderWidget->setMinMaxValues(0, 100);
  m_sliderWidget->setDefaultValue(100);

  QGridLayout* layout = new QGridLayout(m_ui->m_opacityWidget);
  layout->setContentsMargins(0,0,0,0);
  layout->setAlignment(Qt::AlignTop);
  layout->addWidget(m_sliderWidget);


  //connect slots
  connect(m_sliderWidget, SIGNAL(sliderValueChanged(int)), this, SLOT(onOpacityChanged(int)));
  connect(m_sliderWidget, SIGNAL(sliderReleased()), this, SLOT(onSymbolizerChanged()));

  connect(m_ui->m_composeMRadioButton, SIGNAL(clicked()), this, SLOT(onMonoChannelSelectionClicked()));
  connect(m_ui->m_composeRRadioButton, SIGNAL(clicked()), this, SLOT(onRedChannelSelectionClicked()));
  connect(m_ui->m_composeGRadioButton, SIGNAL(clicked()), this, SLOT(onGreenChannelSelectionClicked()));
  connect(m_ui->m_composeBRadioButton, SIGNAL(clicked()), this, SLOT(onBlueChannelSelectionClicked()));
  connect(m_ui->m_composeCRadioButton, SIGNAL(clicked()), this, SLOT(onCompositionChannelSelectionClicked()));

  connect(m_ui->m_composeMComboBox, SIGNAL(activated(QString)), this, SLOT(onMonoChannelNameChanged(QString)));
  connect(m_ui->m_composeRComboBox, SIGNAL(activated(QString)), this, SLOT(onRedChannelNameChanged(QString)));
  connect(m_ui->m_composeGComboBox, SIGNAL(activated(QString)), this, SLOT(onGreenChannelNameChanged(QString)));
  connect(m_ui->m_composeBComboBox, SIGNAL(activated(QString)), this, SLOT(onBlueChannelNameChanged(QString)));

  connect(m_ui->m_contrastTypeComboBox, SIGNAL(activated(QString)), this, SLOT(onTypeConstratChanged(QString)));

  connect(m_ui->m_contrastMHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onMonoGammaChanged(int)));
  connect(m_ui->m_contrastRHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onRedGammaChanged(int)));
  connect(m_ui->m_contrastGHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onGreenGammaChanged(int)));
  connect(m_ui->m_contrastBHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onBlueGammaChanged(int)));

  connect(m_ui->m_contrastMHorizontalSlider, SIGNAL(sliderReleased()), this, SLOT(onSymbolizerChanged()));
  connect(m_ui->m_contrastRHorizontalSlider, SIGNAL(sliderReleased()), this, SLOT(onSymbolizerChanged()));
  connect(m_ui->m_contrastGHorizontalSlider, SIGNAL(sliderReleased()), this, SLOT(onSymbolizerChanged()));
  connect(m_ui->m_contrastBHorizontalSlider, SIGNAL(sliderReleased()), this, SLOT(onSymbolizerChanged()));

  connect(m_ui->m_gainPlusPushButton, SIGNAL(clicked()), this, SLOT(onIncreaseGain()));
  connect(m_ui->m_gainMinusPushButton, SIGNAL(clicked()), this, SLOT(onDecreaseGain()));
  connect(m_ui->m_gainResetPushButton, SIGNAL(clicked()), this, SLOT(onDefaultGain()));
  connect(m_ui->m_offsetPlusPushButton, SIGNAL(clicked()), this, SLOT(onIncreaseOffset()));
  connect(m_ui->m_offsetMinusPushButton, SIGNAL(clicked()), this, SLOT(onDecreaseOffset()));
  connect(m_ui->m_offsetResetPushButton, SIGNAL(clicked()), this, SLOT(onDefaultOffset()));

  connect(m_ui->m_contrastGroupBox, SIGNAL(clicked()), this, SLOT(setContrastVisibility()));

  //init interface
  initialize();
}

te::qt::widgets::RasterSymbolizerWidget::~RasterSymbolizerWidget()
{
  m_ceNames.clear();
}

void  te::qt::widgets::RasterSymbolizerWidget::setRasterSymbolizer(te::se::RasterSymbolizer* rs)
{
  assert(rs);

  m_symbolizer = rs;

  if(m_symbolizer->getChannelSelection())
  {
    m_cs = m_symbolizer->getChannelSelection();

    m_scRed = m_cs->getRedChannel();
    m_scGreen = m_cs->getGreenChannel();
    m_scBlue = m_cs->getBlueChannel();
    m_scMono = m_cs->getGrayChannel();

    if(m_cs->getColorCompositionType() == te::se::RGB_COMPOSITION)
    {
      m_ui->m_composeCRadioButton->setChecked(true);

      onCompositionChannelSelectionClicked();
    }
    else if(m_cs->getColorCompositionType() == te::se::GRAY_COMPOSITION)
    {
      m_ui->m_composeMRadioButton->setChecked(true);

      onMonoChannelSelectionClicked();
    }
    else if(m_cs->getColorCompositionType() == te::se::RED_COMPOSITION)
    {
      m_ui->m_composeRRadioButton->setChecked(true);

      onRedChannelSelectionClicked();
    }
    else if(m_cs->getColorCompositionType() == te::se::GREEN_COMPOSITION)
    {
      m_ui->m_composeGRadioButton->setChecked(true);

      onGreenChannelSelectionClicked();
    }
    else if(m_cs->getColorCompositionType() == te::se::BLUE_COMPOSITION)
    {
      m_ui->m_composeBRadioButton->setChecked(true);

      onBlueChannelSelectionClicked();
    }

    m_contrastRed = 0;
    if(m_scRed && m_scRed->getContrastEnhancement())
    {
      m_contrastRed = m_scRed->getContrastEnhancement();
    }

    m_contrastGreen = 0;
    if(m_scGreen && m_scGreen->getContrastEnhancement())
    {
      m_contrastGreen = m_scGreen->getContrastEnhancement();
    }

    m_contrastBlue = 0;
    if(m_scBlue && m_scBlue->getContrastEnhancement())
    {
      m_contrastBlue = m_scBlue->getContrastEnhancement();
    }

    m_contrastMono = 0;
    if(m_scMono && m_scMono->getContrastEnhancement())
    {
      m_contrastMono = m_scMono->getContrastEnhancement();
    }
  }
  else
  {
    m_symbolizer->setChannelSelection(m_cs);
  }

  updateUi();
}

void te::qt::widgets::RasterSymbolizerWidget::setBandProperty(std::vector<te::rst::BandProperty*> bp)
{
  QStringList bandNames;

  for(size_t i = 0; i < bp.size(); ++i)
  {
    // if the band property does not have the description information
    // we must used the index information.
    if(bp[i]->m_description.empty())
    {
      QString bandInfo;
      bandInfo.setNum(bp[i]->m_idx);

      bandNames.push_back(bandInfo);
    }
    else
    {
      bandNames.push_back(bp[i]->m_description.c_str());
    }
  }

  m_ui->m_composeMComboBox->clear();
  m_ui->m_composeMComboBox->addItems(bandNames);

  m_ui->m_composeRComboBox->clear();
  m_ui->m_composeRComboBox->addItems(bandNames);

  m_ui->m_composeGComboBox->clear();
  m_ui->m_composeGComboBox->addItems(bandNames);

  m_ui->m_composeBComboBox->clear();
  m_ui->m_composeBComboBox->addItems(bandNames);
}

void te::qt::widgets::RasterSymbolizerWidget::initialize()
{
  //set the pixmaps
  m_ui->m_gainLabel->setPixmap(QIcon::fromTheme("gain").pixmap(16,16));
  m_ui->m_offSetLabel->setPixmap(QIcon::fromTheme("offset").pixmap(16,16));
  m_ui->m_gainPlusPushButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_gainPlusPushButton->setIconSize(QSize(16,16));
  m_ui->m_gainMinusPushButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_gainMinusPushButton->setIconSize(QSize(16,16));
  m_ui->m_gainResetPushButton->setIcon(QIcon::fromTheme("edit-undo"));
  m_ui->m_gainResetPushButton->setIconSize(QSize(16,16));
  m_ui->m_offsetPlusPushButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_offsetPlusPushButton->setIconSize(QSize(16,16));
  m_ui->m_offsetMinusPushButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_offsetMinusPushButton->setIconSize(QSize(16,16));
  m_ui->m_offsetResetPushButton->setIcon(QIcon::fromTheme("edit-undo"));
  m_ui->m_offsetResetPushButton->setIconSize(QSize(16,16));

  m_ui->m_composeMonoLabel->setPixmap(QIcon::fromTheme("bullet-black").pixmap(16,16));
  m_ui->m_composeRedLabel->setPixmap(QIcon::fromTheme("bullet-red").pixmap(16,16));
  m_ui->m_composeGreenLabel->setPixmap(QIcon::fromTheme("bullet-green").pixmap(16,16));
  m_ui->m_composeBlueLabel->setPixmap(QIcon::fromTheme("bullet-blue").pixmap(16,16));

  m_ui->m_composeCRadioButton->setIcon(QIcon::fromTheme("channels").pixmap(16,16));
  m_ui->m_composeRRadioButton->setIcon(QIcon::fromTheme("channel-red").pixmap(16,16));
  m_ui->m_composeGRadioButton->setIcon(QIcon::fromTheme("channel-green").pixmap(16,16));
  m_ui->m_composeBRadioButton->setIcon(QIcon::fromTheme("channel-blue").pixmap(16,16));
  m_ui->m_composeMRadioButton->setIcon(QIcon::fromTheme("channel-gray").pixmap(16,16));

  m_ui->m_rContrastLabel->setPixmap(QIcon::fromTheme("contrast-red").pixmap(16,16));
  m_ui->m_gContrastLabel->setPixmap(QIcon::fromTheme("contrast-green").pixmap(16,16));
  m_ui->m_bContrastLabel->setPixmap(QIcon::fromTheme("contrast-blue").pixmap(16,16));
  m_ui->m_mContrastLabel->setPixmap(QIcon::fromTheme("contrast-mono").pixmap(16,16));

   //set the contrast enhancement names
  m_ceNames.clear();

  m_ceNames.insert(std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::value_type
    (te::se::ContrastEnhancement::ENHANCEMENT_NORMALIZE, tr("Normalize")));
  m_ceNames.insert(std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::value_type
    (te::se::ContrastEnhancement::ENHANCEMENT_HISTOGRAM, tr("Histogram")));
  m_ceNames.insert(std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::value_type
    (te::se::ContrastEnhancement::ENHANCEMENT_NONE, tr("None")));

  std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::iterator it = m_ceNames.begin();

  while(it != m_ceNames.end())
  {
    m_ui->m_contrastTypeComboBox->addItem(it->second);

    ++it;
  }

  m_ui->m_contrastTypeComboBox->setCurrentIndex(m_ui->m_contrastTypeComboBox->findText(m_ceNames[te::se::ContrastEnhancement::ENHANCEMENT_NONE]));

  m_ui->m_contrastTypeComboBox->setVisible(false);
  m_ui->m_contrastTypeLabel->setVisible(false);

  // other values
  m_gainValue = 0.0;
  m_offsetValue = 0.0;
  m_gainOriginalValue = m_gainValue;
  m_offsetOriginalValue = m_offsetValue;
}

void te::qt::widgets::RasterSymbolizerWidget::updateUi()
{
  if(m_symbolizer)
  {
    if(m_symbolizer->getOpacity())
    {
      double value = te::se::GetDouble(m_symbolizer->getOpacity()) * 100.;
      m_sliderWidget->setCurrentValue((int)value);
    }
    else
    {
      onOpacityChanged(m_sliderWidget->getValue());
    }

    if(m_symbolizer->getGain())
    {
      m_gainValue = te::se::GetDouble(m_symbolizer->getGain());
      m_gainOriginalValue = m_gainValue;
    }
    else
    {
      QString s;
      s.setNum(m_gainValue);
      m_symbolizer->setGain(new te::se::ParameterValue(s.toStdString()));
    }

    if(m_symbolizer->getOffset())
    {
      m_offsetValue = te::se::GetDouble(m_symbolizer->getOffset());
      m_offsetOriginalValue = m_offsetValue;
    }
    else
    {
      QString s;
      s.setNum(m_offsetValue);
      m_symbolizer->setOffset(new te::se::ParameterValue(s.toStdString()));
    }

    //update channel selection
    if(m_cs->getRedChannel())
    {
      te::se::SelectedChannel* channel = m_cs->getRedChannel();
      std::string name = channel->getSourceChannelName();
      setComboBoxText(m_ui->m_composeRComboBox, name);

      if(channel->getContrastEnhancement())
      {
        te::se::ContrastEnhancement* ce = channel->getContrastEnhancement();
        double v = ce->getGammaValue() * 100.;
        m_ui->m_contrastRHorizontalSlider->setValue((int)v);
        m_ui->m_contrastTypeComboBox->setCurrentIndex(m_ui->m_contrastTypeComboBox->findText(m_ceNames[ce->getContrastEnhancementType()]));

        m_ui->m_contrastGroupBox->setChecked(true);
      }
      else
      {
        m_ui->m_contrastRHorizontalSlider->setValue(100.);
      }
    }

    if(m_cs->getGreenChannel())
    {
      te::se::SelectedChannel* channel = m_cs->getGreenChannel();
      std::string name = channel->getSourceChannelName();
      setComboBoxText(m_ui->m_composeGComboBox, name);

      if(channel->getContrastEnhancement())
      {
        te::se::ContrastEnhancement* ce = channel->getContrastEnhancement();
        double v = ce->getGammaValue() * 100.;
        m_ui->m_contrastGHorizontalSlider->setValue((int)v);
        m_ui->m_contrastTypeComboBox->setCurrentIndex(m_ui->m_contrastTypeComboBox->findText(m_ceNames[ce->getContrastEnhancementType()]));

        m_ui->m_contrastGroupBox->setChecked(true);
      }
      else
      {
        m_ui->m_contrastGHorizontalSlider->setValue(100.);
      }
    }

    if(m_cs->getBlueChannel())
    {
      te::se::SelectedChannel* channel = m_cs->getBlueChannel();
      std::string name = channel->getSourceChannelName();
      setComboBoxText(m_ui->m_composeBComboBox, name);

      if(channel->getContrastEnhancement())
      {
        te::se::ContrastEnhancement* ce = channel->getContrastEnhancement();
        double v = ce->getGammaValue() * 100.;
        m_ui->m_contrastBHorizontalSlider->setValue((int)v);
        m_ui->m_contrastTypeComboBox->setCurrentIndex(m_ui->m_contrastTypeComboBox->findText(m_ceNames[ce->getContrastEnhancementType()]));

        m_ui->m_contrastGroupBox->setChecked(true);
      }
      else
      {
        m_ui->m_contrastBHorizontalSlider->setValue(100.);
      }
    }

    if(m_cs->getGrayChannel())
    {
      te::se::SelectedChannel* channel = m_cs->getGrayChannel();
      std::string name = channel->getSourceChannelName();
      setComboBoxText(m_ui->m_composeMComboBox, name);

      if(channel->getContrastEnhancement())
      {
        te::se::ContrastEnhancement* ce = channel->getContrastEnhancement();
        double v = ce->getGammaValue() * 100.;
        m_ui->m_contrastMHorizontalSlider->setValue((int)v);
        m_ui->m_contrastTypeComboBox->setCurrentIndex(m_ui->m_contrastTypeComboBox->findText(m_ceNames[ce->getContrastEnhancementType()]));

        m_ui->m_contrastGroupBox->setChecked(true);
      }
      else
      {
        m_ui->m_contrastMHorizontalSlider->setValue(100.);
      }
    }
  }

  setContrastVisibility();
}

void te::qt::widgets::RasterSymbolizerWidget::setComboBoxText(QComboBox* cb, std::string value)
{
  QString name = value.c_str();

  bool found = false;

  for(int i = 0; i < cb->count(); ++i)
  {
    if(cb->itemText(i) == name)
    {
      cb->setCurrentIndex(i);
      found = true;
      break;
    }
  }

  if(!found)
  {
    cb->addItem(name);
    cb->setCurrentIndex(cb->count() - 1);
  }
}

void te::qt::widgets::RasterSymbolizerWidget::onOpacityChanged(int value)
{
  if(m_symbolizer)
  {
    int opacity = value;
    double seOpacity = opacity / 100.;
    QString qStrOpacity;
    qStrOpacity.setNum(seOpacity);

    m_symbolizer->setOpacity(new te::se::ParameterValue(qStrOpacity.toStdString()));
  }
}

void te::qt::widgets::RasterSymbolizerWidget::onMonoChannelSelectionClicked()
{
  m_ui->m_composeMComboBox->setEnabled(true);
  m_ui->m_composeRComboBox->setEnabled(false);
  m_ui->m_composeGComboBox->setEnabled(false);
  m_ui->m_composeBComboBox->setEnabled(false);

  if(m_scMono == 0)
  {
    m_scMono = new te::se::SelectedChannel();
    m_scMono->setSourceChannelName(m_ui->m_composeMComboBox->currentText().toStdString());
    m_cs->setGrayChannel(m_scMono);
  }

  m_cs->setColorCompositionType(te::se::GRAY_COMPOSITION);

  onSymbolizerChanged();

  setContrastVisibility();
}

void te::qt::widgets::RasterSymbolizerWidget::onRedChannelSelectionClicked()
{
  m_ui->m_composeMComboBox->setEnabled(false);
  m_ui->m_composeRComboBox->setEnabled(true);
  m_ui->m_composeGComboBox->setEnabled(false);
  m_ui->m_composeBComboBox->setEnabled(false);

  if(m_scRed == 0)
  {
    m_scRed = new te::se::SelectedChannel();
    m_scRed->setSourceChannelName(m_ui->m_composeRComboBox->currentText().toStdString());
    m_cs->setRedChannel(m_scRed);
  }

  m_cs->setColorCompositionType(te::se::RED_COMPOSITION);

  onSymbolizerChanged();

  setContrastVisibility();
}

void te::qt::widgets::RasterSymbolizerWidget::onGreenChannelSelectionClicked()
{
  m_ui->m_composeMComboBox->setEnabled(false);
  m_ui->m_composeRComboBox->setEnabled(false);
  m_ui->m_composeGComboBox->setEnabled(true);
  m_ui->m_composeBComboBox->setEnabled(false);

  if(m_scGreen== 0)
  {
    m_scGreen = new te::se::SelectedChannel();
    m_scGreen->setSourceChannelName(m_ui->m_composeGComboBox->currentText().toStdString());
    m_cs->setGreenChannel(m_scGreen);
  }

  m_cs->setColorCompositionType(te::se::GREEN_COMPOSITION);

  onSymbolizerChanged();

  setContrastVisibility();
}

void te::qt::widgets::RasterSymbolizerWidget::onBlueChannelSelectionClicked()
{
  m_ui->m_composeMComboBox->setEnabled(false);
  m_ui->m_composeRComboBox->setEnabled(false);
  m_ui->m_composeGComboBox->setEnabled(false);
  m_ui->m_composeBComboBox->setEnabled(true);

  if(m_scBlue== 0)
  {
    m_scBlue = new te::se::SelectedChannel();
    m_scBlue->setSourceChannelName(m_ui->m_composeBComboBox->currentText().toStdString());
    m_cs->setBlueChannel(m_scBlue);
  }

  m_cs->setColorCompositionType(te::se::BLUE_COMPOSITION);

  onSymbolizerChanged();

  setContrastVisibility();
}

void te::qt::widgets::RasterSymbolizerWidget::onCompositionChannelSelectionClicked()
{
  m_ui->m_composeMComboBox->setEnabled(false);
  m_ui->m_composeRComboBox->setEnabled(true);
  m_ui->m_composeGComboBox->setEnabled(true);
  m_ui->m_composeBComboBox->setEnabled(true);

  if(m_scRed == 0)
  {
    m_scRed = new te::se::SelectedChannel();
    m_scRed->setSourceChannelName(m_ui->m_composeRComboBox->currentText().toStdString());
    m_cs->setRedChannel(m_scRed);
  }

  if(m_scGreen== 0)
  {
    m_scGreen = new te::se::SelectedChannel();
    m_scGreen->setSourceChannelName(m_ui->m_composeGComboBox->currentText().toStdString());
    m_cs->setGreenChannel(m_scGreen);
  }

  if(m_scBlue== 0)
  {
    m_scBlue = new te::se::SelectedChannel();
    m_scBlue->setSourceChannelName(m_ui->m_composeBComboBox->currentText().toStdString());
    m_cs->setBlueChannel(m_scBlue);
  }

  m_cs->setColorCompositionType(te::se::RGB_COMPOSITION);

  onSymbolizerChanged();

  setContrastVisibility();
}

void te::qt::widgets::RasterSymbolizerWidget::onMonoChannelNameChanged(QString s)
{
  m_scMono->setSourceChannelName(s.toLatin1().data());

  onSymbolizerChanged();
}

void te::qt::widgets::RasterSymbolizerWidget::onRedChannelNameChanged(QString s)
{
  m_scRed->setSourceChannelName(s.toLatin1().data());

  onSymbolizerChanged();
}

void te::qt::widgets::RasterSymbolizerWidget::onGreenChannelNameChanged(QString s)
{
  m_scGreen->setSourceChannelName(s.toLatin1().data());

  onSymbolizerChanged();
}

void te::qt::widgets::RasterSymbolizerWidget::onBlueChannelNameChanged(QString s)
{
  m_scBlue->setSourceChannelName(s.toLatin1().data());

  onSymbolizerChanged();
}

void te::qt::widgets::RasterSymbolizerWidget::onTypeConstratChanged(QString s)
{
  if(m_contrastRed == 0)
  {
    m_contrastRed = new te::se::ContrastEnhancement();
    m_scRed->setContrastEnhancement(m_contrastRed);
  }

  if(m_contrastGreen == 0)
  {
    m_contrastGreen = new te::se::ContrastEnhancement();
    m_scGreen->setContrastEnhancement(m_contrastGreen);
  }

  if(m_contrastBlue == 0)
  {
    m_contrastBlue = new te::se::ContrastEnhancement();
    m_scBlue->setContrastEnhancement(m_contrastBlue);
  }

  if(m_contrastMono == 0)
  {
    m_contrastMono = new te::se::ContrastEnhancement();
    m_scMono->setContrastEnhancement(m_contrastMono);
  }

  std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString>::iterator it = m_ceNames.begin();

  while(it != m_ceNames.end())
  {
    if(it->second == s)
    {
      m_contrastRed->setContrastEnhancementType(it->first);
      m_contrastGreen->setContrastEnhancementType(it->first);
      m_contrastBlue->setContrastEnhancementType(it->first);
      m_contrastMono->setContrastEnhancementType(it->first);

      break;
    }

    ++it;
  }

  onSymbolizerChanged();
}

void te::qt::widgets::RasterSymbolizerWidget::onMonoGammaChanged(int v)
{
  if(m_contrastMono == 0)
  {
    m_contrastMono = new te::se::ContrastEnhancement();
    m_scMono->setContrastEnhancement(m_contrastMono);
  }

  double value = double(v) / 100.;

  m_contrastMono->setGammaValue(value);
}

void te::qt::widgets::RasterSymbolizerWidget::onRedGammaChanged(int v)
{
  if(m_contrastRed == 0)
  {
    m_contrastRed = new te::se::ContrastEnhancement();
    m_scRed->setContrastEnhancement(m_contrastRed);
  }

  double value = double(v) / 100.;

  m_contrastRed->setGammaValue(value);
}

void te::qt::widgets::RasterSymbolizerWidget::onGreenGammaChanged(int v)
{
  if(m_contrastGreen == 0)
  {
    m_contrastGreen = new te::se::ContrastEnhancement();
    m_scGreen->setContrastEnhancement(m_contrastGreen);
  }

  double value = double(v) / 100.;

  m_contrastGreen->setGammaValue(value);
}

void te::qt::widgets::RasterSymbolizerWidget::onBlueGammaChanged(int v)
{
  if(m_contrastBlue == 0)
  {
    m_contrastBlue = new te::se::ContrastEnhancement();
    m_scBlue->setContrastEnhancement(m_contrastBlue);
  }

  double value = double(v) / 100.;

  m_contrastBlue->setGammaValue(value);
}

void te::qt::widgets::RasterSymbolizerWidget::onIncreaseGain()
{
  m_gainValue += GAIN_CONSTANT_VALUE;

  QString s;
  s.setNum(m_gainValue);

  if(m_symbolizer)
  {
    m_symbolizer->setGain(new te::se::ParameterValue(s.toStdString()));

    onSymbolizerChanged();
  }
}

void te::qt::widgets::RasterSymbolizerWidget::onDecreaseGain()
{
  m_gainValue -= GAIN_CONSTANT_VALUE;

  QString s;
  s.setNum(m_gainValue);

  if(m_symbolizer)
  {
    m_symbolizer->setGain(new te::se::ParameterValue(s.toStdString()));

    onSymbolizerChanged();
  }
}

void te::qt::widgets::RasterSymbolizerWidget::onDefaultGain()
{
  m_gainValue = m_gainOriginalValue;

  QString s;
  s.setNum(m_gainValue);

  if(m_symbolizer)
  {
    m_symbolizer->setGain(new te::se::ParameterValue(s.toStdString()));

    onSymbolizerChanged();
  }
}

void te::qt::widgets::RasterSymbolizerWidget::onIncreaseOffset()
{
  m_offsetValue += OFFSET_CONSTANT_VALUE;

  QString s;
  s.setNum(m_offsetValue);
  
  if(m_symbolizer)
  {
    m_symbolizer->setOffset(new te::se::ParameterValue(s.toStdString()));

    onSymbolizerChanged();
  }
}

void te::qt::widgets::RasterSymbolizerWidget::onDecreaseOffset()
{
  m_offsetValue -= OFFSET_CONSTANT_VALUE;

  QString s;
  s.setNum(m_offsetValue);
  
  if(m_symbolizer)
  {
    m_symbolizer->setOffset(new te::se::ParameterValue(s.toStdString()));

    onSymbolizerChanged();
  }
}

void te::qt::widgets::RasterSymbolizerWidget::onDefaultOffset()
{
  m_offsetValue = m_offsetOriginalValue;

  QString s;
  s.setNum(m_offsetValue);

  if(m_symbolizer)
  {
    m_symbolizer->setOffset(new te::se::ParameterValue(s.toStdString()));

    onSymbolizerChanged();
  }
}

void te::qt::widgets::RasterSymbolizerWidget::onSymbolizerChanged()
{
  emit symbolizerChanged();
}

void te::qt::widgets::RasterSymbolizerWidget::setContrastVisibility()
{
  if(m_ui->m_contrastGroupBox->isChecked() == false)
  {
    m_ui->m_contrastRHorizontalSlider->setValue(100);
    m_ui->m_contrastGHorizontalSlider->setValue(100);
    m_ui->m_contrastBHorizontalSlider->setValue(100);
    m_ui->m_contrastMHorizontalSlider->setValue(100);

    return;
  }

  m_ui->m_contrastRHorizontalSlider->setEnabled(false);
  m_ui->m_contrastGHorizontalSlider->setEnabled(false);
  m_ui->m_contrastBHorizontalSlider->setEnabled(false);
  m_ui->m_contrastMHorizontalSlider->setEnabled(false);

  if(m_ui->m_composeMRadioButton->isChecked())
  {
    m_ui->m_contrastMHorizontalSlider->setEnabled(true);
  }

  if(m_ui->m_composeRRadioButton->isChecked())
  {
    m_ui->m_contrastRHorizontalSlider->setEnabled(true);
  }

  if(m_ui->m_composeGRadioButton->isChecked())
  {
    m_ui->m_contrastGHorizontalSlider->setEnabled(true);
  }

  if(m_ui->m_composeBRadioButton->isChecked())
  {
    m_ui->m_contrastBHorizontalSlider->setEnabled(true);
  }

  if(m_ui->m_composeCRadioButton->isChecked())
  {
    m_ui->m_contrastRHorizontalSlider->setEnabled(true);
    m_ui->m_contrastGHorizontalSlider->setEnabled(true);
    m_ui->m_contrastBHorizontalSlider->setEnabled(true);
  }
}
