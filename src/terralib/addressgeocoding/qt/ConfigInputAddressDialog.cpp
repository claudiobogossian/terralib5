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
#include "ui_ConfigInputAddressDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

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

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  std::string fileName;
  std::string streetName;
}

te::addressgeocoding::ConfigInputAddressDialog::~ConfigInputAddressDialog()
{
}

te::da::DataSourcePtr te::addressgeocoding::ConfigInputAddressDialog::getDataSource()
{
  return m_dataSource;
}

void te::addressgeocoding::ConfigInputAddressDialog::setDataSource(te::da::DataSourcePtr dataSource)
{
  m_dataSource = dataSource;
  std::vector<std::string> names = m_dataSource->getDataSetNames();

  m_ui->m_inputAddressLineEdit->clear();
  m_ui->m_inputAddressLineEdit->setText(QString(names[0].c_str()));

  m_ui->m_sTypeComboBox->clear();
  m_ui->m_sTitleComboBox->clear();
  m_ui->m_sNameComboBox->clear();
  m_ui->m_sNumberComboBox->clear();
  m_ui->m_sNeighborhoodComboBox->clear();
  m_ui->m_sPostalCodeComboBox->clear();

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

std::string te::addressgeocoding::ConfigInputAddressDialog::getAddressFileName()
{
  return m_ui->m_inputAddressLineEdit->text().toStdString();
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetType()
{
  return m_ui->m_sTypeComboBox->currentText().toStdString();
}

void te::addressgeocoding::ConfigInputAddressDialog::setStreetType(std::string streetType)
{
  m_ui->m_sTypeComboBox->setCurrentIndex(m_ui->m_sTypeComboBox->findText(streetType.c_str()));
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetTitle()
{
  return m_ui->m_sTitleComboBox->currentText().toStdString();
}

void te::addressgeocoding::ConfigInputAddressDialog::setStreetTitle(std::string streetTitle)
{
  m_ui->m_sTitleComboBox->setCurrentIndex(m_ui->m_sTitleComboBox->findText(streetTitle.c_str()));
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetName()
{
  return m_ui->m_sNameComboBox->currentText().toStdString();
}

void te::addressgeocoding::ConfigInputAddressDialog::setStreetName(std::string streetName)
{
  m_ui->m_sNameComboBox->setCurrentIndex(m_ui->m_sNameComboBox->findText(streetName.c_str()));
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetNumber()
{
  return m_ui->m_sNumberComboBox->currentText().toStdString();
}

void te::addressgeocoding::ConfigInputAddressDialog::setStreetNumber(std::string streetNumber)
{
  m_ui->m_sNumberComboBox->setCurrentIndex(m_ui->m_sNumberComboBox->findText(streetNumber.c_str()));
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetNeighborhood()
{
  return m_ui->m_sNeighborhoodComboBox->currentText().toStdString();
}

void te::addressgeocoding::ConfigInputAddressDialog::setStreetNeighborhood(std::string streetNeighborhood)
{
  m_ui->m_sNeighborhoodComboBox->setCurrentIndex(m_ui->m_sNeighborhoodComboBox->findText(streetNeighborhood.c_str()));
}

std::string te::addressgeocoding::ConfigInputAddressDialog::getStreetPostalCode()
{
  return m_ui->m_sPostalCodeComboBox->currentText().toStdString();
}

void te::addressgeocoding::ConfigInputAddressDialog::setStreetPostalCode(std::string streetPostalCode)
{
  m_ui->m_sPostalCodeComboBox->setCurrentIndex(m_ui->m_sPostalCodeComboBox->findText(streetPostalCode.c_str()));
}

void te::addressgeocoding::ConfigInputAddressDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::addressgeocoding::ConfigInputAddressDialog::onOkPushButtonClicked()
{
  std::map<std::string, std::string> connInfo = m_dataSource->getConnectionInfo();
  std::map<std::string, std::string>::iterator it = connInfo.begin();

  std::vector<std::string>dsNames = m_dataSource->getDataSetNames();

  std::map<std::string, std::string> addressConfig;
  addressConfig.insert(std::pair<std::string,std::string>("fileName", dsNames[0]));
  addressConfig.insert(std::pair<std::string,std::string>("filePath", it->second));
  addressConfig.insert(std::pair<std::string,std::string>("streetType", m_ui->m_sTypeComboBox->currentText().toStdString()));
  addressConfig.insert(std::pair<std::string,std::string>("streetTitle", m_ui->m_sTitleComboBox->currentText().toStdString()));
  addressConfig.insert(std::pair<std::string,std::string>("streetName", m_ui->m_sNameComboBox->currentText().toStdString()));
  addressConfig.insert(std::pair<std::string,std::string>("number", m_ui->m_sNumberComboBox->currentText().toStdString()));
  addressConfig.insert(std::pair<std::string,std::string>("neighborhood", m_ui->m_sNeighborhoodComboBox->currentText().toStdString()));
  addressConfig.insert(std::pair<std::string,std::string>("postalCode", m_ui->m_sPostalCodeComboBox->currentText().toStdString()));
  
  AddAddressConfigToSettings(addressConfig);

  this->accept();
}

void te::addressgeocoding::ConfigInputAddressDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::addressgeocoding::ConfigInputAddressDialog::AddAddressConfigToSettings(std::map<std::string, std::string> addressConfig)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("Address Geocoding");

  AddAddressConfigItens(addressConfig, sett);

  sett.endGroup();
}

void te::addressgeocoding::ConfigInputAddressDialog::AddAddressConfigItens(std::map<std::string, std::string> addressConfig, QSettings& sett)
{
  std::map<std::string, std::string>::iterator it = addressConfig.begin();

  sett.beginGroup(it->second.c_str());

  while(it != addressConfig.end())
  {
    sett.setValue(it->first.c_str(), it->second.c_str());
    ++it;
  }

  sett.endGroup();

}

void te::addressgeocoding::ConfigInputAddressDialog::AddAddressConfigToSettings(std::string filePath,
                                                                                std::string streetType,
                                                                                std::string streetTitle,
                                                                                std::string streetName,
                                                                                std::string number,
                                                                                std::string neighborhood,
                                                                                std::string postalCode)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  QString key1 = "Address Geocoding/filePath";
  QString key2 = "Address Geocoding/streetType";
  QString key3 = "Address Geocoding/streetTitle";
  QString key4 = "Address Geocoding/streetName";
  QString key5 = "Address Geocoding/number";
  QString key6 = "Address Geocoding/neighborhood";
  QString key7 = "Address Geocoding/postalCode";

  sett.setValue(key1, filePath.c_str());
  sett.setValue(key2, streetType.c_str());
  sett.setValue(key3, streetTitle.c_str());
  sett.setValue(key4, streetName.c_str());
  sett.setValue(key5, number.c_str());
  sett.setValue(key6, neighborhood.c_str());
  sett.setValue(key7, postalCode.c_str());
}

void te::addressgeocoding::ConfigInputAddressDialog::GetAddressConfigToSettings(std::string& filePath,
                                                                                std::string& streetType,
                                                                                std::string& streetTitle,
                                                                                std::string& streetName,
                                                                                std::string& number,
                                                                                std::string& neighborhood,
                                                                                std::string& postalCode)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  std::string key1 = "Address Geocoding/filePath";
  std::string key2 = "Address Geocoding/streetType";
  std::string key3 = "Address Geocoding/streetTitle";
  std::string key4 = "Address Geocoding/streetName";
  std::string key5 = "Address Geocoding/number";
  std::string key6 = "Address Geocoding/neighborhood";
  std::string key7 = "Address Geocoding/postalCode";

  filePath = sett.value(key1.c_str()).toString().toStdString();
  streetType = sett.value(key2.c_str()).toString().toStdString();
  streetTitle = sett.value(key3.c_str()).toString().toStdString();
  streetName = sett.value(key4.c_str()).toString().toStdString();
  number = sett.value(key5.c_str()).toString().toStdString();
  neighborhood = sett.value(key5.c_str()).toString().toStdString();
  postalCode = sett.value(key6.c_str()).toString().toStdString();
}

void te::addressgeocoding::ConfigInputAddressDialog::RemoveAddressConfigFromSettings(const QString& path, const QString& typeFile)
{

}
