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

#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"

#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"

#include "../../dataaccess/utils/Utils.h"

#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"

#include "../../geometry/GeometryProperty.h"

#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"

#include "../ComplexData.h"
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

// BOOST
#include <boost/filesystem.hpp>


Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::vp::DifferenceDialog::DifferenceDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::DifferenceDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>())
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-difference-hint").pixmap(112, 48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  //add double list widget to this form
  m_doubleListWidget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_specificParamsTabWidget->widget(0)));
  m_doubleListWidget->setLeftLabel("");
  m_doubleListWidget->setRightLabel("");

  QGridLayout* layout = new QGridLayout(m_ui->m_specificParamsTabWidget->widget(0));
  layout->addWidget(m_doubleListWidget.get());
  layout->setContentsMargins(0, 0, 0, 0);

  QSize iconSize(96, 48);

  m_ui->m_singleRadioButton->setIconSize(iconSize);
  m_ui->m_singleRadioButton->setIcon(QIcon::fromTheme("vp-single-objects"));

  m_ui->m_multiRadioButton->setIconSize(iconSize);
  m_ui->m_multiRadioButton->setIcon(QIcon::fromTheme("vp-multi-objects"));

  connect(m_ui->m_inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputLayerComboBoxChanged(int)));
  connect(m_ui->m_differenceLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDifferenceLayerComboBoxChanged(int)));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

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

std::vector<std::string> te::vp::DifferenceDialog::getSelectedProperties()
{
  std::vector<std::string> outVec = m_doubleListWidget->getOutputValues();
  std::vector<std::string> result;

  for (std::size_t i = 0; i < outVec.size(); ++i)
  {
    std::vector<std::string> tok;
    te::common::Tokenize(outVec[i], tok, ": ");

    result.push_back(tok[1]);
  }

  return result;
}

bool te::vp::DifferenceDialog::isCollection()
{
  if (m_ui->m_singleRadioButton->isChecked())
    return false;

  return true;
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

void te::vp::DifferenceDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if (dsPtrList.empty())
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
    QString(), tr("Shapefile (*.shp *.SHP);;"), 0, QFileDialog::DontConfirmOverwrite);

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

void te::vp::DifferenceDialog::onOkPushButtonClicked()
{
// Validate Input Layer.
  if (m_ui->m_inputLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Difference"), TE_TR("Select an input layer."));
    return;
  }

// Validate DataSource.
  te::da::DataSourcePtr inputDataSource = te::da::GetDataSource(m_inputSelectedLayer->getDataSourceId(), true);
  if (!inputDataSource.get())
  {
    QMessageBox::information(this, "Difference", "The selected input data source can not be accessed.");
    return;
  }


// Validate Difference Layer.
  if (m_ui->m_differenceLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TE_TR("Difference"), TE_TR("Select a layer to do the difference."));
    return;
  }

// Validate DataSource.
  te::da::DataSourcePtr differenceDataSource = te::da::GetDataSource(m_differenceSelectedLayer->getDataSourceId(), true);
  if (!differenceDataSource.get())
  {
    QMessageBox::information(this, "Difference", "The selected difference data source can not be accessed.");
    return;
  }

// Get output attributes.
  std::vector<std::string> attributesVec = this->getSelectedProperties();
  std::map<std::string, te::dt::AbstractData*> specificParams;

  if (!attributesVec.empty())
  {
    specificParams["ATTRIBUTES"] = new te::vp::ComplexData<std::vector<std::string> >(attributesVec);
  }

// Verify if the result is Single or Multi Geometry type
  specificParams["IS_COLLECTION"] = new te::dt::SimpleData<bool, te::dt::BOOLEAN_TYPE>(this->isCollection());


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

  // Verify if "Input Only Selected objects" is checked.
  bool inputIsChecked = false;

  if (m_ui->m_inputOnlySelectedCheckBox->isChecked())
    inputIsChecked = true;


  // Verify if "Difference Only Selected objects" is checked.
  bool differenceIsChecked = false;

  if (m_ui->m_diffOnlySelectedCheckBox->isChecked())
    differenceIsChecked = true;


// Progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);
  
  try
  {
// Declare the input parameters
    te::vp::InputParams structInputParams1;
    te::vp::InputParams structInputParams2;


// Set the inputLayer parameters
    structInputParams1.m_inputDataSource = inputDataSource;
    structInputParams1.m_inputDataSetType = m_inputSelectedLayer->getSchema().release();

// Set the differenceLayer parameters
    structInputParams2.m_inputDataSource = differenceDataSource;
    structInputParams2.m_inputDataSetType = m_differenceSelectedLayer->getSchema().release();


// Verify SRID
    te::gm::GeometryProperty* geomInputProp = te::da::GetFirstGeomProperty(structInputParams1.m_inputDataSetType);

    int inputSRID = 0;

    if (!geomInputProp)
    {
      QMessageBox::information(this, "Difference", "Problem to get geometry property of input layer.");

      te::common::ProgressManager::getInstance().removeViewer(id);

      return;
    }

    if (m_inputSelectedLayer->getSRID() != geomInputProp->getSRID())
      inputSRID = m_inputSelectedLayer->getSRID();


    te::gm::GeometryProperty* geomDiffProp = te::da::GetFirstGeomProperty(structInputParams2.m_inputDataSetType);

    int differenceSRID = 0;

    if (!geomDiffProp)
    {
      QMessageBox::information(this, "Difference", "Problem to get geometry property of difference layer.");

      te::common::ProgressManager::getInstance().removeViewer(id);

      return;
    }

    if (m_differenceSelectedLayer->getSRID() != geomDiffProp->getSRID())
      differenceSRID = m_differenceSelectedLayer->getSRID();


// Select a strategy based on the capabilities of the input datasource
    const te::da::DataSourceCapabilities inputDSCapabilities = inputDataSource->getCapabilities();
    const te::da::DataSourceCapabilities differenceDSCapabilities = differenceDataSource->getCapabilities();

    bool isQuery = false;

    te::da::Select* inputSelect = 0;
    te::da::Select* differenceSelect = 0;

    te::da::DataSet* inputDataSet = 0;
    te::da::DataSet* differenceDataSet = 0;

    if ((inputDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() &&
      differenceDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect()) &&
      (inputDataSource->getId() == differenceDataSource->getId()) &&
      (m_inputSelectedLayer->getSRID() == m_differenceSelectedLayer->getSRID()))
    {
      isQuery = true;

// Get Select Query using AbstractLayerPtr to process by spatial database. 
      inputSelect = te::vp::GetSelectQueryFromLayer(m_inputSelectedLayer, inputIsChecked, inputSRID);
      differenceSelect = te::vp::GetSelectQueryFromLayer(m_differenceSelectedLayer, differenceIsChecked, differenceSRID);

      if (inputSelect)
        structInputParams1.m_inputQuery = inputSelect;

      if (differenceSelect)
        structInputParams2.m_inputQuery = differenceSelect;
    }
    else
    {

// Get DataSet and DataSetType using AbstractLayerPtr to process by memory, using GEOS.
      te::vp::DataStruct inputData = te::vp::GetDataStructFromLayer(m_inputSelectedLayer, inputIsChecked, inputSRID);

      if (inputData.m_dataSet)
        structInputParams1.m_inputDataSet = inputData.m_dataSet;

      if (inputData.m_dataSetType)
        structInputParams1.m_inputDataSetType = inputData.m_dataSetType;


      te::vp::DataStruct differenceData = te::vp::GetDataStructFromLayer(m_differenceSelectedLayer, differenceIsChecked, differenceSRID);

      if (differenceData.m_dataSet)
        structInputParams2.m_inputDataSet = differenceData.m_dataSet;

      if (differenceData.m_dataSetType)
        structInputParams2.m_inputDataSetType = differenceData.m_dataSetType;
    }


    m_inputParams.push_back(structInputParams1);
    m_inputParams.push_back(structInputParams2);


// Get the output dataset name.
    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

// Return of operation result.
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

      this->setCursor(Qt::WaitCursor);

// Set parameters (Input/Output).
      m_params = new te::vp::AlgorithmParams();
      m_params->setInputParams(m_inputParams);
      m_params->setOutputDataSource(dsOGR);
      m_params->setOutputDataSetName(outputdataset);
      m_params->setSpecificParams(specificParams);

      te::vp::Difference difference;

// Select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities inputDSCapabilities = inputDataSource->getCapabilities();
      const te::da::DataSourceCapabilities differenceDSCapabilities = differenceDataSource->getCapabilities();

      if (isQuery)
      {
        res = difference.executeQuery(m_params);
      }
      else
      {
        res = difference.executeMemory(m_params);
      }

      if(!res)
      {
        dsOGR->close();

        QMessageBox::information(this, "Difference", "Error: could not generate the difference.");

        te::common::ProgressManager::getInstance().removeViewer(id);

        reject();
      }

      m_outputDatasource = te::da::DataSourceInfoManager::getInstance().get(dsOGR->getId());

      if (!m_outputDatasource)
      {
        QMessageBox::information(this, "Difference", "The output data source can not be accessed.");

        te::common::ProgressManager::getInstance().removeViewer(id);

        return;
      }

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

      std::string name = te::common::Convert2LCase(outputdataset);

      if (aux->dataSetExists(name))
      {
        QMessageBox::information(this, "Difference", "Dataset already exists. Remove it or select a new name and try again.");

        te::common::ProgressManager::getInstance().removeViewer(id);

        return;
      }

      this->setCursor(Qt::WaitCursor);

// Set parameters (Input/Output).
      m_params = new te::vp::AlgorithmParams();
      m_params->setInputParams(m_inputParams);
      m_params->setOutputDataSource(aux);
      m_params->setOutputDataSetName(outputdataset);
      m_params->setSpecificParams(specificParams);

      te::vp::Difference difference;

// Select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities inputDSCapabilities = inputDataSource->getCapabilities();
      const te::da::DataSourceCapabilities differenceDSCapabilities = differenceDataSource->getCapabilities();

      if(isQuery)
      {
        res = difference.executeQuery(m_params);
      }
      else
      {
        res = difference.executeMemory(m_params);
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

