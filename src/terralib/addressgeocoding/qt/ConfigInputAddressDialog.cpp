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
  \file terralib/addressgeocoding/ImportTableDialog.cpp

  \brief A dialog to import table with address to geocoding operation
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
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
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Exception.h"
#include "ConfigInputAddressDialog.h"
#include "ImportTableDialog.h"
#include "ui_ConfigInputAddressDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::addressgeocoding::ConfigInputAddressDialog::ConfigInputAddressDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ConfigInputAddressDialogForm)
{
// add controls
  m_ui->setupUi(this);

  connect(m_ui->m_importTablePushButton, SIGNAL(clicked()), this, SLOT(onImportTableClicked()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::addressgeocoding::ConfigInputAddressDialog::~ConfigInputAddressDialog()
{
}

te::da::DataSourcePtr te::addressgeocoding::ConfigInputAddressDialog::getDataSource()
{
  return m_dataSource;
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getAddressFileName()
{
  return m_ui->m_inputAddressComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetType()
{
  return m_ui->m_sTypeComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetTitle()
{
  return m_ui->m_sTitleComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetName()
{
  return m_ui->m_sNameComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetNumber()
{
  return m_ui->m_sNumberComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetNeighborhood()
{
  return m_ui->m_sNeighborhoodComboBox->currentText().toStdString();
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetPostalCode()
{
  return m_ui->m_sPostalCodeComboBox->currentText().toStdString();
}


void te::addressgeocoding::ConfigInputAddressDialog::onImportTableClicked()
{
  te::addressgeocoding::ImportTableDialog dlg(this);

  if(dlg.exec() != QDialog::Accepted)
  {
    m_ui->m_inputAddressComboBox->clear();
    m_ui->m_sTypeComboBox->clear();
    m_ui->m_sTitleComboBox->clear();
    m_ui->m_sNameComboBox->clear();
    m_ui->m_sNumberComboBox->clear();
    m_ui->m_sNeighborhoodComboBox->clear();
    m_ui->m_sPostalCodeComboBox->clear();

    m_dataSource = dlg.getDataSource();
    m_dataSet = dlg.getDataSet();

    if(m_dataSource)
    {
      std::vector<std::string> names = m_dataSource->getDataSetNames();
      m_ui->m_inputAddressComboBox->addItem(QString(names[0].c_str()));

      m_ui->m_sTypeComboBox->addItem("");
      m_ui->m_sTitleComboBox->addItem("");
      m_ui->m_sNameComboBox->addItem("");
      m_ui->m_sNumberComboBox->addItem("");
      m_ui->m_sNeighborhoodComboBox->addItem("");
      m_ui->m_sPostalCodeComboBox->addItem("");

      std::auto_ptr<te::da::DataSetType> dsType = m_dataSource->getDataSetType(names[0]);
      std::vector<te::dt::Property*> propsVec = dsType->getProperties();

      for(std::size_t i = 0; i < propsVec.size(); ++i)
      {
        int type = propsVec[i]->getType();
        if(type == te::dt::INT16_TYPE || type == te::dt::INT32_TYPE || type == te::dt::INT64_TYPE || type == te::dt::DOUBLE_TYPE )
        {
          m_ui->m_sNumberComboBox->addItem(QString(propsVec[i]->getName().c_str()));
        }
        else
        {
          m_ui->m_sTypeComboBox->addItem(QString(propsVec[i]->getName().c_str()));
          m_ui->m_sTitleComboBox->addItem(QString(propsVec[i]->getName().c_str()));
          m_ui->m_sNameComboBox->addItem(QString(propsVec[i]->getName().c_str()));
          m_ui->m_sNeighborhoodComboBox->addItem(QString(propsVec[i]->getName().c_str()));
          m_ui->m_sPostalCodeComboBox->addItem(QString(propsVec[i]->getName().c_str()));
        }
      }
    }
  }
  return;
}

void te::addressgeocoding::ConfigInputAddressDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::addressgeocoding::ConfigInputAddressDialog::onOkPushButtonClicked()
{
  this->close();
}

void te::addressgeocoding::ConfigInputAddressDialog::onCancelPushButtonClicked()
{
  reject();
}

