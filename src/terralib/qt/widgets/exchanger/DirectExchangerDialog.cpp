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
  \file terralib/qt/widgets/exchanger/DirectExchangerDialog.h

  \brief A direct exchanger dialog for ADO, POSTGIS and SHP data sources
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/DataSetLayer.h"
#include "DirectExchangerDialog.h"
#include "ui_DirectExchangerDialogForm.h"

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);
Q_DECLARE_METATYPE(te::da::DataSourceInfoPtr);

te::qt::widgets::DirectExchangerDialog::DirectExchangerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::DirectExchangerDialogForm)
{
// setup widget
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("data-exchange-direct-hint").pixmap(112,48));

  m_ui->m_inputPGISToolButton->setIcon(QIcon::fromTheme("datasource-postgis"));
  m_ui->m_inputADOToolButton->setIcon(QIcon::fromTheme("datasource-ado"));
  m_ui->m_inputSHPToolButton->setIcon(QIcon::fromTheme("datasource-ogr"));
  m_ui->m_outputPGISToolButton->setIcon(QIcon::fromTheme("datasource-postgis"));
  m_ui->m_outputADOToolButton->setIcon(QIcon::fromTheme("datasource-ado"));
  m_ui->m_outputSHPToolButton->setIcon(QIcon::fromTheme("datasource-ogr"));

//connectors
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_dirToolButton, SIGNAL(clicked()), this, SLOT(onDirToolButtonClicked()));
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(QString)), this, SLOT(onInputLayerActivated(QString)));
  connect(m_ui->m_inputPGISToolButton, SIGNAL(clicked(bool)), this, SLOT(onInputPostGISToolButtonClicked(bool)));
  connect(m_ui->m_inputADOToolButton, SIGNAL(clicked(bool)), this, SLOT(onInputADOToolButtonClicked(bool)));
  connect(m_ui->m_inputSHPToolButton, SIGNAL(clicked(bool)), this, SLOT(onInputSHPToolButtonClicked(bool)));
  connect(m_ui->m_outputPGISToolButton, SIGNAL(clicked(bool)), this, SLOT(onOutputPostGISToolButtonClicked(bool)));
  connect(m_ui->m_outputADOToolButton, SIGNAL(clicked(bool)), this, SLOT(onOutputADOToolButtonClicked(bool)));
  connect(m_ui->m_outputSHPToolButton, SIGNAL(clicked(bool)), this, SLOT(onOutputSHPToolButtonClicked(bool)));

  //starup interface
  m_inputDataSourceType = "";
  m_outputDataSourceType = "";
  m_outputDataDriver = "";
  m_exchangeToFile = false;
}

te::qt::widgets::DirectExchangerDialog::~DirectExchangerDialog()
{
}

void te::qt::widgets::DirectExchangerDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
}

void te::qt::widgets::DirectExchangerDialog::setDataSources()
{
  m_ui->m_outputDataSourceComboBox->clear();

  std::vector<te::da::DataSourceInfoPtr> datasources;

  te::da::DataSourceInfoManager::getInstance().getByType(m_outputDataSourceType, datasources);

  for(std::size_t i = 0; i < datasources.size(); ++i)
  {
    const te::da::DataSourceInfoPtr& datasource = datasources[i];

    if(datasource.get() == 0)
      continue;

    const std::string& title = datasource->getTitle();

    m_ui->m_outputDataSourceComboBox->addItem(title.c_str(), QVariant::fromValue(datasource));
  }
}

void te::qt::widgets::DirectExchangerDialog::setInputLayers()
{
  m_ui->m_inputLayerComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

    if(!dsLayer)
      continue;

    std::string dsName = dsLayer->getDataSourceId();

    te::da::DataSourcePtr dsPtr = te::da::GetDataSource(dsName);

    if(dsPtr->getType() == m_inputDataSourceType)
      m_ui->m_inputLayerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

  if(m_ui->m_inputLayerComboBox->count() > 0 && m_ui->m_dataSetLineEdit->isEnabled())
  {
    QString s = m_ui->m_inputLayerComboBox->currentText();

    m_ui->m_dataSetLineEdit->setText(s);
  }
}

bool te::qt::widgets::DirectExchangerDialog::exchangeToFile()
{
  int idxLayer = m_ui->m_inputLayerComboBox->currentIndex();

  if(idxLayer == -1)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Input layer not selected."));
    return false;
  }

  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(idxLayer, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  if(!layer.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error getting selected layer."));
    return false;
  }

  if(m_ui->m_dataSetLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Output File Name not defined."));
    return false;
  }

  try
  {
    //create adapter
    std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();
    
    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());

    if(dsType->size() == 0)
      te::da::LoadProperties(dsType.get(), dsLayer->getDataSourceId());

    //create data source
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = m_ui->m_dataSetLineEdit->text().toStdString();
    connInfo["DRIVER"] = m_outputDataDriver;
  
    std::auto_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make(m_outputDataSourceType);
    dsOGR->setConnectionInfo(connInfo);
    dsOGR->open();

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), dsOGR->getCapabilities());

    te::da::DataSetType* dsTypeResult = converter->getResult();

    dsTypeResult->setName(m_ui->m_dataSetLineEdit->text().toStdString());

    //exchange
    std::map<std::string,std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = layer->getData();

    dsOGR->createDataSet(dsTypeResult, nopt);

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

    if(dataset->moveBeforeFirst())
      dsOGR->add(dsTypeResult->getName(), dsAdapter.get(), dsOGR->getConnectionInfo());

    dsOGR->close();

    QMessageBox::information(this, tr("Exchanger"), tr("Layer exported successfully."));
  }
  catch(const std::exception& e)
  {
    QString errMsg(tr("Error during exchanger. The reported error is: %1"));

    errMsg = errMsg.arg(e.what());

    QMessageBox::information(this, tr("Exchanger"), errMsg);

    return false;
  }

  return true;
}

bool te::qt::widgets::DirectExchangerDialog::exchangeToDatabase()
{
  int idxLayer = m_ui->m_inputLayerComboBox->currentIndex();

  if(idxLayer == -1)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Input layer not selected."));
    return false;
  }

  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(idxLayer, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  if(!layer.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error getting selected layer."));
    return false;
  }

  int idxDataSource = m_ui->m_outputDataSourceComboBox->currentIndex();

  if(idxLayer == -1)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Output data source not selected."));
    return false;
  }

  QVariant varDataSource = m_ui->m_outputDataSourceComboBox->itemData(idxDataSource, Qt::UserRole);
  te::da::DataSourceInfoPtr dsInfo = varDataSource.value<te::da::DataSourceInfoPtr>();

  if(!dsInfo.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error getting selected data source."));
    return false;
  }

  if(m_ui->m_dataSetLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Data Set name not defined."));
    return false;
  }

  try
  {
    //create adapter
    std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();
    
    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());

    if(dsType->size() == 0)
      te::da::LoadProperties(dsType.get(), dsLayer->getDataSourceId());

    te::da::DataSourcePtr targetDSPtr = te::da::DataSourceManager::getInstance().get(dsInfo->getId(), dsInfo->getType(), dsInfo->getConnInfo()); 

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), targetDSPtr->getCapabilities());

    te::da::DataSetType* dsTypeResult = converter->getResult();
    
    dsTypeResult->setName(m_ui->m_dataSetLineEdit->text().toStdString());

    //create index
    if(m_ui->m_spatialIndexCheckBox->isChecked())
    {
      te::gm::GeometryProperty* p = te::da::GetFirstGeomProperty(dsTypeResult);

      if(p)
      {
        te::da::Index* idx = new te::da::Index(dsTypeResult);

        std::string name = m_ui->m_dataSetLineEdit->text().toStdString() + "_" + p->getName() + "_idx";
        idx->setName(name);
        idx->setIndexType(te::da::R_TREE_TYPE);

        te::dt::Property* pClone = p->clone();

        idx->add(pClone);
      }
    }

    //exchange
    std::map<std::string,std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = layer->getData();

    targetDSPtr->createDataSet(dsTypeResult, nopt);

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

     if(dataset->moveBeforeFirst())
       targetDSPtr->add(dsTypeResult->getName(), dsAdapter.get(), targetDSPtr->getConnectionInfo());

    QMessageBox::information(this, tr("Exchanger"), tr("Layer exported successfully."));
  }
  catch(const std::exception& e)
  {
    QString errMsg(tr("Error during exchanger. The reported error is: %1"));

    errMsg = errMsg.arg(e.what());

    QMessageBox::information(this, tr("Exchanger"), errMsg);

    return false;
  }

  return true;
}

void te::qt::widgets::DirectExchangerDialog::onInputPostGISToolButtonClicked(bool flag)
{
  if(!flag)
    return;

  m_inputDataSourceType = "POSTGIS";

  m_ui->m_inputLayerComboBox->setEnabled(true);

  setInputLayers();
}

void te::qt::widgets::DirectExchangerDialog::onInputADOToolButtonClicked(bool flag)
{
  if(!flag)
    return;

  m_inputDataSourceType = "ADO";

  m_ui->m_inputLayerComboBox->setEnabled(true);

  setInputLayers();
}

void te::qt::widgets::DirectExchangerDialog::onInputSHPToolButtonClicked(bool flag)
{
  if(!flag)
    return;

  m_inputDataSourceType = "OGR";

  m_ui->m_inputLayerComboBox->setEnabled(true);

  setInputLayers();
}

void te::qt::widgets::DirectExchangerDialog::onOutputPostGISToolButtonClicked(bool flag)
{
  if(!flag)
    return;

  m_outputDataSourceType = "POSTGIS";

  m_exchangeToFile = false;
  
  m_ui->m_outputDataSourceComboBox->setEnabled(true);
  m_ui->m_dataSetLineEdit->setEnabled(true);
  m_ui->m_dirToolButton->setEnabled(false);
  m_ui->m_spatialIndexCheckBox->setEnabled(true);
  m_ui->m_spatialIndexCheckBox->setChecked(true);

  setDataSources();
}

void te::qt::widgets::DirectExchangerDialog::onOutputADOToolButtonClicked(bool flag)
{
  if(!flag)
    return;

  m_outputDataSourceType = "ADO";

  m_exchangeToFile = false;

  m_ui->m_outputDataSourceComboBox->setEnabled(true);
  m_ui->m_dataSetLineEdit->setEnabled(true);
  m_ui->m_dirToolButton->setEnabled(false);
  m_ui->m_spatialIndexCheckBox->setEnabled(false);
  m_ui->m_spatialIndexCheckBox->setChecked(false);

  setDataSources();
}

void te::qt::widgets::DirectExchangerDialog::onOutputSHPToolButtonClicked(bool flag)
{
  if(!flag)
    return;

  m_outputDataSourceType = "OGR";
  m_outputDataDriver = "ESRI Shapefile";

  m_exchangeToFile = true;

  m_ui->m_outputDataSourceComboBox->clear();
  m_ui->m_outputDataSourceComboBox->setEnabled(false);
  m_ui->m_dataSetLineEdit->clear();
  m_ui->m_dataSetLineEdit->setEnabled(false);
  m_ui->m_dirToolButton->setEnabled(true);
  m_ui->m_spatialIndexCheckBox->setEnabled(false);
  m_ui->m_spatialIndexCheckBox->setChecked(false);
}

void te::qt::widgets::DirectExchangerDialog::onInputLayerActivated(QString value)
{
  if(m_ui->m_dataSetLineEdit->isEnabled())
    m_ui->m_dataSetLineEdit->setText(value);
}

void te::qt::widgets::DirectExchangerDialog::onDirToolButtonClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                        QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);
  
  if (fileName.isEmpty())
    return;
  
  m_ui->m_dataSetLineEdit->setText(fileName);
}

void te::qt::widgets::DirectExchangerDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::qt::widgets::DirectExchangerDialog::onOkPushButtonClicked()
{
  bool res = false;

  if(m_exchangeToFile)
    res = exchangeToFile();
  else
    res = exchangeToDatabase();

  if(res)
    accept();
}
