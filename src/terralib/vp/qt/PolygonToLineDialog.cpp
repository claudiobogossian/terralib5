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
  \file terralib/vp/PolygonToLineDialog.cpp

  \brief A dialog for polygon to line operation
*/

// TerraLib
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
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../Config.h"
#include "../Exception.h"
#include "../PolygonToLineMemory.h"
#include "../PolygonToLineOp.h"
#include "../PolygonToLineQuery.h"
#include "PolygonToLineDialog.h"
#include "ui_PolygonToLineDialogForm.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QSize>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::PolygonToLineDialog::PolygonToLineDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::PolygonToLineDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_selectedLayer(0),
    m_toFile(false)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-polygon-line-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

//signals

  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::vp::PolygonToLineDialog::~PolygonToLineDialog()
{
}

void te::vp::PolygonToLineDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::map::LayerSchema> layerSchema = it->get()->getSchema();
    te::gm::GeometryProperty* prop = static_cast<te::gm::GeometryProperty*>(layerSchema->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));
    
    if(!prop)
    {
      ++it;
      continue;
    }

    te::gm::GeomType geomType = prop->getGeometryType();

    if( geomType == te::gm::PolygonMType || geomType == te::gm::PolygonType || geomType == te::gm::PolygonZMType || geomType == te::gm::PolygonZType ||
      geomType == te::gm::MultiPolygonMType || geomType == te::gm::MultiPolygonType || geomType == te::gm::MultiPolygonZMType || geomType == te::gm::MultiPolygonZType)
      m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));

    ++it;
  }
}

te::map::AbstractLayerPtr te::vp::PolygonToLineDialog::getLayer()
{
  return m_outLayer;
}

void te::vp::PolygonToLineDialog::onLayerComboBoxChanged(int index)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  while(it != m_layers.end())
  {
    if(layerID == it->get()->getId().c_str())
    {
      te::map::AbstractLayerPtr selectedLayer = it->get();
      m_selectedLayer = selectedLayer;

      return;
    }
    ++it;
  }
}

void te::vp::PolygonToLineDialog::onTargetDatasourceToolButtonPressed()
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

void te::vp::PolygonToLineDialog::onTargetFileToolButtonPressed()
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

void te::vp::PolygonToLineDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::vp::PolygonToLineDialog::onOkPushButtonClicked()
{
  if(m_ui->m_layersComboBox->count() == 0)
  {
    QMessageBox::information(this, "Polygon to line", "Select input layer.");
    return;
  }

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_selectedLayer.get());
  if(!dsLayer)
  {
    QMessageBox::information(this, "Polygon to line", "Can not execute this operation on this type of layer.");
  }

  const te::da::ObjectIdSet* oidSet = 0;
  if(m_ui->m_onlySelectedCheckBox->isChecked())
  {
    oidSet = m_selectedLayer->getSelected();
    if(!oidSet)
    {
      QMessageBox::information(this, "Polygon to line", "Select the layer objects to perform the polygon to line operation.");
      return;
    }
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  if(!inDataSource.get())
  {
    QMessageBox::information(this, "Polygon to line", "The selected input data source can not be accessed.");
    return;
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Polygon to line", "Define a repository for the result.");
    return;
  }

  std::string outputDataSet = m_ui->m_newLayerNameLineEdit->text().toStdString();
  if(outputDataSet.empty())
  {
    QMessageBox::information(this, "Polygon to line", "Define a name for the resulting layer.");
    return;
  }

  bool res;

  try
  {
    if(m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());
      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Polygon to line", "Output file already exists. Remove it or select a new name and try again.");
        return;
      }

      std::size_t idx = outputDataSet.find(".");
      if (idx != std::string::npos)
        outputDataSet=outputDataSet.substr(0,idx);

      std::map<std::string, std::string> dsinfo;
      dsinfo["URI"] = uri.string();
      
      te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
      dsOGR->setConnectionInfo(dsinfo);
      dsOGR->open();
      if (dsOGR->dataSetExists(outputDataSet))
      {
        QMessageBox::information(this, "Polygon to Line", "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
        return;
      }

      this->setCursor(Qt::WaitCursor);

      te::vp::PolygonToLineOp* pol2LineOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();

      if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
        pol2LineOp = new te::vp::PolygonToLineQuery();
      else
        pol2LineOp = new te::vp::PolygonToLineMemory();

      pol2LineOp->setInput(inDataSource, dsLayer->getDataSetName(), oidSet);
      pol2LineOp->setOutput(dsOGR, outputDataSet);

      if(!pol2LineOp->paramsAreValid())
        res = false;
      else
        res = pol2LineOp->run();

      if(!res)
      {
        this->setCursor(Qt::ArrowCursor);
        dsOGR->close();
        QMessageBox::information(this, "Polygon to Line", "Error: Error in operation.");
        reject();
      }
      dsOGR->close();

      delete pol2LineOp;

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
      te::da::DataSourcePtr dsOGR = te::da::GetDataSource(m_outputDatasource->getId());
      if(!dsOGR)
      {
        QMessageBox::information(this, "Polygon to Line", "The selected output datasource can not be accessed.");
        return;
      }

      if(dsOGR->dataSetExists(outputDataSet))
      {
        QMessageBox::information(this, "Polygon to Line", "Dataset already exists. Remove ir or select a new name and try again.");
        return;
      }

      this->setCursor(Qt::WaitCursor);

      te::vp::PolygonToLineOp* pol2LineOp = 0;

      const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();

      if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
        pol2LineOp = new te::vp::PolygonToLineQuery();
      else
        pol2LineOp = new te::vp::PolygonToLineMemory();

      pol2LineOp->setInput(inDataSource, dsLayer->getDataSetName(), oidSet);
      pol2LineOp->setOutput(dsOGR, outputDataSet);

      if(!pol2LineOp->paramsAreValid())
        res = false;
      else
        res = pol2LineOp->run();

      delete pol2LineOp;

      if(!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Polygon to Line", "Error in operation.");
        reject();
      }
    }

    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputDataSet).release());
    
    m_outLayer = converter(dt);
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);

    QMessageBox::information(this, "Polygon to Line", e.what());
    
//    te::common::Logger::logDebug("vp", e.what());
//    te::common::ProgressManager::getInstance().removeViewer(id);
    return;
  }

//  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);

  accept();
}

void te::vp::PolygonToLineDialog::onCancelPushButtonClicked()
{
  reject();
}
