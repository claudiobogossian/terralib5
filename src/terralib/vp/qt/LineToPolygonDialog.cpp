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
  \file terralib/vp/LineToPolygonDialog.cpp

  \brief A dialog for line to polygon operation
*/

// TerraLib
#include "../../common/progress/ProgressManager.h"
#include "../../common/Logger.h"
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
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../Config.h"
#include "../Exception.h"
#include "../LineToPolygonMemory.h"
#include "../LineToPolygonOp.h"
#include "../LineToPolygonQuery.h"
#include "LineToPolygonDialog.h"
#include "ui_LineToPolygonDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>
#include <QSize>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::LineToPolygonDialog::LineToPolygonDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::LineToPolygonDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_selectedLayer(0),
    m_toFile(false)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-line-polygon-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

//signals

  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/vp/vp_lineToPolygon.html");
}

te::vp::LineToPolygonDialog::~LineToPolygonDialog()
{
}

void te::vp::LineToPolygonDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
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

    if( geomType == te::gm::LineStringMType || geomType == te::gm::LineStringType || geomType == te::gm::LineStringZMType || geomType == te::gm::LineStringZType ||
      geomType == te::gm::MultiLineStringMType || geomType == te::gm::MultiLineStringType || geomType == te::gm::MultiLineStringZMType || geomType == te::gm::MultiLineStringZType)
      m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));

    ++it;
  }
}

te::map::AbstractLayerPtr te::vp::LineToPolygonDialog::getLayer()
{
  return m_outLayer;
}

void te::vp::LineToPolygonDialog::onLayerComboBoxChanged(int index)
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

void te::vp::LineToPolygonDialog::onTargetDatasourceToolButtonPressed()
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

void te::vp::LineToPolygonDialog::onTargetFileToolButtonPressed()
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

void te::vp::LineToPolygonDialog::onOkPushButtonClicked()
{
  if(m_ui->m_layersComboBox->count() == 0)
  {
    QMessageBox::information(this, "Line to polygon", "Select input layer.");
    return;
  }

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_selectedLayer.get());
  if(!dsLayer)
  {
    QMessageBox::information(this, "Line to polygon", "Can not execute this operation on this type of layer.");
  }

  const te::da::ObjectIdSet* oidSet = 0;
  if(m_ui->m_onlySelectedCheckBox->isChecked())
  {
    oidSet = m_selectedLayer->getSelected();
    if(!oidSet)
    {
      QMessageBox::information(this, "Line to polygon", "Select the layer objects to perform the polygon to line operation.");
      return;
    }
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  if(!inDataSource.get())
  {
    QMessageBox::information(this, "Line to polygon", "The selected input data source can not be accessed.");
    return;
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Line to polygon", "Define a repository for the result.");
    return;
  }

  std::string outputDataSet = m_ui->m_newLayerNameLineEdit->text().toStdString();
  if(outputDataSet.empty())
  {
    QMessageBox::information(this, "Line to polygon", "Define a name for the resulting layer.");
    return;
  }

  bool res;

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    if(m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());
      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Line to polygon", "Output file already exists. Remove it or select a new name and try again.");
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
        QMessageBox::information(this, "Line to polygon", "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
        return;
      }

      std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(dsLayer->getSchema().get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));

      te::da::AssociateDataSetTypeConverterSRID(converter.get(), dsLayer->getSRID());

      this->setCursor(Qt::WaitCursor);

      te::vp::LineToPolygonOp* line2polygonOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();

      if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
        line2polygonOp = new te::vp::LineToPolygonQuery();
      else
        line2polygonOp = new te::vp::LineToPolygonMemory();

      line2polygonOp->setInput(inDataSource, dsLayer->getDataSetName(), converter, oidSet);
      line2polygonOp->setOutput(dsOGR, outputDataSet);

      if(!line2polygonOp->paramsAreValid())
        res = false;
      else
        res = line2polygonOp->run();

      if(!res)
      {
        this->setCursor(Qt::ArrowCursor);
        dsOGR->close();
        QMessageBox::information(this, "Line to polygon", "Error: Error in operation.");
        reject();
      }
      dsOGR->close();

      delete line2polygonOp;

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
        QMessageBox::information(this, "Line to polygon", "The selected output datasource can not be accessed.");
        return;
      }

      if(dsOGR->dataSetExists(outputDataSet))
      {
        QMessageBox::information(this, "Line to polygon", "Dataset already exists. Remove ir or select a new name and try again.");
        return;
      }

      std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(dsLayer->getSchema().get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));

      te::da::AssociateDataSetTypeConverterSRID(converter.get(), dsLayer->getSRID());

      this->setCursor(Qt::WaitCursor);

      te::vp::LineToPolygonOp* line2polygonOp = 0;

      const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();

      if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
        line2polygonOp = new te::vp::LineToPolygonQuery();
      else
        line2polygonOp = new te::vp::LineToPolygonMemory();

      line2polygonOp->setInput(inDataSource, dsLayer->getDataSetName(), converter, oidSet);
      line2polygonOp->setOutput(dsOGR, outputDataSet);

      if(!line2polygonOp->paramsAreValid())
        res = false;
      else
        res = line2polygonOp->run();

      delete line2polygonOp;

      if(!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Line to polygon", "Error in operation.");
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

    QMessageBox::information(this, "Line to polygon", e.what());

#ifdef TERRALIB_LOGGER_ENABLED
    std::string str = "Line to polygon - ";
    str += e.what();
    te::common::Logger::logDebug("vp", str.c_str());
#endif

    te::common::ProgressManager::getInstance().removeViewer(id);
    return;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);

  accept();
}

void te::vp::LineToPolygonDialog::onCancelPushButtonClicked()
{
  reject();
}
