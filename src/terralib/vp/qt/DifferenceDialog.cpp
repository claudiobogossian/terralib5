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
  \file terralib/vp/qt/DifferenceDialog.cpp

  \brief Difference operation dialog.
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"

#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"

#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"

#include "../../dataaccess/query/From.h"
#include "../../dataaccess/query/FromItem.h"

#include "../../dataaccess/utils/Utils.h"

#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"

#include "../../maptools/QueryLayer.h"

#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"

#include "../../srs/Config.h"

#include "../Exception.h"
#include "../Difference.h"
#include "../Utils.h"

#include "DifferenceDialog.h"
#include "ui_DifferenceDialogForm.h"
#include "Utils.h"

// Qt
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QTreeWidget>

// BOOST
#include <boost/filesystem.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::vp::DifferenceDialog::DifferenceDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::DifferenceDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>())
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-difference-hint").pixmap(48,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  //add double list widget to this form
  m_doubleListWidget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_attributesGroupBox));
  m_doubleListWidget->setLeftLabel("");
  m_doubleListWidget->setRightLabel("");

  QGridLayout* layout = new QGridLayout(m_ui->m_attributesGroupBox);
  layout->addWidget(m_doubleListWidget.get());
  layout->setContentsMargins(0, 0, 0, 0);

  connect(m_ui->m_inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputLayerComboBoxChanged(int)));
  connect(m_ui->m_differenceLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDifferenceLayerComboBoxChanged(int)));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/vp/vp_difference.html");
}

te::vp::DifferenceDialog::~DifferenceDialog()
{
}

void te::vp::DifferenceDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while (it != layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if (dsType->hasGeom())
    {
      m_layers.push_back(*it);
    }

    ++it;
  }

  updateInputLayerComboBox();

  updateDifferenceLayerComboBox();

  updateDoubleListWidget();
}

te::map::AbstractLayerPtr te::vp::DifferenceDialog::getLayer()
{
  return m_layerResult;
}

std::vector<std::pair<std::string, std::string> > te::vp::DifferenceDialog::getSelectedProperties()
{
  std::vector<std::string> outVec = m_doubleListWidget->getOutputValues();
  std::vector<std::pair<std::string, std::string> > result;

  for (std::size_t i = 0; i < outVec.size(); ++i)
  {
    std::vector<std::string> tok;
    te::common::Tokenize(outVec[i], tok, ": ");

    std::pair<std::string, std::string> p;
    p.first = tok[0];
    p.second = tok[1];

    result.push_back(p);
  }

  return result;
}

void te::vp::DifferenceDialog::updateInputLayerComboBox()
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  disconnect(m_ui->m_inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputLayerComboBoxChanged(int)));

  while (it != m_layers.end())
  {
    m_ui->m_inputLayerComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant::fromValue(*it));
    ++it;
  }

  connect(m_ui->m_inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputLayerComboBoxChanged(int)));

  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_inputSelectedLayer = layer;
}

void te::vp::DifferenceDialog::updateDifferenceLayerComboBox()
{
  int currIndex = m_ui->m_inputLayerComboBox->currentIndex();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  disconnect(m_ui->m_differenceLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDifferenceLayerComboBoxChanged(int)));

  while (it != m_layers.end())
  {
    m_ui->m_differenceLayerComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant::fromValue(*it));
    ++it;
  }

  m_ui->m_differenceLayerComboBox->removeItem(currIndex);

  connect(m_ui->m_differenceLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDifferenceLayerComboBoxChanged(int)));

  QVariant varLayer = m_ui->m_differenceLayerComboBox->itemData(m_ui->m_differenceLayerComboBox->currentIndex(), Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_differenceSelectedLayer = layer;
}

void te::vp::DifferenceDialog::updateDoubleListWidget()
{
  std::vector<std::string> inputValues;

  std::auto_ptr<te::da::DataSetType> inputSchema;

  if (m_inputSelectedLayer)
    inputSchema = m_inputSelectedLayer->getSchema();
  else
    return;

  std::vector<te::dt::Property*> inputProps = inputSchema->getProperties();
  for (std::size_t i = 0; i < inputProps.size(); ++i)
  {
    if (inputProps[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      std::string name = inputSchema->getTitle();

      if (name.empty())
        name = inputSchema->getName();

      inputValues.push_back(name + ": " + inputProps[i]->getName());
    }
  }

  m_doubleListWidget->setInputValues(inputValues);
}

void te::vp::DifferenceDialog::onInputLayerComboBoxChanged(int index)
{
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_ui->m_differenceLayerComboBox->clear();
  m_doubleListWidget->clearOutputValues();
  
  m_inputSelectedLayer = layer;

  updateDifferenceLayerComboBox();

  updateDoubleListWidget();
}

void te::vp::DifferenceDialog::onDifferenceLayerComboBoxChanged(int index)
{
  QVariant varLayer = m_ui->m_differenceLayerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_differenceSelectedLayer = layer;
}

void te::vp::DifferenceDialog::onOkPushButtonClicked()
{
// Validate Input Layer
  if (m_ui->m_inputLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Difference"), TE_TR("Select an input layer."));
    return;
  }

  const te::da::ObjectIdSet* inputOidSet = 0;
  if (m_ui->m_inputOnlySelectedCheckBox->isChecked())
  {
    inputOidSet = m_inputSelectedLayer->getSelected();
    if(!inputOidSet)
    {
      QMessageBox::information(this, "Difference", "Select the layer objects to perform the difference operation.");
      return;
    }
  }

  te::da::DataSourcePtr inputDataSource = te::da::GetDataSource(m_inputSelectedLayer->getDataSourceId(), true);
  if (!inputDataSource.get())
  {
    QMessageBox::information(this, "Difference", "The selected input data source can not be accessed.");
    return;
  }


// Validate Difference Layer
  if (m_ui->m_differenceLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Difference"), TE_TR("Select a layer to do the difference."));
    return;
  }

  const te::da::ObjectIdSet* differenceOidSet = 0;
  if (m_ui->m_diffOnlySelectedCheckBox->isChecked())
  {
    differenceOidSet = m_differenceSelectedLayer->getSelected();
    if(!differenceOidSet)
    {
      QMessageBox::information(this, "Difference", "Select the layer objects to perform the difference operation.");
      return;
    }
  }

  te::da::DataSourcePtr differenceDataSource = te::da::GetDataSource(m_differenceSelectedLayer->getDataSourceId(), true);
  if (!differenceDataSource.get())
  {
    QMessageBox::information(this, "Difference", "The selected difference data source can not be accessed.");
    return;
  }


// Get output attributes.
  std::vector<std::pair<std::string, std::string> > attributesVec = this->getSelectedProperties();
  std::map<std::string, te::dt::AbstractData*> specificParams;

  for (std::size_t attPos = 0; attPos < attributesVec.size(); ++attPos)
  {
    specificParams.insert(std::pair<std::string, te::dt::AbstractData*>(
      boost::lexical_cast<std::string>(attPos),
      new te::dt::SimpleData<std::string, te::dt::STRING_TYPE>(attributesVec[attPos].second)));
  }


// Validade output repository.
  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Difference"), TE_TR("Select a repository for the resulting layer."));
    return;
  }
  
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Difference"), TE_TR("Define a name for the resulting layer."));
    return;
  }
  
  if ((m_inputSelectedLayer->getSRID() == TE_UNKNOWN_SRS && m_differenceSelectedLayer->getSRID() != TE_UNKNOWN_SRS) ||
      (m_inputSelectedLayer->getSRID() != TE_UNKNOWN_SRS && m_differenceSelectedLayer->getSRID() == TE_UNKNOWN_SRS))
  {
    int ret = QMessageBox::question(this, "Difference", "The two layers have incompatible SRS. The result might be incorrect. Do you wish to continue?", QMessageBox::No, QMessageBox::Yes);
    if (ret == QMessageBox::No)
      return;
  }

// Do a Cast from AbstractLayerPtr to DataSetLayer or QueryLayer.
  te::map::DataSetLayer* inputDataSetLayer = 0;
  te::map::QueryLayer* inputQueryLayer = 0;

  te::map::DataSetLayer* differenceDataSetLayer = 0;
  te::map::QueryLayer* differenceQueryLayer = 0;

  if (m_inputSelectedLayer->getType() == "DATASETLAYER")
  {
    inputDataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputSelectedLayer.get());
    if (!inputDataSetLayer)
    {
      QMessageBox::information(this, "Difference", "Can not execute this operation on this type of input layer.");
      return;
    }
  }
  else if (m_inputSelectedLayer->getType() == "QUERYLAYER")
  {
    inputQueryLayer = dynamic_cast<te::map::QueryLayer*>(m_inputSelectedLayer.get());
    if (!inputQueryLayer)
    {
      QMessageBox::information(this, "Difference", "Can not execute this operation on this type of input layer.");
      return;
    }
  }
  else
  {
    QMessageBox::information(this, "Difference", "Can not execute this operation on this type of input layer.");
    return;
  }

  if (m_differenceSelectedLayer->getType() == "DATASETLAYER")
  {
    differenceDataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_differenceSelectedLayer.get());
    if (!differenceDataSetLayer)
    {
      QMessageBox::information(this, "Difference", "Can not execute this operation on this type of input layer.");
      return;
    }
  }
  else if (m_differenceSelectedLayer->getType() == "QUERYLAYER")
  {
    differenceQueryLayer = dynamic_cast<te::map::QueryLayer*>(m_differenceSelectedLayer.get());
    if (!differenceQueryLayer)
    {
      QMessageBox::information(this, "Difference", "Can not execute this operation on this type of input layer.");
      return;
    }
  }
  else
  {
    QMessageBox::information(this, "Difference", "Can not execute this operation on this type of difference layer.");
    return;
  }

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);
  
  try
  {
// Get the output dataset name.
    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

// Set the input parameters
    te::vp::InputParams structInputParams1;
    structInputParams1.m_inputDataSource = inputDataSource;
    structInputParams1.m_inputDataSetName = m_inputSelectedLayer->getDataSetName();

    if (inputQueryLayer)
    {
      structInputParams1.m_inputRestriction = inputQueryLayer->getQuery();

      std::auto_ptr<te::map::LayerSchema> layerSchema(inputQueryLayer->getSchema());
      structInputParams1.m_inputDataSetType = layerSchema.release();
    }

    m_inputParams.push_back(structInputParams1);


    te::vp::InputParams structInputParams2;
    structInputParams2.m_inputDataSource = differenceDataSource;
    structInputParams2.m_inputDataSetName = m_differenceSelectedLayer->getDataSetName();

    if (differenceQueryLayer)
    {
      structInputParams2.m_inputRestriction = differenceQueryLayer->getQuery();

      std::auto_ptr<te::map::LayerSchema> layerSchema(differenceQueryLayer->getSchema());
      structInputParams2.m_inputDataSetType = layerSchema.release();
    }

    m_inputParams.push_back(structInputParams2);


    bool res = true;
    if (m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());
      
      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Difference", "Output file already exists. Remove it and try again. ");

        te::common::ProgressManager::getInstance().removeViewer(id);

        return;
      }

      std::size_t idx = outputdataset.find(".");
      if(idx != std::string::npos)
        outputdataset = outputdataset.substr(0, idx);

      te::da::DataSourcePtr dsOGR = te::vp::CreateOGRDataSource(m_ui->m_repositoryLineEdit->text().toStdString());

      dsOGR->open();

      //std::auto_ptr<te::da::DataSetTypeConverter> inputConverter(new te::da::DataSetTypeConverter(m_inputSelectedLayer->getSchema().get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));
      //te::da::AssociateDataSetTypeConverterSRID(inputConverter.get(), m_inputSelectedLayer->getSRID());

      //std::auto_ptr<te::da::DataSetTypeConverter> differenceConverter(new te::da::DataSetTypeConverter(m_differenceSelectedLayer->getSchema().get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));
      //te::da::AssociateDataSetTypeConverterSRID(differenceConverter.get(), m_inputSelectedLayer->getSRID());

      this->setCursor(Qt::WaitCursor);

// Set parameters (Input/Output).
      m_params = new te::vp::AlgorithmParams();
      m_params->setInputParams(m_inputParams);
      m_params->setOutputDataSource(dsOGR);
      m_params->setOutputDataSetName(outputdataset);
      m_params->setSpecificParams(specificParams);

      te::vp::Difference difference;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities inputDSCapabilities = inputDataSource->getCapabilities();
      const te::da::DataSourceCapabilities differenceDSCapabilities = differenceDataSource->getCapabilities();

      if ((inputDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() &&
          differenceDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect()) &&
          (inputDataSource->getId() == differenceDataSource->getId()) &&
          (m_inputSelectedLayer->getSRID() == m_differenceSelectedLayer->getSRID()))
      {
        res = difference.executeQuery(m_params);
      }
      else
      {
        QMessageBox::information(this, "Difference", "This operation was not implemented to do in memory yet. Use Data Exchanger to export data in to a database.");

        te::common::ProgressManager::getInstance().removeViewer(id);
      }

      if(!res)
      {
        dsOGR->close();

        QMessageBox::information(this, "Difference", "Error: could not generate the difference.");

        te::common::ProgressManager::getInstance().removeViewer(id);

        reject();
      }

      m_outputDatasource = te::da::DataSourceInfoManager::getInstance().get(dsOGR->getId());

      delete m_params;
    }
    else
    {
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux.get())
      {
        QMessageBox::information(this, "Difference", "The output data source can not be accessed.");
        
        te::common::ProgressManager::getInstance().removeViewer(id);
        
        return;
      }
      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Difference", "Dataset already exists. Remove it or select a new name and try again. ");

        te::common::ProgressManager::getInstance().removeViewer(id);

        return;
      }

      /*std::auto_ptr<te::da::DataSetTypeConverter> inputConverter(new te::da::DataSetTypeConverter(m_inputSelectedLayer->getSchema().get(), aux->getCapabilities(), aux->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(inputConverter.get(), m_inputSelectedLayer->getSRID());

      std::auto_ptr<te::da::DataSetTypeConverter> differenceConverter(new te::da::DataSetTypeConverter(m_differenceSelectedLayer->getSchema().get(), aux->getCapabilities(), aux->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(differenceConverter.get(), m_inputSelectedLayer->getSRID());*/
      
      this->setCursor(Qt::WaitCursor);

// Set parameters (Input/Output).
      m_params = new te::vp::AlgorithmParams();
      m_params->setInputParams(m_inputParams);
      m_params->setOutputDataSource(aux);
      m_params->setOutputDataSetName(outputdataset);
      m_params->setSpecificParams(specificParams);

      te::vp::Difference difference;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities inputDSCapabilities = inputDataSource->getCapabilities();
      const te::da::DataSourceCapabilities differenceDSCapabilities = differenceDataSource->getCapabilities();

      if( inputDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          differenceDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          (inputDataSource->getId() == differenceDataSource->getId()) &&
          (m_inputSelectedLayer->getSRID() == m_differenceSelectedLayer->getSRID()))
      {
        res = difference.executeQuery(m_params);
      }
      else
      {
        QMessageBox::information(this, "Difference", "This operation was not implemented to do in memory yet. Use Data Exchanger to export data in to a database.");

        te::common::ProgressManager::getInstance().removeViewer(id);

        //res = difference.executeMemory(m_params);
      }

      delete m_params;

      if(!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Difference", "Error: could not generate the difference.");
        reject();
      }
    }

    // creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    
    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
      
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_layerResult = converter(dt);
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);
    QMessageBox::warning(this, TE_TR("Difference"), e.what());

#ifdef TERRALIB_LOGGER_ENABLED
    std::string str = "Difference - ";
    str += e.what();
    te::common::Logger::logDebug("vp", str.c_str());
#endif //TERRALIB_LOGGER_ENABLED

    te::common::ProgressManager::getInstance().removeViewer(id);
    return;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);
  accept();
}

void te::vp::DifferenceDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::vp::DifferenceDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.empty())
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;

  m_toFile = false;
}

void te::vp::DifferenceDialog::onTargetFileToolButtonPressed()
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