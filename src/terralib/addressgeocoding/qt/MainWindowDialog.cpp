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
  \file terralib/addressgeocoding/MainWindowDialog.cpp

  \brief A dialog for Address Geocoding operation
*/

// TerraLib
//#include "../../common/CharEncodingConv.h"
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
#include <QMessageBox>
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
  connect(m_ui->m_inputAddressComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onAddressComboBoxChanged(int)));

  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));

  connect(m_ui->m_configureLayerPushButton, SIGNAL(clicked()), this, SLOT(onConfigureLayerClicked()));
  connect(m_ui->m_targetFileAddressToolButton, SIGNAL(clicked()), this, SLOT(onTargetFileAddressToolButtonPressed()));

  connect(m_ui->m_removeAddressFileToolButton, SIGNAL(clicked()), this, SLOT(onRemoveAddressToolButtonPressed()));
  connect(m_ui->m_editAddressFileToolButton, SIGNAL(clicked()), this, SLOT(onEditAddressToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins");
  m_ui->m_helpPushButton->setPageReference("plugins/addressgeocoding/addressgeocoding.html");

// Load the Address file Name.
  GetAddressFilePathToSettings(m_mapFields);
  std::map<std::string, std::vector<std::string> >::iterator it = m_mapFields.begin();

  while(it != m_mapFields.end())
  {
    m_ui->m_inputAddressComboBox->addItem(QString(it->first.c_str()));
    ++it;
  }
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

  if(dlg.exec() == QDialog::Accepted)
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
  // Load the Address file Name.

  if(index == -1)
  {
    m_ui->m_editAddressFileToolButton->setEnabled(false);
    m_ui->m_removeAddressFileToolButton->setEnabled(false);
    return;
  }

  GetAddressFilePathToSettings(m_mapFields);

  std::map<std::string, std::vector<std::string> >::iterator it;

  std::string value = m_ui->m_inputAddressComboBox->itemText(index).toStdString();

  it = m_mapFields.find(value);
  m_addressFile = it->first;
  std::vector<std::string> vecFields = it->second;

  m_associatedProps.clear();

  for(std::size_t i = 0; i < vecFields.size(); ++i)
  {
    if(i != 0 && i != 2 && vecFields[i] != "")
      m_associatedProps.push_back(vecFields[i]);
    if(i == 0)
      GetAddressDataSource(vecFields[i]);
    if(i == 2)
      m_streetNumber = m_mapFields.begin()->second[i];
  }

  m_ui->m_editAddressFileToolButton->setEnabled(true);
  m_ui->m_removeAddressFileToolButton->setEnabled(true);
}

void te::addressgeocoding::MainWindowDialog::onTargetFileAddressToolButtonPressed()
{
  try
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Textual File"), te::qt::widgets::GetFilePathFromSettings("tabular"), tr("dBASE (*.dbf *.DBF);; Comma Separated Value (*.csv *.CSV)"), 
      0, QFileDialog::ReadOnly);

    GetAddressDataSource(fileName.toStdString());

    if (fileName.isEmpty())
      return;

    te::addressgeocoding::ConfigInputAddressDialog dlg(this);
    
    if(m_addressDataSource)
      dlg.setDataSource(m_addressDataSource);

    if(dlg.exec() == QDialog::Accepted)
    {
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
  te::addressgeocoding::ConfigInputAddressDialog dlg(this);

  if(m_addressDataSource)
    dlg.setDataSource(m_addressDataSource);

  std::map<std::string, std::vector<std::string> > mapFields;
  GetAddressFilePathToSettings(mapFields);
  std::map<std::string, std::vector<std::string> >::iterator it;

  std::string value = m_ui->m_inputAddressComboBox->currentText().toStdString();

  it = mapFields.find(value);
  m_addressFile = it->first;
  std::vector<std::string> vecFields = it->second;

  dlg.setStreetNeighborhood(vecFields[1]);
  dlg.setStreetNumber(vecFields[2]);
  dlg.setStreetPostalCode(vecFields[3]);
  dlg.setStreetName(vecFields[4]);
  dlg.setStreetTitle(vecFields[5]);
  dlg.setStreetType(vecFields[6]);

  if(dlg.exec() == QDialog::Accepted)
  {
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

void te::addressgeocoding::MainWindowDialog::onRemoveAddressToolButtonPressed()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("Address Geocoding");
  
  sett.remove(m_ui->m_inputAddressComboBox->currentText());
  m_ui->m_inputAddressComboBox->removeItem(m_ui->m_inputAddressComboBox->currentIndex());

  sett.endGroup();
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

  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
  
  te::da::DataSourcePtr outputDataSource;
  bool res;
  
  try
  {
    if(m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

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

// creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    
    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
      
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_resultLayer = converter(dt);

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

void te::addressgeocoding::MainWindowDialog::GetAddressFilePathToSettings(std::map<std::string, std::vector<std::string> >& mapFields)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.beginGroup("Address Geocoding");
  QStringList lst = sett.childGroups();

  QStringList::iterator it;
  
  for(it=lst.begin(); it != lst.end(); ++it)
  {
    QString gr = *it;

    sett.beginGroup(gr);

    std::vector<std::string> vecFields;

    std::string fileName = sett.value("fileName").toString().toStdString();
    vecFields.push_back(sett.value("filePath").toString().toStdString());
    vecFields.push_back(sett.value("neighborhood").toString().toStdString());
    vecFields.push_back(sett.value("number").toString().toStdString());
    vecFields.push_back(sett.value("postalCode").toString().toStdString());
    vecFields.push_back(sett.value("streetName").toString().toStdString());
    vecFields.push_back(sett.value("streetTitle").toString().toStdString());
    vecFields.push_back(sett.value("streetType").toString().toStdString());

    mapFields.insert(std::pair<std::string, std::vector<std::string> >(fileName, vecFields));

    sett.endGroup();
  }
  sett.endGroup();
}

void te::addressgeocoding::MainWindowDialog::GetAddressDataSource(std::string filePath)
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
