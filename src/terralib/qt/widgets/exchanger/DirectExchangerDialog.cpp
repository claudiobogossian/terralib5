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
  \file terralib/qt/widgets/exchanger/DirectExchangerDialog.h

  \brief A direct exchanger dialog for ADO, POSTGIS and SHP data sources
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/PrimaryKey.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../srs/Config.h"
#include "../../widgets/datasource/selector/DataSourceExplorerDialog.h"
#include "../../widgets/datasource/core/DataSourceType.h"
#include "../../widgets/datasource/core/DataSourceTypeManager.h"
#include "../../widgets/srs/SRSManagerDialog.h"
#include "DirectExchangerDialog.h"
#include "ui_DirectExchangerDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
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
  m_ui->m_dsToolButton->setIcon(QIcon::fromTheme("datasource"));

//connectors
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_dirToolButton, SIGNAL(clicked()), this, SLOT(onDirToolButtonClicked()));
  connect(m_ui->m_dsToolButton, SIGNAL(clicked()), this, SLOT(onDataSoruceToolButtonClicked()));
  connect(m_ui->m_dsTypeComboBox, SIGNAL(activated(int)), this, SLOT(onDataSourceTypeActivated(int)));
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(QString)), this, SLOT(onInputLayerActivated(QString)));
  connect(m_ui->m_outputSRIDToolButton, SIGNAL(clicked()), SLOT(onOutputLayerSRSTriggered()));

  m_ui->m_outputSRIDToolButton->setIcon(QIcon::fromTheme("srs"));
  m_ui->m_helpPushButton->setPageReference("widgets/exchanger_direct/exchanger_direct.html");

  //starup interface
  m_outputDataSourceType = "";
  m_exchangeToFile = false;
  m_lastDsType = "";

  setOutputDataSources();
}

te::qt::widgets::DirectExchangerDialog::~DirectExchangerDialog()
{
}

void te::qt::widgets::DirectExchangerDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  m_ui->m_inputLayerComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

      std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

      if(dsType.get() && !dsType->hasRaster())
        m_ui->m_inputLayerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

  if(m_ui->m_inputLayerComboBox->count() > 0)
  {
    QString s = m_ui->m_inputLayerComboBox->currentText();

    onInputLayerActivated(s);
  }
  
  if(m_ui->m_inputLayerComboBox->count() > 1)
    m_ui->m_inputLayerComboBox->setEnabled(true);
}

void te::qt::widgets::DirectExchangerDialog::setLastDataSource(std::string dataSource)
{
  m_lastDsType = dataSource;
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

  //get srid information
  int inputSRID = m_ui->m_inputSRIDLineEdit->text().toInt();

  int outputSRID = m_ui->m_outputSRIDLineEdit->text().toInt();

  try
  {
    //create adapter
    std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();

    //create data source
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = m_ui->m_dataSetLineEdit->text().toStdString();

    std::auto_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make(m_outputDataSourceType);
    dsOGR->setConnectionInfo(connInfo);
    dsOGR->open();

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), dsOGR->getCapabilities(), dsOGR->getEncoding());

    te::da::AssociateDataSetTypeConverterSRID(converter, inputSRID, outputSRID);

    te::da::DataSetType* dsTypeResult = converter->getResult();

    boost::filesystem::path uri(m_ui->m_dataSetLineEdit->text().toStdString());

    std::string val = uri.stem().string();

    dsTypeResult->setName(val);

    // Check dataset name
    if(!dsOGR->isDataSetNameValid(dsTypeResult->getName()))
    {
      int r = QMessageBox::question(this, tr("Exchanger"), tr("Layer name invalid for output datasource. Would you like to normalize the name?"), QMessageBox::Yes, QMessageBox::No);

      if(r == QMessageBox::Yes)
      {
        bool aux;
        std::string newName = te::common::ReplaceSpecialChars(dsTypeResult->getName(), aux);
        dsTypeResult->setName(newName);
      }
      else
      {
        throw te::common::Exception(tr("Layer name invalid for output datasource!").toStdString());
      }
    }

    // Check properties names
    std::vector<te::dt::Property* > props = dsTypeResult->getProperties();
    std::map<std::size_t, std::string> invalidNames;
    for(std::size_t i = 0; i < props.size(); ++i)
    {
      if(!dsOGR->isPropertyNameValid(props[i]->getName()))
      {
        invalidNames[i] = props[i]->getName();
      }
    }

    if(!invalidNames.empty())
    {
      int r = QMessageBox::question(this, tr("Exchanger"), tr("Some property name is invalid for output datasource. Would you like to normalize the name?"), QMessageBox::Yes, QMessageBox::No);

      if(r == QMessageBox::Yes)
      {
        std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

        while(it != invalidNames.end())
        {
          bool aux;
          std::string newName = te::common::ReplaceSpecialChars(it->second, aux);

          props[it->first]->setName(newName);

          ++it;
        }
      }
      else
      {
        QString err(tr("Some property name is invalid for output datasource:\n\n"));

        std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

        while(it != invalidNames.end())
        {
          err.append(" - ");
          err.append(it->second.c_str());

          ++it;
        }

        throw te::common::Exception(err.toStdString());
      }
    }

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

  //get srid information
  int inputSRID = m_ui->m_inputSRIDLineEdit->text().toInt();

  int outputSRID = m_ui->m_outputSRIDLineEdit->text().toInt();

  std::auto_ptr<te::da::DataSourceTransactor> transactor;

  try
  {
    setCursor(Qt::WaitCursor);

    //create adapter
    std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();

    bool isLinked = te::da::HasLinkedTable(dsType.get());

    if (isLinked)
    {
      te::da::PrimaryKey* pk = dsType->getPrimaryKey();

      if (pk)
      {
        std::vector<te::dt::Property*> props = pk->getProperties();

        for (size_t t = 0; t < props.size(); ++t)
        {
          te::dt::SimpleProperty* p = dynamic_cast<te::dt::SimpleProperty*>(props[t]);

          if (p)
          {
            p->setRequired(false);
            p->setAutoNumber(false);
            p->setDefaultValue(0);
          }
        }

        dsType->setPrimaryKey(0);
      }
    }

    te::da::DataSourcePtr targetDSPtr = te::da::DataSourceManager::getInstance().get(dsInfo->getId(), dsInfo->getType(), dsInfo->getConnInfo()); 

    transactor = targetDSPtr->getTransactor();

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), targetDSPtr->getCapabilities(), targetDSPtr->getEncoding());

    te::da::AssociateDataSetTypeConverterSRID(converter, inputSRID, outputSRID);

    te::da::DataSetType* dsTypeResult = converter->getResult();
    
    dsTypeResult->setName(m_ui->m_dataSetLineEdit->text().toStdString());

    // Check dataset name
    if(!targetDSPtr->isDataSetNameValid(dsTypeResult->getName()))
    {
      int r = QMessageBox::question(this, tr("Exchanger"), tr("Layer name invalid for output datasource. Would you like to normalize the name?"), QMessageBox::Yes, QMessageBox::No);

      if(r == QMessageBox::Yes)
      {
        bool aux;
        std::string newName = te::common::ReplaceSpecialChars(dsTypeResult->getName(), aux);
        dsTypeResult->setName(newName);
      }
      else
      {
        throw te::common::Exception(tr("Layer name invalid for output datasource!").toStdString());
      }
    }

    // Check properties names
    std::vector<te::dt::Property* > props = dsTypeResult->getProperties();
    std::map<std::size_t, std::string> invalidNames;
    for(std::size_t i = 0; i < props.size(); ++i)
    {
      if(!targetDSPtr->isPropertyNameValid(props[i]->getName()))
      {
        invalidNames[i] = props[i]->getName();
      }
    }

    if(!invalidNames.empty())
    {
      int r = QMessageBox::question(this, tr("Exchanger"), tr("Some property name is invalid for output datasource. Would you like to normalize the name?"), QMessageBox::Yes, QMessageBox::No);

      if(r == QMessageBox::Yes)
      {
        std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

        while(it != invalidNames.end())
        {
          bool aux;
          std::string newName = te::common::ReplaceSpecialChars(it->second, aux);

          props[it->first]->setName(newName);

          ++it;
        }
      }
      else
      {
        QString err(tr("Some property name is invalid for output datasource:\n\n"));

        std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

        while(it != invalidNames.end())
        {
          err.append(" - ");
          err.append(it->second.c_str());

          ++it;
        }

        throw te::common::Exception(err.toStdString());
      }
    }

    //fix repeated names
    std::set<std::string> names;

    props = dsTypeResult->getProperties();

    for (std::size_t i = 0; i < props.size(); ++i)
    {
      //check if the property name its duplicated
      std::string propName = props[i]->getName();

      int count = 1;
      while (names.find(te::common::Convert2UCase(propName)) != names.end())
      {
        propName += "_";
        propName += te::common::Convert2String(count);
      }

      names.insert(te::common::Convert2UCase(propName));

      props[i]->setName(propName);
    }

    //create index
    if(m_ui->m_spatialIndexCheckBox->isChecked())
    {
      te::gm::GeometryProperty* p = te::da::GetFirstGeomProperty(dsTypeResult);

      if(p)
      {
        te::da::Index* idx = new te::da::Index(dsTypeResult);

        std::string name = m_ui->m_dataSetLineEdit->text().toStdString() + "_" + p->getName() + "_idx";

        boost::replace_all(name, ".", "_");

        idx->setName(name);
        idx->setIndexType(te::da::R_TREE_TYPE);

        te::dt::Property* pClone = p->clone();

        idx->add(pClone);
      }
    }

    if (!isLinked)
    {
      //create primary key
      if (dsType->getPrimaryKey())
      {
        te::da::PrimaryKey* pk = new te::da::PrimaryKey(dsTypeResult);

        std::string name = m_ui->m_dataSetLineEdit->text().toStdString() + "_" + dsType->getPrimaryKey()->getName() + "_pk";

        boost::replace_all(name, ".", "_");

        pk->setName(name);

        std::vector<te::dt::Property*> props = dsType->getPrimaryKey()->getProperties();

        for (size_t t = 0; t < props.size(); ++t)
        {
          te::dt::Property* p = props[t]->clone();

          pk->add(p);
        }
      }
    }

    //exchange
    std::map<std::string,std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = layer->getData();

    transactor->begin();

    transactor->createDataSet(dsTypeResult, nopt);

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

    if(dataset->moveBeforeFirst())
       transactor->add(dsTypeResult->getName(), dsAdapter.get(), targetDSPtr->getConnectionInfo());

    if (isLinked)
    {
      std::string name = m_ui->m_dataSetLineEdit->text().toStdString() + "_id";

      te::dt::SimpleProperty* p = new te::dt::SimpleProperty(name, te::dt::INT32_TYPE);
      p->setAutoNumber(true);

      transactor->addProperty(dsTypeResult->getName(), p);

      te::da::PrimaryKey* pk = new te::da::PrimaryKey(dsTypeResult);

      name += "_pk";

      boost::replace_all(name, ".", "_");

      pk->setName(name);

      pk->add(p);

      transactor->addPrimaryKey(dsTypeResult->getName(), pk);
    }

     transactor->commit();

     setCursor(Qt::ArrowCursor);
     QMessageBox::information(this, tr("Exchanger"), tr("Layer exported successfully."));
  }
  catch(const std::exception& e)
  {
    setCursor(Qt::ArrowCursor);

    transactor->rollBack();

    QString errMsg(tr("Error during exchanger. The reported error is: %1"));

    errMsg = errMsg.arg(e.what());

    QMessageBox::information(this, tr("Exchanger"), errMsg);

    return false;
  }

  return true;
}

void te::qt::widgets::DirectExchangerDialog::onDataSourceTypeActivated(int index)
{
  QString value = m_ui->m_dsTypeComboBox->itemData(index).toString();

  m_outputDataSourceType = value.toStdString();

  if(m_outputDataSourceType == "POSTGIS")
  {
    m_exchangeToFile = false;
  
    m_ui->m_outputDataSourceComboBox->setEnabled(true);
    m_ui->m_dsToolButton->setEnabled(true);
    m_ui->m_dataSetLineEdit->clear();
    m_ui->m_dataSetLineEdit->setEnabled(true);
    m_ui->m_dirToolButton->setEnabled(false);
    m_ui->m_spatialIndexCheckBox->setEnabled(true);
    m_ui->m_spatialIndexCheckBox->setChecked(true);

    setDataSources();
  }
  else if(m_outputDataSourceType == "ADO")
  {
    m_exchangeToFile = false;

    m_ui->m_outputDataSourceComboBox->setEnabled(true);
    m_ui->m_dsToolButton->setEnabled(true);
    m_ui->m_dataSetLineEdit->clear();
    m_ui->m_dataSetLineEdit->setEnabled(true);
    m_ui->m_dirToolButton->setEnabled(false);
    m_ui->m_spatialIndexCheckBox->setEnabled(false);
    m_ui->m_spatialIndexCheckBox->setChecked(false);

    setDataSources();
  }
  else if(m_outputDataSourceType == "OGR")
  {
    m_exchangeToFile = true;

    m_ui->m_outputDataSourceComboBox->clear();
    m_ui->m_outputDataSourceComboBox->setEnabled(false);
    m_ui->m_dsToolButton->setEnabled(false);
    m_ui->m_dataSetLineEdit->clear();
    m_ui->m_dataSetLineEdit->setEnabled(false);
    m_ui->m_dirToolButton->setEnabled(true);
    m_ui->m_spatialIndexCheckBox->setEnabled(false);
    m_ui->m_spatialIndexCheckBox->setChecked(false);
  }
}

void te::qt::widgets::DirectExchangerDialog::onInputLayerActivated(QString value)
{
  onDataSourceTypeActivated(m_ui->m_dsTypeComboBox->currentIndex());

  if(m_ui->m_dataSetLineEdit->isEnabled())
    m_ui->m_dataSetLineEdit->setText(value);

  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  if (!layer.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error getting selected layer."));
    return;
  }

  int inputSRID = layer->getSRID();

  m_ui->m_inputSRIDLineEdit->setText(QString::number(inputSRID));
  m_ui->m_outputSRIDLineEdit->setText(QString::number(inputSRID));

  if (inputSRID == TE_UNKNOWN_SRS)
  {
    m_ui->m_outputSRIDLineEdit->setEnabled(false);
    m_ui->m_outputSRIDToolButton->setEnabled(false);
  }
  else
  {
    m_ui->m_outputSRIDLineEdit->setEnabled(true);
    m_ui->m_outputSRIDToolButton->setEnabled(true);
  }
}

void te::qt::widgets::DirectExchangerDialog::onDirToolButtonClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("Shapefile (*.shp *.SHP);;Mapinfo File (*.mif *.MIF);;KML (*.kml *.KML);;GeoJSON (*.geojson *.GEOJSON);;GML (*.gml *.GML);;DXF (*.dxf *.DXF);;DGN (*.dgn *.DGN);;"),0, QFileDialog::DontConfirmOverwrite);
  
  if (fileName.isEmpty())
    return;
  
  m_ui->m_dataSetLineEdit->setText(fileName);
}

void te::qt::widgets::DirectExchangerDialog::onDataSoruceToolButtonClicked()
{
  std::auto_ptr<te::qt::widgets::DataSourceExplorerDialog> dExplorer(new te::qt::widgets::DataSourceExplorerDialog(this));

  if(!m_lastDsType.empty())
    dExplorer->setDataSourceToUse(m_lastDsType.c_str());

  dExplorer->exec();

  setDataSources();
}

void te::qt::widgets::DirectExchangerDialog::onOkPushButtonClicked()
{
  if (m_ui->m_outputSRIDLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Output Layer SRID not defined."));
    return;
  }

  int inputSRID = m_ui->m_inputSRIDLineEdit->text().toInt();
  int outputSRID = m_ui->m_outputSRIDLineEdit->text().toInt();

  if (inputSRID != outputSRID && outputSRID == TE_UNKNOWN_SRS)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Invalid output Layer SRID."));
    return;
  }

  if (inputSRID != outputSRID && inputSRID == TE_UNKNOWN_SRS)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Invalid input Layer SRID."));
    return;
  }

  bool res = false;

  if(m_exchangeToFile)
    res = exchangeToFile();
  else
    res = exchangeToDatabase();

  if(res)
    accept();
}

void te::qt::widgets::DirectExchangerDialog::setOutputDataSources()
{
  m_ui->m_dsTypeComboBox->clear();

  // add the list of data sources available in the system
  std::map<std::string, DataSourceType*>::const_iterator it = DataSourceTypeManager::getInstance().begin();
  std::map<std::string, DataSourceType*>::const_iterator itend = DataSourceTypeManager::getInstance().end();

  while (it != itend)
  {
    std::string dataSourceName = it->first;

    if (dataSourceName == "POSTGIS" || dataSourceName == "ADO" || dataSourceName == "OGR")
    {
      QIcon icon = it->second->getIcon(DataSourceType::ICON_DATASOURCE_SMALL);
      QString title = QString::fromStdString(it->second->getTitle());

      m_ui->m_dsTypeComboBox->addItem(icon, title, QVariant(dataSourceName.c_str()));
    }

    ++it;
  }
}

void te::qt::widgets::DirectExchangerDialog::onOutputLayerSRSTriggered()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if (srsDialog.exec() == QDialog::Rejected)
    return;

  std::pair<int, std::string> srid = srsDialog.getSelectedSRS();

  m_ui->m_outputSRIDLineEdit->setText(QString::number(srid.first));
}
