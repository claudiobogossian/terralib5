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
  \file terralib/qt/widgets/se/SelectedChannelWidget.cpp

  \brief A dialog used to build a SelectedChannel element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "ContrastEnhancementWidget.h"
#include "SelectedChannelWidget.h"
#include "ui_SelectedChannelWidgetForm.h"
#include "../../../se.h"

// Qt


// STL
#include <cassert>


te::qt::widgets::SelectedChannelWidget::SelectedChannelWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::SelectedChannelWidgetForm),
    m_se(new te::se::SelectedChannel)
{
  m_ui->setupUi(this);

  //Constrast Enhancement Widget
  m_contrastWidget = new te::qt::widgets::ContrastEnhancementWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_contrastEnhancementFrame);
  layout->addWidget(m_contrastWidget);

  // Signals & slots
  connect(m_ui->m_channelNameComboBox, SIGNAL(activated(QString)), SLOT(onChannelNameChanged(QString)));
  connect(m_contrastWidget, SIGNAL(contrastEnhancementChanged()), SLOT(onContrastEnhancementChanged()));

  initialize();
}

te::qt::widgets::SelectedChannelWidget::~SelectedChannelWidget()
{
//  delete m_se;
}

void te::qt::widgets::SelectedChannelWidget::setSelectedChannel( te::se::SelectedChannel* sc)
{
  assert(sc);

  //delete m_se;

  m_se = sc;//->clone();

  updateUi();
}

te::se::SelectedChannel* te::qt::widgets::SelectedChannelWidget::getSelectedChannel() const
{
  m_se->setContrastEnhancement(m_contrastWidget->getContrastEnhancement());

  return m_se;//->clone();
}

void te::qt::widgets::SelectedChannelWidget::setChannelNames(const QStringList& list)
{
  m_ui->m_channelNameComboBox->clear();
  m_ui->m_channelNameComboBox->addItems(list);

  m_se->setSourceChannelName(m_ui->m_channelNameComboBox->currentText().toLatin1().data());
}

void te::qt::widgets::SelectedChannelWidget::initialize()
{
}

void te::qt::widgets::SelectedChannelWidget::updateUi()
{
  // update the channel name
  QString name = m_se->getSourceChannelName().c_str();

  bool found = false;

  for(int i = 0; i < m_ui->m_channelNameComboBox->count(); ++i)
  {
    if(m_ui->m_channelNameComboBox->itemText(i) == name)
    {
      m_ui->m_channelNameComboBox->setCurrentIndex(i);
      found = true;
      break;
    }
  }

  if(!found)
  {
    m_ui->m_channelNameComboBox->addItem(m_se->getSourceChannelName().c_str());
    m_ui->m_channelNameComboBox->setCurrentIndex(m_ui->m_channelNameComboBox->count() - 1);
  }

  if(m_se->getContrastEnhancement())
  {
    m_contrastWidget->setContrastEnhancement(m_se->getContrastEnhancement());
  }
}

void te::qt::widgets::SelectedChannelWidget::onChannelNameChanged(QString value)
{
  m_se->setSourceChannelName(value.toLatin1().data());

  emit selectedChannelChanged();
}

void te::qt::widgets::SelectedChannelWidget::onContrastEnhancementChanged()
{
  m_se->setContrastEnhancement(m_contrastWidget->getContrastEnhancement());

  emit selectedChannelChanged();
}
