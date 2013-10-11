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
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Property.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../Exception.h"
#include "../Intersection.h"
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
    QMessageBox::warning(this, TR_VP("Intersection Operation"), TR_VP("Choose an input layer."));
    return;
  }

  std::auto_ptr<te::map::LayerSchema> firstSchema = m_firstSelectedLayer->getSchema();

  if(!firstSchema->hasGeom())
  {
    QMessageBox::warning(this, TR_VP("Intersection Operation"), TR_VP("The input layer do not have a geometry column!"));
    return;
  }

  if(m_ui->m_secondLayerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, TR_VP("Intersection Operation"), TR_VP("Choose an input layer."));
    return;
  }

  std::auto_ptr<te::map::LayerSchema> secondSchema = m_secondSelectedLayer->getSchema();

  if(!secondSchema->hasGeom())
  {
    QMessageBox::warning(this, TR_VP("Intersection Operation"), TR_VP("The input layer do not have a geometry column!"));
    return;
  }

  bool copyInputColumns = m_ui->m_copyColumnsCheckBox->isChecked();

  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
  if(outputdataset.empty())
  {
    QMessageBox::warning(this, TR_VP("Intersection Operation"), TR_VP("It is necessary a name for the new layer."));
    return;
  }

  if(m_ui->m_repositoryLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, TR_VP("Intersection Operation"), TR_VP("Set a repository for the new Layer."));

    return;
  }
  
  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);
  
  try
  {
    te::map::DataSetLayer* firstDataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_firstSelectedLayer.get());
    te::da::DataSourcePtr firstDataSource = te::da::GetDataSource(firstDataSetLayer->getDataSourceId(), true);

    te::map::DataSetLayer* secondDataSetLayer = dynamic_cast<te::map::DataSetLayer*>(m_firstSelectedLayer.get());
    te::da::DataSourcePtr secondDataSource = te::da::GetDataSource(secondDataSetLayer->getDataSourceId(), true);

    if (!firstDataSource.get() || !secondDataSource)
    {
      QMessageBox::information(this, "Intersection", "Error: can not find the input datasource.");
      return;
    }

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

      std::auto_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make("OGR");
      dsOGR->setConnectionInfo(dsinfo);
      dsOGR->open();
      if(dsOGR->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Intersection", "Error: there is already a dataset with the requested output name.");
        return;
      }

      res = te::vp::Intersection( firstDataSetLayer->getDataSetName(),
                                  firstDataSource.get(),
                                  secondDataSetLayer->getDataSetName(),
                                  secondDataSource.get(),
                                  copyInputColumns,
                                  outputdataset, 
                                  dsOGR.get());

      if(!res)
      {
        dsOGR->close();
        QMessageBox::information(this, "Intersection", "Error: could not generate the intersection.");
        reject();
      }
      dsOGR->close();

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
      te::da::DataSourcePtr aux = te::da::DataSourceManager::getInstance().find(m_outputDatasource->getId());
      res = te::vp::Intersection(firstDataSetLayer->getDataSetName(),
                                firstDataSource.get(),
                                secondDataSetLayer->getDataSetName(),
                                secondDataSource.get(),
                                copyInputColumns,
                                outputdataset, 
                                aux.get());
      if(!res)
      {
        QMessageBox::information(this, "Intersection", "Error: could not generate the intersection.");
        reject();
      }
    }

    // creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::DataSourceManager::getInstance().find(m_outputDatasource->getId());
    
    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
      
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_layerResult = converter(dt);
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, TR_VP("Intersection"), e.what());
    return;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  accept();
}

void te::vp::IntersectionDialog::onTargetDatasourceToolButtonPressed()
{
  m_outputDatasource.reset();
  m_outputArchive = "";

  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
}

void te::vp::IntersectionDialog::onTargetFileToolButtonPressed()
{
  m_outputDatasource.reset();
  m_outputArchive = "";

  QString directoryName = QFileDialog::getExistingDirectory(this, tr("Open Feature File"), QString(""));

  if(directoryName.isEmpty())
    return;

  QString fullName = directoryName + "\\" + m_ui->m_newLayerNameLineEdit->text() + ".shp";

  m_ui->m_repositoryLineEdit->setText(fullName);

  m_outputArchive = std::string(fullName.toStdString());
}
