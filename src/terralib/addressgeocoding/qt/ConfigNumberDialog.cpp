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
  \file terralib/addressgeocoding/ConfigNumberDialog.cpp

  \brief A dialog to import table with address to geocoding operation
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../Config.h"
#include "../Exception.h"
#include "ConfigNumberDialog.h"
#include "ui_ConfigNumberDialogForm.h"

// Qt
#include <QMessageBox>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>

te::addressgeocoding::ConfigNumberDialog::ConfigNumberDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::ConfigNumberDialogForm)
{
// add controls
  m_ui->setupUi(this);

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::addressgeocoding::ConfigNumberDialog::~ConfigNumberDialog()
{
}

void te::addressgeocoding::ConfigNumberDialog::setLayer(te::map::AbstractLayerPtr layer)
{
  m_selectedLayer = layer;
  m_ui->m_layerLineEdit->setText(QString(m_selectedLayer->getTitle().c_str()));

  std::auto_ptr<te::da::DataSetType> dsType = m_selectedLayer->getSchema();
  std::vector<te::dt::Property*> propVec = dsType->getProperties();

  int type;

  for(std::size_t i = 0; i < propVec.size(); ++i)
  {
    type = propVec[i]->getType();
    if(type == te::dt::INT16_TYPE || 
      type == te::dt::INT32_TYPE || 
      type == te::dt::INT64_TYPE || 
      type == te::dt::DOUBLE_TYPE || 
      type == te::dt::NUMERIC_TYPE)
    {
      m_ui->m_iLeftComboBox->addItem(QString(propVec[i]->getName().c_str()));
      m_ui->m_fLeftComboBox->addItem(QString(propVec[i]->getName().c_str()));
      m_ui->m_iRightComboBox->addItem(QString(propVec[i]->getName().c_str()));
      m_ui->m_fRightComboBox->addItem(QString(propVec[i]->getName().c_str()));
    }
  }
}

std::string te::addressgeocoding::ConfigNumberDialog::getInitialLeft()
{
  return m_ui->m_iLeftComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigNumberDialog::getFinalLeft()
{
  return m_ui->m_fLeftComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigNumberDialog::getInitialRight()
{
  return m_ui->m_iRightComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigNumberDialog::getFinalRight()
{
  return m_ui->m_fRightComboBox->currentText().toStdString();
}

void te::addressgeocoding::ConfigNumberDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::addressgeocoding::ConfigNumberDialog::onOkPushButtonClicked()
{
  this->close();
}

void te::addressgeocoding::ConfigNumberDialog::onCancelPushButtonClicked()
{
  reject();
}

