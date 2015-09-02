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
  \file terralib/attributefill/RasterToVectorDialog.cpp

  \brief Raster to vector attributefill dialog.
*/

// TerraLib
#include "../../BuildConfig.h"
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
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
#include "../../raster/RasterProperty.h"
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Exception.h"
#include "RasterToVectorDialog.h"
#include "../RasterToVector.h"
#include "ui_RasterToVectorDialogForm.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::attributefill::RasterToVectorDialog::RasterToVectorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::RasterToVectorDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>())
{
  // add controls
  m_ui->setupUi(this);

  // add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("raster-vector-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_inRasterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRasterComboBoxChanged(int)));
  connect(m_ui->m_inVectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVectorComboBoxChanged(int)));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/attributefill/attrfill_raster_to_vector.html");

}

te::attributefill::RasterToVectorDialog::~RasterToVectorDialog()
{
}

void te::attributefill::RasterToVectorDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if(dsType->hasRaster())
      m_ui->m_inRasterComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    if(dsType->hasGeom())
      m_ui->m_inVectorComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

te::map::AbstractLayerPtr te::attributefill::RasterToVectorDialog::getLayer()
{
  return m_outLayer;
}

std::vector<unsigned int> te::attributefill::RasterToVectorDialog::getSelectedBands()
{
  std::vector<unsigned int> vecBands;
  
  for(int i = 0; i < m_ui->m_bandsListWidget->count(); ++i)
  {
    if(m_ui->m_bandsListWidget->isItemSelected(m_ui->m_bandsListWidget->item(i)))
    {
      vecBands.push_back(i);
    }
  }
  return vecBands;
}

std::vector<te::stat::StatisticalSummary> te::attributefill::RasterToVectorDialog::getSelectedStatistics()
{
  std::vector<te::stat::StatisticalSummary> vecStatistics;
  
  for(int i = 0; i < m_ui->m_statisticsListWidget->count(); ++i)
  {
    if(m_ui->m_statisticsListWidget->isItemSelected(m_ui->m_statisticsListWidget->item(i)))
    {
      switch(i)
      {
        case 0:
          vecStatistics.push_back(te::stat::MIN_VALUE);
          break;
        case 1:
          vecStatistics.push_back(te::stat::MAX_VALUE);
          break;
        case 2:
          vecStatistics.push_back(te::stat::MEAN);
          break;
        case 3:
          vecStatistics.push_back(te::stat::SUM);
          break;
        case 4:
          vecStatistics.push_back(te::stat::COUNT);
          break;
        case 5:
          vecStatistics.push_back(te::stat::VALID_COUNT);
          break;
        case 6:
          vecStatistics.push_back(te::stat::STANDARD_DEVIATION);
          break;
        case 7:
          vecStatistics.push_back(te::stat::VARIANCE);
          break;
        case 8:
          vecStatistics.push_back(te::stat::SKEWNESS);
          break;
        case 9:
          vecStatistics.push_back(te::stat::KURTOSIS);
          break;
        case 10:
          vecStatistics.push_back(te::stat::AMPLITUDE);
          break;
        case 11:
          vecStatistics.push_back(te::stat::MEDIAN);
          break;
        case 12:
          vecStatistics.push_back(te::stat::VAR_COEFF);
          break;
        case 13:
          vecStatistics.push_back(te::stat::MODE);
          break;
        case 14:
          vecStatistics.push_back(te::stat::PERCENT_EACH_CLASS_BY_AREA);
          break;
        default:
          continue;
      }
    }
  }
  return vecStatistics;
}

void te::attributefill::RasterToVectorDialog::onRasterComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  
  std::string layerID = m_ui->m_inRasterComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
      m_rasterLayer = it->get();
    
    ++it;
  }

  m_ui->m_bandsListWidget->clear();

  std::auto_ptr<te::da::DataSetType> dsType = m_rasterLayer->getSchema();
  te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsType.get());
  std::auto_ptr<te::da::DataSet> dsRaster = m_rasterLayer->getData();
  std::auto_ptr<te::rst::Raster> raster = dsRaster->getRaster(rasterProp->getName());
  std::size_t n_bands = raster->getNumberOfBands();

  for(std::size_t b = 0; b < n_bands; ++b)
    m_ui->m_bandsListWidget->addItem(boost::lexical_cast<std::string>(b).c_str());


  m_ui->m_statisticsListWidget->clear();

  m_ui->m_statisticsListWidget->addItem("Minimum value");
  m_ui->m_statisticsListWidget->addItem("Maximum value");
  m_ui->m_statisticsListWidget->addItem("Mean");
  m_ui->m_statisticsListWidget->addItem("Sum of values");
  m_ui->m_statisticsListWidget->addItem("Total number of values");
  m_ui->m_statisticsListWidget->addItem("Total not null values");
  m_ui->m_statisticsListWidget->addItem("Standard deviation");
  m_ui->m_statisticsListWidget->addItem("Variance");
  m_ui->m_statisticsListWidget->addItem("Skewness");
  m_ui->m_statisticsListWidget->addItem("Kurtosis");
  m_ui->m_statisticsListWidget->addItem("Amplitude");
  m_ui->m_statisticsListWidget->addItem("Median");
  m_ui->m_statisticsListWidget->addItem("Coefficient variation");
  m_ui->m_statisticsListWidget->addItem("Mode");
  m_ui->m_statisticsListWidget->addItem("Percent of each class by area");

}

void te::attributefill::RasterToVectorDialog::onVectorComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  
  std::string layerID = m_ui->m_inVectorComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
      m_vectorLayer = it->get();
    
    ++it;
  }
}

void te::attributefill::RasterToVectorDialog::onTargetDatasourceToolButtonPressed()
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

void te::attributefill::RasterToVectorDialog::onTargetFileToolButtonPressed()
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

void te::attributefill::RasterToVectorDialog::onOkPushButtonClicked()
{
  if(m_ui->m_inRasterComboBox->count() == 0)
  {
    QMessageBox::information(this, "Fill", "Select an input raster layer.");
    return;
  }

  if(m_ui->m_inVectorComboBox->count() == 0)
  {
    QMessageBox::information(this, "Fill", "Select an input vector layer.");
    return;
  }


  te::map::DataSetLayer* dsRasterLayer = dynamic_cast<te::map::DataSetLayer*>(m_rasterLayer.get());

  if(!dsRasterLayer)
  {
    QMessageBox::information(this, "Fill", "Can not execute this operation on this type of layer.");
    return;
  }

  te::map::DataSetLayer* dsVectorLayer = dynamic_cast<te::map::DataSetLayer*>(m_vectorLayer.get());

  if(!dsVectorLayer)
  {
    QMessageBox::information(this, "Fill", "Can not execute this operation on this type of layer.");
    return;
  }
  
  te::da::DataSourcePtr inRasterDataSource = te::da::GetDataSource(dsRasterLayer->getDataSourceId(), true);
  if (!inRasterDataSource.get())
  {
    QMessageBox::information(this, "Fill", "The selected raster data source can not be accessed.");
    return;
  }

  te::da::DataSourcePtr inVectorDataSource = te::da::GetDataSource(dsVectorLayer->getDataSourceId(), true);
  if (!inVectorDataSource.get())
  {
    QMessageBox::information(this, "Fill", "The selected vector data source can not be accessed.");
    return;
  }

  std::vector<unsigned int> vecBands = getSelectedBands();
  if(vecBands.empty())
  {
    QMessageBox::information(this, "Fill", "Select at least one band.");
    return;
  }


  std::vector<te::stat::StatisticalSummary> vecStatistics = getSelectedStatistics();
  m_texture = m_ui->m_textureCheckBox->isChecked();

  if(vecStatistics.empty() && m_texture == false)
  {
    QMessageBox::information(this, "Fill", "Select at least one statistic operation or select the texture checkbox.");
    return;
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a repository for the result.");
    return;
  }

  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Fill", "Define a name for the resulting layer.");
    return;
  }
  
  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    bool res;

    if(m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());
      
      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Fill", "Output file already exists. Remove it or select a new name and try again.");
        return;
      }

      std::size_t idx = outputdataset.find(".");
      if (idx != std::string::npos)
        outputdataset=outputdataset.substr(0,idx);

      std::map<std::string, std::string> dsinfo;
      dsinfo["URI"] = uri.string();

      te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
      dsOGR->setConnectionInfo(dsinfo);
      dsOGR->open();
      if (dsOGR->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Fill", "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
        return;
      }

      this->setCursor(Qt::WaitCursor);

      te::attributefill::RasterToVector* rst2vec = new te::attributefill::RasterToVector();
      rst2vec->setInput(inRasterDataSource, 
                        dsRasterLayer->getDataSetName(), 
                        dsRasterLayer->getSchema(),
                        inVectorDataSource, 
                        dsVectorLayer->getDataSetName(),
                        dsVectorLayer->getSchema());

      rst2vec->setParams(vecBands, vecStatistics, m_texture);

      rst2vec->setOutput(dsOGR, outputdataset);
      
      if (!rst2vec->paramsAreValid())
        res = false;
      else
        res = rst2vec->run();

      if (!res)
      {
        this->setCursor(Qt::ArrowCursor);
        dsOGR->close();
        QMessageBox::information(this, "Fill", "Error: could not generate the operation.");
        reject();
      }
      dsOGR->close();

      delete rst2vec;

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
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux)
      {
        QMessageBox::information(this, "Fill", "The selected output datasource can not be accessed.");
        return;
      }

      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Fill", "Dataset already exists. Remove it or select a new name and try again.");
        return;
      }
      this->setCursor(Qt::WaitCursor);

      te::attributefill::RasterToVector* rst2vec = new te::attributefill::RasterToVector();
      
      rst2vec->setInput(inRasterDataSource, 
                        dsRasterLayer->getDataSetName(), 
                        dsRasterLayer->getSchema(),
                        inVectorDataSource, 
                        dsVectorLayer->getDataSetName(),
                        dsVectorLayer->getSchema());

      rst2vec->setParams(vecBands, vecStatistics, m_texture);

      rst2vec->setOutput(aux, outputdataset);

      if (!rst2vec->paramsAreValid())
        res = false;
      else
        res = rst2vec->run();

      delete rst2vec;

      if (!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Fill", "Error: could not generate the operation.");
        te::common::ProgressManager::getInstance().removeViewer(id);

        reject();
      }
    }

    // creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    
    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
      
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_outLayer = converter(dt);
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);

    QMessageBox::information(this, "Fill", e.what());

#ifdef TERRALIB_LOGGER_ENABLED
    te::common::Logger::logDebug("attributefill", e.what());
#endif // TERRALIB_LOGGER_ENABLED

    te::common::ProgressManager::getInstance().removeViewer(id);

    return;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);
  accept();
}

void te::attributefill::RasterToVectorDialog::onCancelPushButtonClicked()
{
  reject();
}