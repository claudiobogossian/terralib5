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
  \file terralib/addressgeocoding/MainWindowDialog.cpp

  \brief A dialog for Address Geocoding operation
*/

// TerraLib
#include "../../common/CharEncodingConv.h"
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
#include "../../qt/widgets/Utils.h"
#include "../AddressGeocodingOp.h"
#include "../Config.h"
#include "../Exception.h"
#include "MainWindowDialog.h"
#include "ConfigInputAddressDialog.h"
#include "ConfigInputLayerDialog.h"
#include "ui_MainWindowDialogForm.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTreeWidget>
#include <QSettings>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::addressgeocoding::MainWindowDialog::MainWindowDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::MainWindowDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_newColumnLayer(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("addressgeocoding-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));
  m_ui->m_targetDataSourceAddressToolButton->setIcon(QIcon::fromTheme("datasource"));
  m_ui->m_editAddressFileToolButton->setIcon(QIcon::fromTheme("preferences-system"));
  m_ui->m_removeAddressFileToolButton->setIcon(QIcon::fromTheme("delete"));

  connect(m_ui->m_inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));
  connect(m_ui->m_iLeftComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInitialLeftComboBoxChanged(int)));
  connect(m_ui->m_fLeftComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFinalLeftComboBoxChanged(int)));
  connect(m_ui->m_iRightComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInitialRightComboBoxChanged(int)));
  connect(m_ui->m_fRightComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFinalRightComboBoxChanged(int)));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_configureLayerPushButton, SIGNAL(clicked()), this, SLOT(onConfigureLayerClicked()));
  connect(m_ui->m_targetFileAddressToolButton, SIGNAL(clicked()), this, SLOT(onTargetFileAddressToolButtonPressed()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

// Load the Address file path.
  std::string filePath;
  GetAddressFilePathToSettings(filePath);

// Load the Address Data Source.
  GetDataSourceAddress(filePath);
  
  std::vector<std::string>vecNames;
  if(m_addressDataSource)
  {
    vecNames = m_addressDataSource->getDataSetNames();
    m_addressFile = vecNames[0];
    m_ui->m_inputAddressComboBox->addItem(QString(m_addressFile.c_str()));
  }

// Load the Address parameters.
  std::string streetType;
  std::string streetTitle;
  std::string streetName;
  std::string number;
  std::string neighborhood;
  std::string postalCode;

  GetAddressConfigToSettings(streetType, streetTitle, streetName, number, neighborhood, postalCode);

  if(streetType != "")
    m_associatedProps.push_back(streetType);

  if(streetTitle != "")
    m_associatedProps.push_back(streetTitle);

  if(streetName != "")
    m_associatedProps.push_back(streetName);

  if(neighborhood != "")
    m_associatedProps.push_back(neighborhood);

  if(postalCode != "")
    m_associatedProps.push_back(postalCode);

  m_streetNumber = number;

}

te::addressgeocoding::MainWindowDialog::~MainWindowDialog()
{
}

void te::addressgeocoding::MainWindowDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    std::vector<te::dt::Property*> properties = dsType->getProperties();
    for(std::size_t i = 0; i < properties.size(); ++i)
    {
      std::string name = properties[i]->getName();
      if(name == "tsvector")
        m_ui->m_inputLayerComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    }
      
    ++it;
  }
}

te::map::AbstractLayerPtr te::addressgeocoding::MainWindowDialog::getLayer()
{
  return m_resultLayer;
}

void te::addressgeocoding::MainWindowDialog::onLayerComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  
  std::string layerID = m_ui->m_inputLayerComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  
  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_selectedLayer = selectedLayer;

      std::auto_ptr<te::da::DataSetType> dsType = m_selectedLayer->getSchema();
      std::vector<te::dt::Property*> propVec = dsType->getProperties();

      m_ui->m_iLeftComboBox->clear();
      m_ui->m_fLeftComboBox->clear();
      m_ui->m_iRightComboBox->clear();
      m_ui->m_fRightComboBox->clear();

      m_ui->m_iLeftComboBox->addItem("");
      m_ui->m_fLeftComboBox->addItem("");
      m_ui->m_iRightComboBox->addItem("");
      m_ui->m_fRightComboBox->addItem("");

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

      return;
    }
    ++it;
  }
}

void te::addressgeocoding::MainWindowDialog::onInitialLeftComboBoxChanged(int index)
{
  m_initialLeft = m_ui->m_iLeftComboBox->itemText(index).toStdString();
}

void te::addressgeocoding::MainWindowDialog::onFinalLeftComboBoxChanged(int index)
{
  m_finalLeft = m_ui->m_fLeftComboBox->itemText(index).toStdString();
}

void te::addressgeocoding::MainWindowDialog::onInitialRightComboBoxChanged(int index)
{
  m_initialRight = m_ui->m_iRightComboBox->itemText(index).toStdString();
}

void te::addressgeocoding::MainWindowDialog::onFinalRightComboBoxChanged(int index)
{
  m_finalRight = m_ui->m_fRightComboBox->itemText(index).toStdString();
}

void te::addressgeocoding::MainWindowDialog::onConfigureLayerClicked()
{
  te::addressgeocoding::ConfigInputLayerDialog dlg(this);
  dlg.setLayers(m_layers);

  if(dlg.exec() != QDialog::Accepted)
  {
     m_newColumnLayer = dlg.getLayer();
     m_layerDataSource = dlg.getDataSource();
     m_ui->m_inputLayerComboBox->clear();
     setLayers(m_layers);
  }
  return;
}

void te::addressgeocoding::MainWindowDialog::onAddressComboBoxChanged(int index)
{

}

void te::addressgeocoding::MainWindowDialog::onTargetFileAddressToolButtonPressed()
{
try
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Textual File"), te::qt::widgets::GetFilePathFromSettings("tabular"), tr("dBASE (*.dbf *.DBF);; Comma Separated Value (*.csv *.CSV)"), 
      0, QFileDialog::ReadOnly);

    GetDataSourceAddress(fileName.toStdString());

    te::addressgeocoding::ConfigInputAddressDialog dlg(this);
    
    if(m_addressDataSource)
      dlg.setDataSource(m_addressDataSource);

    if(dlg.exec() != QDialog::Accepted)
    {
      m_ui->m_inputAddressComboBox->clear();

      m_addressDataSource = dlg.getDataSource();
      m_addressFile = dlg.getAddressFileName();

      if(dlg.getStreetType() != "")
        m_associatedProps.push_back(dlg.getStreetType());

      if(dlg.getStreetTitle() != "")
        m_associatedProps.push_back(dlg.getStreetTitle());

      if(dlg.getStreetName() != "")
        m_associatedProps.push_back(dlg.getStreetName());

      if(dlg.getStreetNeighborhood() != "")
        m_associatedProps.push_back(dlg.getStreetNeighborhood());

      if(dlg.getStreetPostalCode() != "")
        m_associatedProps.push_back(dlg.getStreetPostalCode());

      m_streetNumber = dlg.getStreetNumber();

      m_ui->m_inputAddressComboBox->addItem(QString(m_addressFile.c_str()));
    }
    return;
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Address Geocoding"), e.what());
  }
}

void te::addressgeocoding::MainWindowDialog::onEditAddressToolButtonPressed()
{

}

void te::addressgeocoding::MainWindowDialog::onRemoveAddressToolButtonPressed()
{

}

void te::addressgeocoding::MainWindowDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
  
  m_toFile = false;
}

void te::addressgeocoding::MainWindowDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                        QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;
  
  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());
  
  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::addressgeocoding::MainWindowDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::addressgeocoding::MainWindowDialog::onOkPushButtonClicked()
{
  if(!m_selectedLayer)
  {
    QMessageBox::information(this, "Address Geocoding", "Configure an input layer.");
    return;
  }

  if((m_initialLeft == "" || m_finalLeft == "")&&(m_initialRight == "" || m_finalRight == ""))
  {
    QMessageBox::information(this, "Address Geocoding", "Associates the number fields with attributes of input layer.");
    return;
  }

  if(m_addressFile == "")
  {
    QMessageBox::information(this, "Address Geocoding", "Configure an input address.");
    return;
  }

  if(!m_layerDataSource)
  {
    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_selectedLayer.get());
    m_layerDataSource = te::da::GetDataSource(dsLayer->getDataSourceId());
  }

  te::addressgeocoding::AddressGeocodingOp* addGeoOp = new te::addressgeocoding::AddressGeocodingOp();

  addGeoOp->setInput( m_layerDataSource,
                      m_selectedLayer->getTitle(),
                      m_selectedLayer->getSRID(),
                      m_addressDataSource,
                      m_addressFile);

  addGeoOp->setParams(m_associatedProps,
                      m_streetNumber);

  addGeoOp->setNumAttributes( m_initialLeft,
                              m_finalLeft,
                              m_initialRight,
                              m_finalRight);

  std::string outputdataset;
  
  te::da::DataSourcePtr outputDataSource;
  bool res;
  
  try
  {
    if(m_toFile)
    {
      outputdataset = m_ui->m_repositoryLineEdit->text().toStdString();
      boost::filesystem::path uri(outputdataset);

      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Address Geocoding", "Output file already exists. Remove it or select a new name and try again.");
        return;
      }

      std::size_t idx = outputdataset.find(".");
      if (idx != std::string::npos)
        outputdataset=outputdataset.substr(0,idx);

      std::map<std::string, std::string> dsinfo;
      dsinfo["URI"] = uri.string();
      
      outputDataSource.reset(te::da::DataSourceFactory::make("OGR").release());
      outputDataSource->setConnectionInfo(dsinfo);
      outputDataSource->open();
      if (outputDataSource->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Address Geocoding", "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
        return;
      }

      this->setCursor(Qt::WaitCursor);

      addGeoOp->setOutput(outputDataSource,
                          outputdataset);

      if (!addGeoOp->paramsAreValid())
        res = false;
      else
        res = addGeoOp->run();

      if (!res)
      {
        this->setCursor(Qt::ArrowCursor);
        outputDataSource->close();
        QMessageBox::information(this, "Address Geocoding", "Error: could not generate the address geocoding.");
        reject();
      }
      outputDataSource->close();

      delete addGeoOp;

// let's include the new datasource in the managers
      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id_ds = boost::uuids::to_string(u);
      
      te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
      ds->setConnInfo(dsinfo);
      ds->setTitle(uri.stem().string());
      ds->setAccessDriver("OGR");
      ds->setType("OGR");
      ds->setDescription(uri.string());
      ds->setId(id_ds);
      
      te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id_ds, "OGR", ds->getConnInfo());
      newds->open();
      te::da::DataSourceInfoManager::getInstance().add(ds);
      m_outputDatasource = ds;

    }
    else
    {
      outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
      outputDataSource = te::da::GetDataSource(m_outputDatasource->getId());
      if (!outputDataSource)
      {
        QMessageBox::information(this, "Address Geocoding", "The selected output datasource can not be accessed.");
        return;
      }
      
      if (outputDataSource->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Address Geocoding", "Dataset already exists. Remove it or select a new name and try again.");
        return;
      }
      this->setCursor(Qt::WaitCursor);

      addGeoOp->setOutput(outputDataSource,
                          outputdataset);

      if (!addGeoOp->paramsAreValid())
        res = false;
      else
        res = addGeoOp->run();

      delete addGeoOp;

      if (!res)
      {
        this->setCursor(Qt::ArrowCursor);
        outputDataSource->close();
        QMessageBox::information(this, "Address Geocoding", "Error: could not generate the address geocoding.");
        reject();
      }
      outputDataSource->close();

    }

  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);

    QMessageBox::information(this, "Address Geocoding", e.what());
    
    //te::common::Logger::logDebug("addressgeocoding", e.what());
    //te::common::ProgressManager::getInstance().removeViewer(id);
    return;
  }

  this->setCursor(Qt::ArrowCursor);

  accept();
}

void te::addressgeocoding::MainWindowDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::addressgeocoding::MainWindowDialog::GetAddressFilePathToSettings(std::string& filePath)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  std::string key1 = "Address Geocoding/filePath";

  filePath = sett.value(key1.c_str()).toString().toStdString();
}

void te::addressgeocoding::MainWindowDialog::GetAddressConfigToSettings(std::string& streetType,
                                                                        std::string& streetTitle,
                                                                        std::string& streetName,
                                                                        std::string& number,
                                                                        std::string& neighborhood,
                                                                        std::string& postalCode)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  std::string key1 = "Address Geocoding/streetType";
  std::string key2 = "Address Geocoding/streetTitle";
  std::string key3 = "Address Geocoding/streetName";
  std::string key4 = "Address Geocoding/number";
  std::string key5 = "Address Geocoding/neighborhood";
  std::string key6 = "Address Geocoding/postalCode";

  streetType = sett.value(key1.c_str()).toString().toStdString();
  streetTitle = sett.value(key2.c_str()).toString().toStdString();
  streetName = sett.value(key3.c_str()).toString().toStdString();
  number = sett.value(key4.c_str()).toString().toStdString();
  neighborhood = sett.value(key5.c_str()).toString().toStdString();
  postalCode = sett.value(key6.c_str()).toString().toStdString();

}

void te::addressgeocoding::MainWindowDialog::GetDataSourceAddress(std::string filePath)
{
  if(QString(filePath.c_str()).isEmpty())
    return;

  QFileInfo info(QString(filePath.c_str()));
  te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "tabular");

  //Getting the connection info
  std::string ogrInfo("connection_string=" + filePath);
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = filePath;

  boost::filesystem::path uri(filePath);
  std::string file = uri.stem().string();

  //Creating a DataSource
  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();

  te::da::DataSourceInfoPtr dsInfo(new te::da::DataSourceInfo);
  dsInfo->setConnInfo(connInfo);
  dsInfo->setId(boost::uuids::to_string(u));
  dsInfo->setTitle(filePath);
  dsInfo->setDescription("");
  dsInfo->setAccessDriver("OGR");
  dsInfo->setType("OGR");

  te::da::DataSourceInfoManager::getInstance().add(dsInfo);

  m_addressDataSource = te::da::DataSourceFactory::make(dsInfo->getAccessDriver());
  m_addressDataSource->setConnectionInfo(dsInfo->getConnInfo());

  m_addressDataSource->setId(boost::uuids::to_string(u));
  m_addressDataSource->open();
}