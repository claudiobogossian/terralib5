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
  \file terralib/vp/MultipartToSinglepartDialog.cpp

  \brief A Multipart to Singlepart operation dialog
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
#include "../../maptools/AbstractLayer.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Exception.h"
#include "../MultipartToSinglepart.h"
#include "MultipartToSinglepartDialog.h"
#include "ui_MultipartToSinglepartDialogForm.h"
#include "Utils.h"

// Qt
#include <QFileDialog>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTreeWidget>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::vp::MultipartToSinglepartDialog::MultipartToSinglepartDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::MultipartToSinglepartDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_selectedLayer(0),
    m_toFile(false)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("vp-multiparttosinglepart-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/vp/vp_multiparttosinglepart.html");

  m_outputDatasource = te::da::DataSourceInfoPtr();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
}

te::vp::MultipartToSinglepartDialog::~MultipartToSinglepartDialog()
{
}

void te::vp::MultipartToSinglepartDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while(it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if(dsType->hasGeom())
      m_ui->m_layersComboBox->addItem(it->get()->getTitle().c_str(), QVariant(QVariant::fromValue(*it)));

    ++it;
  }
}

te::map::AbstractLayerPtr te::vp::MultipartToSinglepartDialog::getLayer()
{
  return m_layer;
}

void te::vp::MultipartToSinglepartDialog::onLayerComboBoxChanged(int index)
{
  QVariant varLayer = m_ui->m_layersComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  m_selectedLayer = layer;

  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
}

void te::vp::MultipartToSinglepartDialog::onTargetDatasourceToolButtonPressed()
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

void te::vp::MultipartToSinglepartDialog::onTargetFileToolButtonPressed()
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

void te::vp::MultipartToSinglepartDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::vp::MultipartToSinglepartDialog::onOkPushButtonClicked()
{
  if(m_ui->m_layersComboBox->count() == 0)
  {
    QMessageBox::information(this, tr("Multipart To Singlepart"), tr("Select an input layer."));
    return;
  }
  
  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_selectedLayer.get());

  if(!dsLayer)
  {
    QMessageBox::information(this, tr("Multipart To Singlepart"), tr("Can not execute this operation on this type of layer."));
    return;
  }

  const te::da::ObjectIdSet* oidSet = 0;

  if(m_ui->m_onlySelectedCheckBox->isChecked())
  {
    oidSet = m_selectedLayer->getSelected();
    if(!oidSet)
    {
      QMessageBox::information(this, tr("Multipart To Singlepart"), tr("None selected object!"));
      return;
    }
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    QMessageBox::information(this, tr("Multipart To Singlepart"), tr("The selected input data source can not be accessed."));
    return;
  }

  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
  
  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Multipart To Singlepart"), tr("Define a repository for the result."));
    return;
  }

  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Multipart To Singlepart"), "Define a name for the resulting layer.");
    return;
  }

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    bool res;
    
    if (m_toFile)
    {
      boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());
      
      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, tr("Multipart To Singlepart"), "Output file already exists. Remove it or select a new name and try again.");
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
        QMessageBox::information(this, tr("Multipart To Singlepart"), "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
        return;
      }

      this->setCursor(Qt::WaitCursor);

      te::vp::MultipartToSinglepart* m2s = new te::vp::MultipartToSinglepart();

      m2s->setInput(inDataSource, dsLayer->getDataSetName(),dsLayer->getSchema(), oidSet);
      m2s->setOutput(dsOGR, outputdataset);

      res = m2s->run();

      if (!res)
      {
        this->setCursor(Qt::ArrowCursor);
        dsOGR->close();
        QMessageBox::information(this, tr("Multipart To Singlepart"), tr("Error: could not operate."));
        reject();
      }
      dsOGR->close();

      delete m2s;
      
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
        QMessageBox::information(this, tr("Multipart To Singlepart"), "The selected output datasource can not be accessed.");
        return;
      }
      
      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, tr("Multipart To Singlepart"), "Dataset already exists. Remove it or select a new name and try again.");
        return;
      }
      this->setCursor(Qt::WaitCursor);

      te::vp::MultipartToSinglepart* m2s = new te::vp::MultipartToSinglepart();

      m2s->setInput(inDataSource, dsLayer->getDataSetName(), dsLayer->getSchema(), oidSet);
      m2s->setOutput(aux, outputdataset);

      res = m2s->run();

      delete m2s;

      if (!res)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, tr("Multipart To Singlepart"), tr("Error: could not operate."));
        reject();
      }
    }
    
    // creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    
    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
      
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_layer = converter(dt);
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);

    QMessageBox::information(this, tr("Multipart To Singlepart"), e.what());

#ifdef TERRALIB_LOGGER_ENABLED
    std::string str = "Multipart To Singlepart - ";
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

void te::vp::MultipartToSinglepartDialog::onCancelPushButtonClicked()
{
  reject();
}

