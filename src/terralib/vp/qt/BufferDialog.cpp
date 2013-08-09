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
  \file terralib/vp/BufferDialog.cpp

  \brief A dialog for buffer operation
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../srs/Config.h"
#include "../Config.h"
#include "../Exception.h"
#include "BufferDialog.h"
#include "ui_BufferDialogForm.h"
#include "VectorProcessingConfig.h"
#include "Utils.h"

// Qt
#include <QtCore/QList>
#include <QtCore/QSize.h>
#include <QtGui/QFileDialog>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>

te::vp::BufferDialog::BufferDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::BufferDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_selectedLayer(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme(VP_IMAGES"/vp-buffer-hint").pixmap(112,48));
  
  QSize iconSize(96, 48);

  m_ui->m_withoutBoundRadioButton->setIconSize(iconSize);
  m_ui->m_withoutBoundRadioButton->setIcon(QIcon::fromTheme("buffer-without-boundaries"));

  m_ui->m_withBoundRadioButton->setIconSize(iconSize);
  m_ui->m_withBoundRadioButton->setIcon(QIcon::fromTheme("buffer-with-boundaries"));

  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer-inside-outside").pixmap(150,60));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));  

//add controls
  m_ui->m_fixedDistanceLineEdit->setEnabled(true);
  m_ui->m_fixedDistanceComboBox->setEnabled(true);

  setPossibleLevels();

//signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));
  connect(m_ui->m_filterLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onFilterLineEditTextChanged(const QString&)));
  connect(m_ui->m_fixedRadioButton, SIGNAL(toggled(bool)), this, SLOT(onFixedDistanceToggled()));
  connect(m_ui->m_fromAttRadioButton, SIGNAL(toggled(bool)), this, SLOT(onAttDistanceToggled()));
  connect(m_ui->m_ruleInOutRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRuleInOutToggled()));
  connect(m_ui->m_ruleOnlyOutRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRuleOutToggled()));
  connect(m_ui->m_ruleOnlyInRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRuleInToggled()));
  connect(m_ui->m_withoutBoundRadioButton, SIGNAL(toggled(bool)), this, SLOT(onWithoutBoundToggled()));
  connect(m_ui->m_withBoundRadioButton, SIGNAL(toggled(bool)), this, SLOT(onWithBoundToggled()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::vp::BufferDialog::~BufferDialog()
{
}

void te::vp::BufferDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {  
    m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

void te::vp::BufferDialog::setPossibleLevels()
{
  for(int i=1; i <= 10; ++i)
    m_ui->m_levelsNumComboBox->addItem(QString::number(i));
}

void te::vp::BufferDialog::setAttributesForDistance(std::vector<te::dt::Property*> properties)
{
  m_ui->m_fromAttDistanceComboBox->clear();

  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    if(properties[i]->getType() == te::dt::CDOUBLE_TYPE ||
       properties[i]->getType() == te::dt::CFLOAT_TYPE ||
       properties[i]->getType() == te::dt::CINT16_TYPE ||
       properties[i]->getType() == te::dt::CINT32_TYPE ||
       properties[i]->getType() == te::dt::DOUBLE_TYPE || 
       properties[i]->getType() == te::dt::INT16_TYPE ||                              
       properties[i]->getType() == te::dt::INT32_TYPE || 
       properties[i]->getType() == te::dt::INT64_TYPE ||
       properties[i]->getType() == te::dt::FLOAT_TYPE ||
       properties[i]->getType() == te::dt::NUMERIC_TYPE ||
       properties[i]->getType() == te::dt::UINT16_TYPE ||
       properties[i]->getType() == te::dt::UINT32_TYPE ||
       properties[i]->getType() == te::dt::UINT64_TYPE)
      m_ui->m_fromAttDistanceComboBox->addItem(properties[i]->getName().c_str());
  }

  if(m_ui->m_fromAttDistanceComboBox->count() > 0)
    m_ui->m_fromAttRadioButton->setEnabled(true);
  else
    m_ui->m_fromAttRadioButton->setEnabled(false);
}

void te::vp::BufferDialog::onLayerComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      std::size_t type;
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_selectedLayer = selectedLayer;
      std::auto_ptr<const te::map::LayerSchema> schema(selectedLayer->getSchema());

      if(schema->size() == 0)
        return;

      te::common::FreeContents(m_properties);
      m_properties.clear();

      const std::vector<te::dt::Property*>& properties = schema->getProperties();
      te::common::Clone(properties, m_properties);

      setAttributesForDistance(m_properties);

      return;
    }
    ++it;
  }
}

void te::vp::BufferDialog::onFilterLineEditTextChanged(const QString& text)
{
  std::list<te::map::AbstractLayerPtr> filteredLayers = te::vp::GetFilteredLayers(text.toStdString(), m_layers);

  m_ui->m_layersComboBox->clear();

  if(text.isEmpty())
    filteredLayers = m_layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = filteredLayers.begin();

  while(it != filteredLayers.end())
  {  
    m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

void te::vp::BufferDialog::onFixedDistanceToggled()
{
  m_ui->m_fixedDistanceLineEdit->setEnabled(true);
  m_ui->m_fixedDistanceComboBox->setEnabled(true);
  m_ui->m_fromAttDistanceComboBox->setEnabled(false);
}

void te::vp::BufferDialog::onAttDistanceToggled()
{
  m_ui->m_fromAttDistanceComboBox->setEnabled(true);
  m_ui->m_fixedDistanceLineEdit->setEnabled(false);
  m_ui->m_fixedDistanceComboBox->setEnabled(false);
}

void te::vp::BufferDialog::onRuleInOutToggled()
{
  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer-inside-outside").pixmap(150,60));
}

void te::vp::BufferDialog::onRuleOutToggled()
{
  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer_only-outside").pixmap(150,60));
}

void te::vp::BufferDialog::onRuleInToggled()
{
  m_ui->m_ruleImgLabel->setPixmap(QIcon::fromTheme("buffer-only-inside").pixmap(150,60));
}

void te::vp::BufferDialog::onWithoutBoundToggled()
{
  m_ui->m_copyColumnsCheckBox->setChecked(false);
  m_ui->m_copyColumnsCheckBox->setEnabled(false);
}

void te::vp::BufferDialog::onWithBoundToggled()
{
  m_ui->m_copyColumnsCheckBox->setEnabled(true);
}

void te::vp::BufferDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::vp::BufferDialog::onOkPushButtonClicked()
{
  QMessageBox::information(this, "Ok", "Under development");
}

void te::vp::BufferDialog::onCancelPushButtonClicked()
{
  reject();
}