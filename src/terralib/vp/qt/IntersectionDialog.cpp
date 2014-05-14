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
  \file terralib/vp/qt/IntersectionDialog.cpp

  \brief A dialog for intersection operation
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Property.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../srs/Config.h"
#include "../Exception.h"
#include "../IntersectionMemory.h"
#include "../IntersectionOp.h"
#include "../IntersectionQuery.h"
#include "../qt/widgets/layer/utils/DataSet2Layer.h"
#include "IntersectionDialog.h"
#include "LayerTreeModel.h"
#include "ui_IntersectionDialogForm.h"
#include "VectorProcessingConfig.h"
#include "Utils.h"

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeWidget>

// BOOST
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::IntersectionDialog::IntersectionDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::IntersectionDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>())
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme(VP_IMAGES"/vp-intersection-hint").pixmap(48,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_firstLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFirstLayerComboBoxChanged(int)));
  connect(m_ui->m_secondLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSecondLayerComboBoxChanged(int)));
  //connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/vp/vp_intersection.html");
}

te::vp::IntersectionDialog::~IntersectionDialog()
{
}

void te::vp::IntersectionDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if(dsType->hasGeom())
      m_ui->m_firstLayerComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    ++it;
  }
}

te::map::AbstractLayerPtr te::vp::IntersectionDialog::getLayer()
{
  return m_layerResult;
}

void te::vp::IntersectionDialog::onFirstLayerComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_firstLayerComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  m_ui->m_secondLayerComboBox->clear();
  while(it != m_layers.end())
  {
    if(layerID != it->get()->getId().c_str())
    {
      std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
      if(dsType->hasGeom())
        m_ui->m_secondLayerComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    }
    else
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_firstSelectedLayer = selectedLayer;
    }

    ++it;
  }
}

void te::vp::IntersectionDialog::onSecondLayerComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_secondLayerComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_secondSelectedLayer = selectedLayer;
    }

    ++it;
  }
}

void te::vp::IntersectionDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Intersection Operation", "Under development");
}

void te::vp::IntersectionDialog::onOkPushButtonClicked()
{
  if(m_ui->m_firstLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TR_VP("Intersection"), TR_VP("Select a first input layer."));
    return;
  }
  
  te::map::DataSetLayer* firstDataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_firstSelectedLayer.get());
  if(!firstDataSetLayer)
  {
    QMessageBox::information(this, "Intersection", "Can not execute this operation on this type of first layer.");
    return;
  }

  std::auto_ptr<te::da::DataSet> firstSelectedDS;
  if(m_ui->m_firstCheckBox->isChecked())
  {
    std::auto_ptr<const te::da::ObjectIdSet> firstOidSet(m_firstSelectedLayer->getSelected());
    if(!firstOidSet.get())
    {
      QMessageBox::information(this, "Intersection", "Select the layer objects to perform the intersection operation.");
      return;
    }
    firstSelectedDS = firstDataSetLayer->getData(firstOidSet.get());
  }

  te::da::DataSourcePtr firstDataSource = te::da::GetDataSource(firstDataSetLayer->getDataSourceId(), true);
  if (!firstDataSource.get())
  {
    QMessageBox::information(this, "Intersection", "The selected first input data source can not be accessed.");
    return;
  }

  if(m_ui->m_secondLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TR_VP("Intersection"), TR_VP("Select a second input layer."));
    return;
  }
  
  te::map::DataSetLayer* secondDataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_secondSelectedLayer.get());
  if(!secondDataSetLayer)
  {
    QMessageBox::information(this, "Intersection", "Can not execute this operation on this type of second layer.");
    return;
  }
  
  te::da::DataSourcePtr secondDataSource = te::da::GetDataSource(secondDataSetLayer->getDataSourceId(), true);
  if (!secondDataSource.get())
  {
    QMessageBox::information(this, "Intersection", "The selected second input data source can not be accessed.");
    return;
  }

  std::auto_ptr<te::da::DataSet> secondSelectedDS;
  if(m_ui->m_secondCheckBox->isChecked())
  {
    std::auto_ptr<const te::da::ObjectIdSet> secondOidSet(m_secondSelectedLayer->getSelected());
    if(!secondOidSet.get())
    {
      QMessageBox::information(this, "Intersection", "Select the layer objects to perform the intersection operation.");
      return;
    }
    secondSelectedDS = secondDataSetLayer->getData(secondOidSet.get());
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, TR_VP("Intersection"), TR_VP("Select a repository for the resulting layer."));
    return;
  }
  
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, TR_VP("Intersection"), TR_VP("Define a name for the resulting layer."));
    return;
  }
  
  if ((firstDataSetLayer->getSRID() == TE_UNKNOWN_SRS && secondDataSetLayer->getSRID() != TE_UNKNOWN_SRS) ||
      (firstDataSetLayer->getSRID() != TE_UNKNOWN_SRS && secondDataSetLayer->getSRID() == TE_UNKNOWN_SRS))
  {
    int ret = QMessageBox::question(this, "Intersection", "The two layers have incompatible SRS. The result might be incorrect. Do you wish to continue?", QMessageBox::No, QMessageBox::Yes);
    if (ret == QMessageBox::No)
      return;
  }

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);
  
  try
  {
    bool copyInputColumns = m_ui->m_copyColumnsCheckBox->isChecked();
    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();

    bool res;
    if (m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());
      
      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Intersection", "Output file already exists. Remove it and try again. ");
        return;
      }

      std::size_t idx = outputdataset.find(".");
      if(idx != std::string::npos)
        outputdataset = outputdataset.substr(0, idx);

      std::map<std::string, std::string> dsinfo;
      dsinfo["URI"] = uri.string();

      te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
      dsOGR->setConnectionInfo(dsinfo);
      dsOGR->open();
      if(dsOGR->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Intersection", "Output file already exists. Remove it or select a new name and try again.");
        return;
      }
      
      this->setCursor(Qt::WaitCursor);

      te::vp::IntersectionOp* intersectionOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities firstDSCapabilities = firstDataSource->getCapabilities();
      const te::da::DataSourceCapabilities secondDSCapabilities = secondDataSource->getCapabilities();

      if( firstDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          secondDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          (firstDataSource->getId() == secondDataSource->getId()))
      {
        intersectionOp = new te::vp::IntersectionQuery();
      }
      else
      {
        intersectionOp = new te::vp::IntersectionMemory();
      }

      intersectionOp->setInput( firstDataSource, firstDataSetLayer->getData(), firstDataSetLayer->getSchema(),
                                secondDataSource, secondDataSetLayer->getData(), secondDataSetLayer->getSchema());
      intersectionOp->setOutput(dsOGR, outputdataset);
      intersectionOp->setParams(copyInputColumns, firstDataSetLayer->getSRID());

      if (!intersectionOp->paramsAreValid())
        res = false;
      else
        res = intersectionOp->run();

      if(!res)
      {
        dsOGR->close();
        QMessageBox::information(this, "Intersection", "Error: could not generate the intersection.");
        reject();
      }
      dsOGR->close();

      delete intersectionOp;

      // let's include the new datasource in the managers
      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);
      
      te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
      ds->setConnInfo(dsinfo);
      ds->setTitle(uri.stem().string());
      ds->setAccessDriver("OGR");
      ds->setType("OGR");
      ds->setDescription(uri.string());
      ds->setId(id);
      
      te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id, "OGR", ds->getConnInfo());
      newds->open();
      te::da::DataSourceInfoManager::getInstance().add(ds);
      m_outputDatasource = ds;
    }
    else
    {
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux.get())
      {
        QMessageBox::information(this, "Intersection", "The output data source can not be accessed.");
        return;
      }
      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Intersection", "Dataset already exists. Remove it or select a new name and try again. ");
        return;
      }
      this->setCursor(Qt::WaitCursor);

      te::vp::IntersectionOp* intersectionOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities firstDSCapabilities = firstDataSource->getCapabilities();
      const te::da::DataSourceCapabilities secondDSCapabilities = secondDataSource->getCapabilities();

      if( firstDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          secondDSCapabilities.getQueryCapabilities().supportsSpatialSQLDialect() && 
          (firstDataSource->getId() == secondDataSource->getId()) &&
          (firstDataSetLayer->getSRID() == secondDataSetLayer->getSRID()))
      {
        intersectionOp = new te::vp::IntersectionQuery();
      }
      else
      {
        intersectionOp = new te::vp::IntersectionMemory();
      }

      intersectionOp->setInput( firstDataSource, firstDataSetLayer->getData(), firstDataSetLayer->getSchema(),
                                secondDataSource, secondDataSetLayer->getData(), secondDataSetLayer->getSchema());
      intersectionOp->setOutput(aux, outputdataset);
      intersectionOp->setParams(copyInputColumns, firstDataSetLayer->getSRID());

      if (!intersectionOp->paramsAreValid())
        res = false;
      else
        res = intersectionOp->run();

      delete intersectionOp;

      if(!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Intersection", "Error: could not generate the intersection.");
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
    QMessageBox::warning(this, TR_VP("Intersection"), e.what());

    te::common::Logger::logDebug("vp", e.what());
    te::common::ProgressManager::getInstance().removeViewer(id);
    return;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);
  accept();
}

void te::vp::IntersectionDialog::onTargetDatasourceToolButtonPressed()
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

void te::vp::IntersectionDialog::onTargetFileToolButtonPressed()
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
